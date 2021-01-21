#include "motor.h"
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM8_PWM_Init(u16 arr,u16 psc)
{  
        GPIO_InitTypeDef GPIO_InitStructure;
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        TIM_OCInitTypeDef  TIM_OCInitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);// 
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  //ʹ��GPIO����ʱ��ʹ��       

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM8_CH1-4
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        
        TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ         80K
        TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
        TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
        TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
        TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
        TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
				TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��        
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
        TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
        TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
				TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��        
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
        TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
        TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
        TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH3Ԥװ��ʹ��        
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
        TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
        TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
        TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��        
        
        TIM_ARRPreloadConfig(TIM8, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���

        TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM1
        TIM_CtrlPWMOutputs(TIM8,ENABLE);        //MOE �����ʹ��        
}


void Motor_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ���߼��������
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);  //ʹ��GPIO����ʱ��ʹ��       
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_5);						 //PB.5 �����
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);						 //PB.5 �����
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);						 //PB.5 �����
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);						 //PB.5 �����
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_9);						 //PB.5 �����
	GPIO_ResetBits(GPIOD,GPIO_Pin_10);						 //PB.5 �����
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);						 //PB.5 �����
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);						 //PB.5 �����
	
	//��ʼ��PWM����
	TIM8_PWM_Init(360,0);//����Ƶ��PWMƵ��=72000000/900=80Khz
	TIM_SetCompare1(TIM8,0);
	TIM_SetCompare2(TIM8,0);
	TIM_SetCompare3(TIM8,0);
	TIM_SetCompare4(TIM8,0);
	
	
}

