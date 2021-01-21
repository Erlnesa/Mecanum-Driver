#ifndef __USART3_H
#define __USART3_H	 
#include "sys.h"  
#include "stdio.h"	

#define USART3_MAX_RECV_LEN		600					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		600					//最大发送缓存字节数
#define USART3_RX_EN 			1					//0,不接收;1,接收.

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern vu16 USART3_RX_STA;   						//接收数据状态

void usart3_init(u32 bound);				//串口3初始化 
void USART3_IRQHandler(void);                	//串口1中断服务函数
void u3_printf(char* fmt,...);

#endif













