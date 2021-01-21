#include "led.h"

    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOF, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_ResetBits(GPIOF,GPIO_Pin_3);						 //PB.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_ResetBits(GPIOF,GPIO_Pin_4); 						 //PE.5 输出高 
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_ResetBits(GPIOG,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5); 						 //PE.5 输出高 
	

}

//显示电池剩余电量
void Battery_LED(float Battery_Voltage){
	if(Battery_Voltage >= 11.72){
			if(Battery_Voltage >= 12.16){
				//灯全亮
				GPIO_SetBits(GPIOG,GPIO_Pin_1);
				GPIO_SetBits(GPIOG,GPIO_Pin_2);
				GPIO_SetBits(GPIOG,GPIO_Pin_3);
				GPIO_SetBits(GPIOG,GPIO_Pin_4);
				GPIO_SetBits(GPIOG,GPIO_Pin_5);
			}else{
				//亮四个
				GPIO_SetBits(GPIOG,GPIO_Pin_1);
				GPIO_SetBits(GPIOG,GPIO_Pin_2);
				GPIO_SetBits(GPIOG,GPIO_Pin_3);
				GPIO_SetBits(GPIOG,GPIO_Pin_4);
				GPIO_ResetBits(GPIOG,GPIO_Pin_5);
			}
		}else if(Battery_Voltage < 11.72){
			if(Battery_Voltage <= 10.84){
				//亮一个
				GPIO_SetBits(GPIOG,GPIO_Pin_1);
				GPIO_ResetBits(GPIOG,GPIO_Pin_2);
				GPIO_ResetBits(GPIOG,GPIO_Pin_3);
				GPIO_ResetBits(GPIOG,GPIO_Pin_4);
				GPIO_ResetBits(GPIOG,GPIO_Pin_5);
			}else if(Battery_Voltage <= 11.28){
				//亮两个
				GPIO_SetBits(GPIOG,GPIO_Pin_1);
				GPIO_SetBits(GPIOG,GPIO_Pin_2);
				GPIO_ResetBits(GPIOG,GPIO_Pin_3);
				GPIO_ResetBits(GPIOG,GPIO_Pin_4);
				GPIO_ResetBits(GPIOG,GPIO_Pin_5);
			}else{
				//亮三个
				GPIO_SetBits(GPIOG,GPIO_Pin_1);
				GPIO_SetBits(GPIOG,GPIO_Pin_2);
				GPIO_SetBits(GPIOG,GPIO_Pin_3);
				GPIO_ResetBits(GPIOG,GPIO_Pin_4);
				GPIO_ResetBits(GPIOG,GPIO_Pin_5);
			}
		}
}

 
