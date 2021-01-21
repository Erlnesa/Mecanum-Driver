#include "motor.h"
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM8_PWM_Init(u16 arr,u16 psc)
{  
        GPIO_InitTypeDef GPIO_InitStructure;
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        TIM_OCInitTypeDef  TIM_OCInitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);// 
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  //使能GPIO外设时钟使能       

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM8_CH1-4
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        
        TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值         80K
        TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
        TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
        TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
        TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
        TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
				TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1预装载使能        
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
        TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
        TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
				TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH2预装载使能        
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
        TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
        TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
        TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH3预装载使能        
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
        TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
        TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
        TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH4预装载使能        
        
        TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器

        TIM_Cmd(TIM8, ENABLE);  //使能TIM1
        TIM_CtrlPWMOutputs(TIM8,ENABLE);        //MOE 主输出使能        
}


void Motor_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化逻辑输出引脚
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);  //使能GPIO外设时钟使能       
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_5);						 //PB.5 输出高
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);						 //PB.5 输出高
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);						 //PB.5 输出高
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);						 //PB.5 输出高
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_9);						 //PB.5 输出高
	GPIO_ResetBits(GPIOD,GPIO_Pin_10);						 //PB.5 输出高
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);						 //PB.5 输出高
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);						 //PB.5 输出高
	
	//初始化PWM控制
	TIM8_PWM_Init(360,0);//不分频。PWM频率=72000000/900=80Khz
	TIM_SetCompare1(TIM8,0);
	TIM_SetCompare2(TIM8,0);
	TIM_SetCompare3(TIM8,0);
	TIM_SetCompare4(TIM8,0);
	
	
}

