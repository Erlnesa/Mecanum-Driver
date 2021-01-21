#include "send.h"
#include "usart.h"
#include "timer.h"
#include "delay.h"
#include "rtc.h"

extern u8 flag_z;
u8 flag_com=10;
u8 flag_zs=0;
u8 m=0;
/*
0	������ѹ�ʹ洢ֵ
1	�����¶Ⱥʹ洢ֵ
2	�����迹�ʹ洢ֵ		2
3	�Ӵ洢ֵ�����ѹ
4	�Ӵ洢ֵ�����¶�
5	�Ӵ洢ֵ�����迹		2
6	�����ʹ����ѹ���洢��
7	�����ʹ����¶ȣ��洢��
8	�����ʹ����迹���洢��	2
9	�ı��ַ/ID
*/
u16 command[10]={0x40,0x41,0x42,0x20,0x21,0x22,0x60,0x61,0x62,/*0xa0*/};
//======================================================================
void command_n(u8 num,u8 ID)
{
	switch(num)
	{
		case 0:command0(ID);break;
		case 1:command1(ID);break;
		case 2:if(flag_z){command2(ID);m++;if(m==2){flag_z=0;m=0;}nn=0;}break;
		case 3:command3(ID);break;
		case 4:command4(ID);break;
		//case 5:if(flag_z){command5(ID);m++;if(m==3){flag_z=0;m=0;}nn=0;}break;
		case 5:command5(ID);break;
		case 6:command6(ID);break;
		case 7:command7(ID);break;
		case 8:if(flag_z){command8(ID);m++;if(m==2){flag_z=0;m=0;}nn=0;}break;
		case 9:command9(ID);break;
		default:break;
	}
}
//======================================================================
void command0(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	//flag_com=0;
	USART_SendData(USART1, ID);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART1, command[0]);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	crc=ID^command[0];
	USART_SendData(USART1, crc);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}
void command1(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	//flag_com=1;
	USART_SendData(USART1, ID);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART1, command[1]);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	crc=ID^command[1];
	USART_SendData(USART1, crc);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}
void command2(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	//flag_com=2;
	USART_SendData(USART1, ID);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART1, command[2]);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	crc=ID^command[2];
	USART_SendData(USART1, crc);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}
void command3(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=3;
	USART_SendData(USART1, ID);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART1, command[3]);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	crc=ID^command[3];
	USART_SendData(USART1, crc);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}
void command4(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=4;
	USART_SendData(USART1, ID);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART1, command[4]);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	crc=ID^command[4];
	USART_SendData(USART1, crc);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}
void command5(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=5;
	USART_SendData(USART1, ID);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART1, command[5]);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	crc=ID^command[5];
	USART_SendData(USART1, crc);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}
void command6(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=6;
	USART_SendData(USART1, ID);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART1, command[6]);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	crc=ID^command[6];
	USART_SendData(USART1, crc);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}
void command7(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=7;
	USART_SendData(USART1, ID);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART1, command[7]);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	crc=ID^command[7];
	USART_SendData(USART1, crc);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}
void command8(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=8;
	USART_SendData(USART1, ID);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART1, command[8]);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	crc=ID^command[8];
	USART_SendData(USART1, crc);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}
void command9(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=9;
	USART_SendData(USART1, ID);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART1, command[9]);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	crc=ID^command[9];
	USART_SendData(USART1, crc);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}

