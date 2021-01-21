#include "ESP8266.h"
#include "sys.h"
#include "delay.h"
//初始化ESP8266
void ESP8266_Init(){
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PE端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //ESP8266_RESET-->PE6 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOE6
	GPIO_SetBits(GPIOE,GPIO_Pin_6);						 //PE6 输出高
	
	ESP8266_Reset();
}

//复位ESP8266
void ESP8266_Reset(){
	GPIO_ResetBits(GPIOE,GPIO_Pin_6);
	delay_ms(500);
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
}




