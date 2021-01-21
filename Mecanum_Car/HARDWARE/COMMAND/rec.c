#include "rec.h"
#include "usart.h"
#include "timer.h"
#include "delay.h"
#include "lcd.h"
#include "convert.h"
#include "display.h"
#include "send.h"
#include "beep.h"
#include "curve.h"

extern u8 menu_ID;

//#define vol_err	2.2				//V
#define tem_err	50				//��
#define res_err	10				//m��

//#define vol_warn	vol_err+0.5		//V
//#define tem_warn	tem_err*0.8				//��
//#define res_warn	res_err*0.8				//m��

#define vol_er		195	//vol_err*100
#define tem_er		6000
#define res_er		1400

#define vol_war		215
#define tem_war		5000
#define res_war		1000

long int num;
int inte,decs;

void rec(u8 i)
{
	u16 xm=0,ym=0;
	
	//����i��ֵ������ʾλ��ƫ��
	if(i<4)ym=0;					//��������ӿ�
	switch(i)
	{
		case 0:xm=0;break;
		case 1:xm=199;break;
		case 2:xm=199*2;break;
		case 3:xm=199*3;break;
		default:break;
	}
	
	if(jiema==1)
	{
		switch(flag_com)
		{
			case 0:{
				flag_com=10;
				jiema=0;
				break;
			}
			case 1:{
				flag_com=10;
				jiema=0;
				break;
			}
			case 2:{
				flag_com=10;
				jiema=0;
				break;
			}
			case 3:{
				num=convert(USART1_RX_BUF[1],USART1_RX_BUF[2]);
				//����
				curve_vol[i]=num;
				//������Ԥ������
				if(num>vol_war)
				{
					warn_vol[i]=0;
					err_vol[i]=0;
				}
				else if((num<=vol_war)&&(num>vol_er))
				{
					warn_vol[i]=num;
					err_vol[i]=0;
				}
				else if(num<=vol_er)
				{
					warn_vol[i]=0;
					err_vol[i]=num;
					BEEP=1;
				}
				if(menu_ID==2)
				{
					POINT_COLOR=CYAN;
					LCD_ShowString(x0_2+xm+36+70,y0_2+ym,12,24,24,".");
					LCD_ShowString(x0_2+xm+36+70+12,y0_2+ym,12,24,24,"0");
					inte=num/100;
					decs=num%100;
					LCD_ShowNum(x0_2+xm+36+20,y0_2+ym,inte,sizeof(inte),24);
					if(decs<10)LCD_ShowNum(x0_2+xm+36+70+24,y0_2+ym,decs,1,24);
					else LCD_ShowNum(x0_2+xm+36+70+12,y0_2+ym,decs,2,24);
				}
				flag_com=10;
				jiema=0;
				break;
			}
			case 4:{
				num=convert_t(USART1_RX_BUF[1],USART1_RX_BUF[2]);
				//����
				curve_tem[i]=num;
				//������Ԥ������
				if(num<tem_war)
				{
					warn_tem[i]=0;
					err_tem[i]=0;
				}
				else if((num>=tem_war)&&(num<tem_er))
				{
					warn_tem[i]=num;
					err_tem[i]=0;
				}
				else if(num>=tem_er)
				{
					warn_tem[i]=0;
					err_tem[i]=num;
					BEEP=1;								//��������
				}
				if(menu_ID==2)
				{
					POINT_COLOR=CYAN;
					LCD_ShowString(x0_2+xm+36+70,y0_2+ym+48,12,24,24,".");
					LCD_ShowString(x0_2+xm+36+70+12,y0_2+ym+48,12,24,24,"0");
					inte=num/100;
					decs=num%100;
					LCD_ShowNum(x0_2+xm+36+20,y0_2+ym+48,inte,sizeof(inte),24);
					if(decs<10)LCD_ShowNum(x0_2+xm+36+70+24,y0_2+ym+48,decs,1,24);
					else LCD_ShowNum(x0_2+xm+36+70+12,y0_2+ym+48,decs,2,24);
				}
				flag_com=10;
				jiema=0;
				break;
			}
			case 5:{
				num=convert(USART1_RX_BUF[1],USART1_RX_BUF[2]);
				//�迹δ������ɱ�־λ
				if(num==51100)break;								//����������500������������ϴ���ֵ
				//����
				curve_res[i]=num;
				//������Ԥ������
				if(num<res_war)
				{
					warn_res[i]=0;
					err_res[i]=0;
				}
				else if((num>=res_war)&&(num<res_er))
				{
					err_res[i]=0;
					warn_res[i]=num;
				}
				else if(num>=res_er)
				{
					warn_res[i]=0;
					err_res[i]=num;
					BEEP=1;								//��������
				}
				if(menu_ID==2)
				{
					POINT_COLOR=CYAN;
					LCD_ShowString(x0_2+xm+36+70,y0_2+ym+72,12,24,24,".");
					LCD_ShowString(x0_2+xm+36+70+12,y0_2+ym+72,12,24,24,"0");
					inte=num/100;
					decs=num%100;
					LCD_ShowNum(x0_2+xm+36+20,y0_2+ym+72,inte,sizeof(inte),24);
					if(decs<10)LCD_ShowNum(x0_2+xm+36+70+24,y0_2+ym+72,decs,1,24);
					else LCD_ShowNum(x0_2+xm+36+70+12,y0_2+ym+72,decs,2,24);
				}
				flag_com=10;
				jiema=0;
				break;
			}
			case 6:{
				num=convert(USART1_RX_BUF[1],USART1_RX_BUF[2]);
//				if(num<vol_)BEEP=1;								//��������
				if(menu_ID==2)
				{
					POINT_COLOR=CYAN;
					LCD_ShowString(x0_2+xm+36+70,y0_2+ym,12,24,24,".");
					LCD_ShowString(x0_2+xm+36+70+12,y0_2+ym,12,24,24,"0");
					inte=num/100;
					decs=num%100;
					LCD_ShowNum(x0_2+xm+36+20,y0_2+ym,inte,sizeof(inte),24);
					if(decs<10)LCD_ShowNum(x0_2+xm+36+70+24,y0_2+ym,decs,1,24);
					else LCD_ShowNum(x0_2+xm+36+70+12,y0_2+ym,decs,2,24);
				}
				flag_com=10;
				jiema=0;
				break;
			}
			case 7:{
				num=convert_t(USART1_RX_BUF[1],USART1_RX_BUF[2]);
//				if(num>temp_)BEEP=1;							//��������
				if(menu_ID==2)
				{
					POINT_COLOR=CYAN;
					LCD_ShowString(x0_2+xm+36+70,y0_2+ym+48,12,24,24,".");
					LCD_ShowString(x0_2+xm+36+70+12,y0_2+ym+48,12,24,24,"0");
					inte=num/100;
					decs=num%100;
					LCD_ShowNum(x0_2+xm+36+20,y0_2+ym+48,inte,sizeof(inte),24);
					if(decs<10)LCD_ShowNum(x0_2+xm+36+70+24,y0_2+ym+48,decs,1,24);
					else LCD_ShowNum(x0_2+xm+36+70+12,y0_2+ym+48,decs,2,24);
				}
				flag_com=10;
				jiema=0;
				break;
			}
			case 8:{
				num=convert(USART1_RX_BUF[1],USART1_RX_BUF[2]);
				if(num>50000)break;//����������500������������ϴ���ֵ
//				if(num>impe_)BEEP=1;							//��������
				if(menu_ID==2)
				{
					POINT_COLOR=CYAN;
					LCD_ShowString(x0_2+xm+36+70,y0_2+ym+72,12,24,24,".");
					LCD_ShowString(x0_2+xm+36+70+12,y0_2+ym+72,12,24,24,"0");
					inte=num/100;
					decs=num%100;
					LCD_ShowNum(x0_2+xm+36+20,y0_2+ym+72,inte,sizeof(inte),24);
					if(decs<10)LCD_ShowNum(x0_2+xm+36+70+24,y0_2+ym+72,decs,1,24);
					else LCD_ShowNum(x0_2+xm+36+70+12,y0_2+ym+72,decs,2,24);
				}
				flag_com=10;
				jiema=0;
				break;
			}
			case 9:{
				flag_com=10;
				jiema=0;
				break;
			}
			default:{
				jiema=0;
				flag_com=10;
				break;
			}
		}
	}
}

