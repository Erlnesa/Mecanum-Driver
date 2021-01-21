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

//int32_t L=250,W=200,R=75;                 //���ߴ磬��λmm

//�������ݰ�����
/*
ҡ����ֵ��Χ��0-127
165,��ҡ��X��ֵbyte,��ҡ��Y��ֵbyte,У���,90
*/
u8 u3_rx_data[16];//��λ������


//�ֶ�ģʽ�������������ֵ
short m1_hope_output = 0;
short m2_hope_output = 0;
short m3_hope_output = 0;
short m4_hope_output = 0;

//��������ı�����ֵ
u16 m1_output = 320;
u16 m2_output = 320;
u16 m3_output = 320;
u16 m4_output = 320;

//��·�������������ֵ
int Encoder_M1=0;
int Encoder_M2=0;
int Encoder_M3=0;
int Encoder_M4=0;

//��·�����ת��־λ
u8 M1_blocking_flag = 0;
u8 M2_blocking_flag = 0;
u8 M3_blocking_flag = 0;
u8 M4_blocking_flag = 0;

//�͵���log�����־
u8 low_power_flag = 1;

//��·���ڽ�����ɱ�־λ
u8 usart2_rx_flag = 0;
u8 usart3_rx_flag = 0;


//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		64
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);
//log���������Դ����
#define LOG_TASK_PRIO		4
#define LOG_STK_SIZE 		256
OS_TCB LOGTaskTCB;
CPU_STK LOG_TASK_STK[LOG_STK_SIZE];
void LOG_task(void *p_arg);
//�������������Դ����
#define MOTOR_CONTROL_PRIO		4
#define MOTOR_STK_SIZE 		64
OS_TCB MotorTaskTCB;
CPU_STK MOTOR_TASK_STK[MOTOR_STK_SIZE];
void MOTOR_CONTROL_task(void *p_arg);
//ADC����������Դ����
#define ADC_TASK_PRIO		4
#define ADC_STK_SIZE 		64
OS_TCB ADCTaskTCB;
CPU_STK ADC_TASK_STK[ADC_STK_SIZE];
void ADC_task(void *p_arg);

extern void usart3_init(u32 bound);

int main(void){
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	//����һ��������һռ���޷�����
	//uart_init(9600);	 	//���ڳ�ʼ��9600
	//��ESP��ͨ�Ŵ���
	usart2_init(9600);
	//��HC05��ͨ�Ŵ���
	usart3_init(115200);
	RTC_Init();	  			//RTC��ʼ��
 	LED_Init();			     //LED�˿ڳ�ʼ��
	BEEP_Init();
	//��ʼ��8266��������
	ESP8266_Init();
	//��·��������ʼ��
	Encoder_Init_TIM1();
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	Encoder_Init_TIM4();
	//��ʼ��TIM8��PWM
	Motor_Init();
	//��ʼ��TIM6��ʱ��ȡ�������ۼ�ֵ
	TIM6_Int_Init(499,7199);
	//��ʼ��UCOS
	OSInit(&err);		//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);  //����UCOSIII
	while(1);
}

//��ʼ���񣬳�ʼ�������������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	CPU_Init();//��ʼ��CPU״̬
	OS_CRITICAL_ENTER();	//�����ٽ���
	//����ADC����
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
	//���������������
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
								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�����ٽ���
}


volatile float Battery_Voltage = 0;//��ص�ѹ



//10.4 10.84 11.28 11.72 12.16 12.6
//ADC���񣬲������ʣ�����
void ADC_task(void *p_arg){
	u32 Battery_Value = 0;//��ص�ѹADC����ֵ
	OS_ERR err;
	p_arg = p_arg;
	Adc_Init();
	while(1){
		Battery_Value = Get_Adc(ADC_Channel_8);
		Battery_Value = f_kalman_fliter(Battery_Value);//ADC����ֵ��������
		Battery_Voltage = (((double)Battery_Value * 3.3) / 1024)-0.3;//�����ص�ѹ
		Battery_LED(Battery_Voltage);//���µ���ָʾLED
		if(Battery_Voltage <= 10.84){//�жϵ͵���
			BEEP=!BEEP;
			u2_printf("LOG:low_power\r\n");//���LOG
			if(low_power_flag){
				low_power_flag = 0;
				u2_printf("LOG:low_power\r\n");//���LOG
			}
			//OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);		//���������л�
			//BEEP=!BEEP;
		}
		LED1=!LED1;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);		//���������л�
	}
}

extern int M1_UpdatePID_OUT(u16 set,u16 yout);
extern int M2_UpdatePID_OUT(u16 set,u16 yout);
extern int M3_UpdatePID_OUT(u16 set,u16 yout);
extern int M4_UpdatePID_OUT(u16 set,u16 yout);


void refresh_motor_speed(void){
	if(m1_hope_output < 0){
		m1_hope_output = -m1_hope_output;
		//m1��ת
		GPIO_ResetBits(GPIOD,GPIO_Pin_5);
		GPIO_SetBits(GPIOD,GPIO_Pin_6);
	}else{
		//m1��ת
		GPIO_SetBits(GPIOD,GPIO_Pin_5);						 //PB.5 �����
		GPIO_ResetBits(GPIOD,GPIO_Pin_6);						 //PB.5 �����
	}
	if(m2_hope_output < 0){
		//m2��ת
		m2_hope_output = -m2_hope_output;
		GPIO_ResetBits(GPIOD,GPIO_Pin_7);						 //PB.5 �����
		GPIO_SetBits(GPIOD,GPIO_Pin_8);						 //PB.5 �����
	}else{
		//m2��ת
		GPIO_SetBits(GPIOD,GPIO_Pin_7);						 //PB.5 �����
		GPIO_ResetBits(GPIOD,GPIO_Pin_8);						 //PB.5 �����
	}
	if(m3_hope_output < 0){
		m3_hope_output = -m3_hope_output;
		//m3��ת
		GPIO_ResetBits(GPIOD,GPIO_Pin_9);						 //PB.5 �����
		GPIO_SetBits(GPIOD,GPIO_Pin_10);						 //PB.5 �����
	}else{
		//m3��ת
		GPIO_SetBits(GPIOD,GPIO_Pin_9);						 //PB.5 �����
		GPIO_ResetBits(GPIOD,GPIO_Pin_10);						 //PB.5 �����
	}
	if(m4_hope_output < 0){
		m4_hope_output = -m4_hope_output;
		//m4��ת
		GPIO_ResetBits(GPIOD,GPIO_Pin_11);						 //PB.5 �����
		GPIO_SetBits(GPIOD,GPIO_Pin_12);						 //PB.5 �����
	}else{
		//m4��ת
		GPIO_SetBits(GPIOD,GPIO_Pin_11);						 //PB.5 �����
		GPIO_ResetBits(GPIOD,GPIO_Pin_12);						 //PB.5 �����
	}
}

//�����������
void MOTOR_CONTROL_task(void *p_arg){
	OS_ERR err;
	short Axe_X = 0;
	short Axe_Y = 0;
	short Axe_W = 0;
	
	p_arg = p_arg;
	//��·��������ʼ��
	Encoder_Init_TIM1();
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	Encoder_Init_TIM4();
	while(1){
		//������ϣ��������յ���ʼλ��У�����ȷ
		if(usart3_rx_flag && u3_rx_data[0] == 165 && u3_rx_data[1] + u3_rx_data[2] + u3_rx_data[3] == u3_rx_data[4]){
			usart3_rx_flag = 0;//��λ������ɱ�־λ
			Axe_X = (short)u3_rx_data[1] - 63;
			Axe_Y = (short)u3_rx_data[2] - 63;
			Axe_W = (short)u3_rx_data[3] - 63;
			//����תģʽ
			if(Axe_W == 0){
				//�������յ�����ʼλ,��У�����ȷ
				//X��Y�����ĵ�Ϊ63����Χ��0-126
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
		//ȡ�����������ľ���ֵ
		if(Encoder_M1 < 0)	Encoder_M1 = -Encoder_M1;
		if(Encoder_M2 < 0)	Encoder_M2 = -Encoder_M2;
		if(Encoder_M3 < 0)	Encoder_M3 = -Encoder_M3;
		if(Encoder_M4 < 0)	Encoder_M4 = -Encoder_M4;
		//�������������ֵ
		m1_output = M1_UpdatePID_OUT(m1_hope_output,Encoder_M1);
		m2_output = M2_UpdatePID_OUT(m2_hope_output,Encoder_M2);
		m3_output = M3_UpdatePID_OUT(m3_hope_output,Encoder_M3);
		m4_output = M4_UpdatePID_OUT(m4_hope_output,Encoder_M4);
		
		//����CCR
		TIM_SetCompare1(TIM8,m1_output);
		TIM_SetCompare2(TIM8,m2_output);
		TIM_SetCompare3(TIM8,m3_output);
		TIM_SetCompare4(TIM8,m4_output);
		
		//u2_printf("M1:%dM2:%dM3:%dM4:%d\r\n",Encoder_M1,Encoder_M2,Encoder_M3,Encoder_M4);
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);		//���������л�
	}
}
