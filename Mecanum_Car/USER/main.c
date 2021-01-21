#include "led.h"
#include "beep.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"	 
#include "adc.h"
#include "includes.h"
#include "ESP8266.h"
#include "rtc.h"
#include "Kalman.h"
#include "pid.h"
#include "usart2.h"
#include "usart3.h"
#include "motor.h"
#include "encoder.h"

//int32_t L=250,W=200,R=75;                 //车尺寸，单位mm

//定义数据包内容
/*
摇杆数值范围：0-127
165,左摇杆X数值byte,左摇杆Y数值byte,校验和,90
*/
u8 u3_rx_data[16];//上位机数据


//手动模式输出期望编码器值
short m1_hope_output = 0;
short m2_hope_output = 0;
short m3_hope_output = 0;
short m4_hope_output = 0;

//正在输出的编码器值
u16 m1_output = 320;
u16 m2_output = 320;
u16 m3_output = 320;
u16 m4_output = 320;

//四路编码器脉冲计数值
int Encoder_M1=0;
int Encoder_M2=0;
int Encoder_M3=0;
int Encoder_M4=0;

//四路电机堵转标志位
u8 M1_blocking_flag = 0;
u8 M2_blocking_flag = 0;
u8 M3_blocking_flag = 0;
u8 M4_blocking_flag = 0;

//低电量log输出标志
u8 low_power_flag = 1;

//两路串口接收完成标志位
u8 usart2_rx_flag = 0;
u8 usart3_rx_flag = 0;


//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		64
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);
//log输出任务资源分配
#define LOG_TASK_PRIO		4
#define LOG_STK_SIZE 		256
OS_TCB LOGTaskTCB;
CPU_STK LOG_TASK_STK[LOG_STK_SIZE];
void LOG_task(void *p_arg);
//电机控制任务资源分配
#define MOTOR_CONTROL_PRIO		4
#define MOTOR_STK_SIZE 		64
OS_TCB MotorTaskTCB;
CPU_STK MOTOR_TASK_STK[MOTOR_STK_SIZE];
void MOTOR_CONTROL_task(void *p_arg);
//ADC采样任务资源分配
#define ADC_TASK_PRIO		4
#define ADC_STK_SIZE 		64
OS_TCB ADCTaskTCB;
CPU_STK ADC_TASK_STK[ADC_STK_SIZE];
void ADC_task(void *p_arg);

extern void usart3_init(u32 bound);

int main(void){
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
	//串口一被编码器一占用无法开启
	//uart_init(9600);	 	//串口初始化9600
	//和ESP的通信串口
	usart2_init(9600);
	//和HC05的通信串口
	usart3_init(115200);
	RTC_Init();	  			//RTC初始化
 	LED_Init();			     //LED端口初始化
	BEEP_Init();
	//初始化8266控制引脚
	ESP8266_Init();
	//四路编码器初始化
	Encoder_Init_TIM1();
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	Encoder_Init_TIM4();
	//初始化TIM8给PWM
	Motor_Init();
	//初始化TIM6定时获取编码器累计值
	TIM6_Int_Init(499,7199);
	//初始化UCOS
	OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII
	while(1);
}

//开始任务，初始化所有任务进程
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	CPU_Init();//初始化CPU状态
	OS_CRITICAL_ENTER();	//进入临界区
	//创建ADC任务
	OSTaskCreate((OS_TCB 	* )&ADCTaskTCB,		
			 (CPU_CHAR	* )"adc_task", 		
							 (OS_TASK_PTR )ADC_task, 			
							 (void		* )0,					
							 (OS_PRIO	  )ADC_TASK_PRIO,     
							 (CPU_STK   * )&ADC_TASK_STK[0],	
							 (CPU_STK_SIZE)ADC_STK_SIZE/10,	
							 (CPU_STK_SIZE)ADC_STK_SIZE,		
							 (OS_MSG_QTY  )0,					
							 (OS_TICK	  )0,					
							 (void   	* )0,					
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 (OS_ERR 	* )&err);				
	//创建电机控制任务
	OSTaskCreate((OS_TCB 	* )&MotorTaskTCB,		
			 (CPU_CHAR	* )"motor_task", 		
							 (OS_TASK_PTR )MOTOR_CONTROL_task, 			
							 (void		* )0,					
							 (OS_PRIO	  )MOTOR_CONTROL_PRIO,     
							 (CPU_STK   * )&MOTOR_TASK_STK[0],	
							 (CPU_STK_SIZE)MOTOR_STK_SIZE/10,	
							 (CPU_STK_SIZE)MOTOR_STK_SIZE,		
							 (OS_MSG_QTY  )0,					
							 (OS_TICK	  )0,					
							 (void   	* )0,					
							 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							 (OS_ERR 	* )&err);		
								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//进入临界区
}


volatile float Battery_Voltage = 0;//电池电压



//10.4 10.84 11.28 11.72 12.16 12.6
//ADC任务，采样电池剩余电量
void ADC_task(void *p_arg){
	u32 Battery_Value = 0;//电池电压ADC采样值
	OS_ERR err;
	p_arg = p_arg;
	Adc_Init();
	while(1){
		Battery_Value = Get_Adc(ADC_Channel_8);
		Battery_Value = f_kalman_fliter(Battery_Value);//ADC采样值过卡尔曼
		Battery_Voltage = (((double)Battery_Value * 3.3) / 1024)-0.3;//计算电池电压
		Battery_LED(Battery_Voltage);//更新电量指示LED
		if(Battery_Voltage <= 10.84){//判断低电量
			BEEP=!BEEP;
			u2_printf("LOG:low_power\r\n");//输出LOG
			if(low_power_flag){
				low_power_flag = 0;
				u2_printf("LOG:low_power\r\n");//输出LOG
			}
			//OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);		//触发任务切换
			//BEEP=!BEEP;
		}
		LED1=!LED1;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);		//触发任务切换
	}
}

extern int M1_UpdatePID_OUT(u16 set,u16 yout);
extern int M2_UpdatePID_OUT(u16 set,u16 yout);
extern int M3_UpdatePID_OUT(u16 set,u16 yout);
extern int M4_UpdatePID_OUT(u16 set,u16 yout);


void refresh_motor_speed(void){
	if(m1_hope_output < 0){
		m1_hope_output = -m1_hope_output;
		//m1反转
		GPIO_ResetBits(GPIOD,GPIO_Pin_5);
		GPIO_SetBits(GPIOD,GPIO_Pin_6);
	}else{
		//m1正转
		GPIO_SetBits(GPIOD,GPIO_Pin_5);						 //PB.5 输出高
		GPIO_ResetBits(GPIOD,GPIO_Pin_6);						 //PB.5 输出高
	}
	if(m2_hope_output < 0){
		//m2反转
		m2_hope_output = -m2_hope_output;
		GPIO_ResetBits(GPIOD,GPIO_Pin_7);						 //PB.5 输出高
		GPIO_SetBits(GPIOD,GPIO_Pin_8);						 //PB.5 输出高
	}else{
		//m2正转
		GPIO_SetBits(GPIOD,GPIO_Pin_7);						 //PB.5 输出高
		GPIO_ResetBits(GPIOD,GPIO_Pin_8);						 //PB.5 输出高
	}
	if(m3_hope_output < 0){
		m3_hope_output = -m3_hope_output;
		//m3反转
		GPIO_ResetBits(GPIOD,GPIO_Pin_9);						 //PB.5 输出高
		GPIO_SetBits(GPIOD,GPIO_Pin_10);						 //PB.5 输出高
	}else{
		//m3正转
		GPIO_SetBits(GPIOD,GPIO_Pin_9);						 //PB.5 输出高
		GPIO_ResetBits(GPIOD,GPIO_Pin_10);						 //PB.5 输出高
	}
	if(m4_hope_output < 0){
		m4_hope_output = -m4_hope_output;
		//m4反转
		GPIO_ResetBits(GPIOD,GPIO_Pin_11);						 //PB.5 输出高
		GPIO_SetBits(GPIOD,GPIO_Pin_12);						 //PB.5 输出高
	}else{
		//m4正转
		GPIO_SetBits(GPIOD,GPIO_Pin_11);						 //PB.5 输出高
		GPIO_ResetBits(GPIOD,GPIO_Pin_12);						 //PB.5 输出高
	}
}

//电机控制任务
void MOTOR_CONTROL_task(void *p_arg){
	OS_ERR err;
	short Axe_X = 0;
	short Axe_Y = 0;
	short Axe_W = 0;
	
	p_arg = p_arg;
	//四路编码器初始化
	Encoder_Init_TIM1();
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	Encoder_Init_TIM4();
	while(1){
		//接受完毕，正常接收到起始位，校验和正确
		if(usart3_rx_flag && u3_rx_data[0] == 165 && u3_rx_data[1] + u3_rx_data[2] + u3_rx_data[3] == u3_rx_data[4]){
			usart3_rx_flag = 0;//复位接收完成标志位
			Axe_X = (short)u3_rx_data[1] - 63;
			Axe_Y = (short)u3_rx_data[2] - 63;
			Axe_W = (short)u3_rx_data[3] - 63;
			//非旋转模式
			if(Axe_W == 0){
				//正常接收到了起始位,且校验和正确
				//X和Y的中心点为63，范围是0-126
				Axe_X = Axe_X * 2;
				Axe_Y = Axe_Y * 2;
				m1_hope_output = (Axe_Y + Axe_X);
				m2_hope_output = (Axe_Y - Axe_X);
				m3_hope_output = (Axe_Y - Axe_X);
				m4_hope_output = (Axe_Y + Axe_X);
				refresh_motor_speed();
			}else{
				Axe_W = Axe_W * 2;
				m1_hope_output = Axe_W;
				m3_hope_output = Axe_W;
				m2_hope_output = -Axe_W;
				m4_hope_output = -Axe_W;
				refresh_motor_speed();
			}
		}
		//refresh_motor_speed();
		//取编码器读数的绝对值
		if(Encoder_M1 < 0)	Encoder_M1 = -Encoder_M1;
		if(Encoder_M2 < 0)	Encoder_M2 = -Encoder_M2;
		if(Encoder_M3 < 0)	Encoder_M3 = -Encoder_M3;
		if(Encoder_M4 < 0)	Encoder_M4 = -Encoder_M4;
		//更新输出编码器值
		m1_output = M1_UpdatePID_OUT(m1_hope_output,Encoder_M1);
		m2_output = M2_UpdatePID_OUT(m2_hope_output,Encoder_M2);
		m3_output = M3_UpdatePID_OUT(m3_hope_output,Encoder_M3);
		m4_output = M4_UpdatePID_OUT(m4_hope_output,Encoder_M4);
		
		//更新CCR
		TIM_SetCompare1(TIM8,m1_output);
		TIM_SetCompare2(TIM8,m2_output);
		TIM_SetCompare3(TIM8,m3_output);
		TIM_SetCompare4(TIM8,m4_output);
		
		//u2_printf("M1:%dM2:%dM3:%dM4:%d\r\n",Encoder_M1,Encoder_M2,Encoder_M3,Encoder_M4);
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);		//触发任务切换
	}
}
