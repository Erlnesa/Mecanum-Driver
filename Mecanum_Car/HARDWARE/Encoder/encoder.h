#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>	 


#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。
void Encoder_Init_TIM1(void);
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
void TIM6_Int_Init(u16 arr,u16 psc);
void TIM6_IRQHandler(void);   //TIM6中断
int Read_Encoder(u8 TIMX);
void TIM1_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
#endif
