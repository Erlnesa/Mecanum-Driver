#include "delay.h"
#include "usart2.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 


//串口接收缓存区 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 			//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u8 u2_rx_data[12];//下位机数据
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u8 usart2_rx_flag;
u8 u2_rx_num = 0;
//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
vu16 USART2_RX_STA=0;   	


void USART2_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res =USART_ReceiveData(USART2);		 
		u2_rx_data[u2_rx_num] = res;
		u2_rx_num++;
		if(u2_rx_num == 12){
			u2_rx_num = 0;
			usart2_rx_flag = 1;
		}
		//USART3_timeout_going = 1;
	}
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}


//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void usart2_init(u32 bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //串口3时钟使能

 	USART_DeInit(USART2);  //复位串口3
	
	// 初始化 串口对应IO口  TX-PA2  RX-PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(USART2, &USART_InitStructure); //初始化串口	3
  
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//0

	USART_Cmd(USART2, ENABLE);                    //使能串口 
	
	//使能接收中断
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断   
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	delay_ms(10);
	USART2_RX_STA=0;		//清零
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//0
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
		USART_SendData(USART2,USART2_TX_BUF[j]); 
	} 
}


void rs485_2_send(char data_need_send){
	//RS485_2_TX = 1;
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delay_ms(5);
	USART_SendData(USART2,data_need_send);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//0
	delay_ms(5);
}





