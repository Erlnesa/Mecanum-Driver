#include "ESP8266.h"
#include "sys.h"
#include "delay.h"
//��ʼ��ESP8266
void ESP8266_Init(){
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //ESP8266_RESET-->PE6 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOE6
	GPIO_SetBits(GPIOE,GPIO_Pin_6);						 //PE6 �����
	
	ESP8266_Reset();
}

//��λESP8266
void ESP8266_Reset(){
	GPIO_ResetBits(GPIOE,GPIO_Pin_6);
	delay_ms(500);
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
}




