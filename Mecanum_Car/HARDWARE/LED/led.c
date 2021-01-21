#include "led.h"

    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOF, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_ResetBits(GPIOF,GPIO_Pin_3);						 //PB.5 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOF,GPIO_Pin_4); 						 //PE.5 ����� 
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOG,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5); 						 //PE.5 ����� 
	

}

//��ʾ���ʣ�����
void Battery_LED(float Battery_Voltage){
	if(Battery_Voltage >= 11.72){
			if(Battery_Voltage >= 12.16){
				//��ȫ��
				GPIO_SetBits(GPIOG,GPIO_Pin_1);
				GPIO_SetBits(GPIOG,GPIO_Pin_2);
				GPIO_SetBits(GPIOG,GPIO_Pin_3);
				GPIO_SetBits(GPIOG,GPIO_Pin_4);
				GPIO_SetBits(GPIOG,GPIO_Pin_5);
			}else{
				//���ĸ�
				GPIO_SetBits(GPIOG,GPIO_Pin_1);
				GPIO_SetBits(GPIOG,GPIO_Pin_2);
				GPIO_SetBits(GPIOG,GPIO_Pin_3);
				GPIO_SetBits(GPIOG,GPIO_Pin_4);
				GPIO_ResetBits(GPIOG,GPIO_Pin_5);
			}
		}else if(Battery_Voltage < 11.72){
			if(Battery_Voltage <= 10.84){
				//��һ��
				GPIO_SetBits(GPIOG,GPIO_Pin_1);
				GPIO_ResetBits(GPIOG,GPIO_Pin_2);
				GPIO_ResetBits(GPIOG,GPIO_Pin_3);
				GPIO_ResetBits(GPIOG,GPIO_Pin_4);
				GPIO_ResetBits(GPIOG,GPIO_Pin_5);
			}else if(Battery_Voltage <= 11.28){
				//������
				GPIO_SetBits(GPIOG,GPIO_Pin_1);
				GPIO_SetBits(GPIOG,GPIO_Pin_2);
				GPIO_ResetBits(GPIOG,GPIO_Pin_3);
				GPIO_ResetBits(GPIOG,GPIO_Pin_4);
				GPIO_ResetBits(GPIOG,GPIO_Pin_5);
			}else{
				//������
				GPIO_SetBits(GPIOG,GPIO_Pin_1);
				GPIO_SetBits(GPIOG,GPIO_Pin_2);
				GPIO_SetBits(GPIOG,GPIO_Pin_3);
				GPIO_ResetBits(GPIOG,GPIO_Pin_4);
				GPIO_ResetBits(GPIOG,GPIO_Pin_5);
			}
		}
}

 
