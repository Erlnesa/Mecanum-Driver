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
0	测量电压和存储值
1	测量温度和存储值
2	测量阻抗和存储值		2
3	从存储值传输电压
4	从存储值传输温度
5	从存储值传输阻抗		2
6	测量和传输电压（存储）
7	测量和传输温度（存储）
8	测量和传输阻抗（存储）	2
9	改变地址/ID
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
	USART_SendData(USART1, ID);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART1, command[0]);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	crc=ID^command[0];
	USART_SendData(USART1, crc);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
}
void command1(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	//flag_com=1;
	USART_SendData(USART1, ID);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART1, command[1]);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	crc=ID^command[1];
	USART_SendData(USART1, crc);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
}
void command2(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	//flag_com=2;
	USART_SendData(USART1, ID);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART1, command[2]);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	crc=ID^command[2];
	USART_SendData(USART1, crc);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
}
void command3(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=3;
	USART_SendData(USART1, ID);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART1, command[3]);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	crc=ID^command[3];
	USART_SendData(USART1, crc);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
}
void command4(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=4;
	USART_SendData(USART1, ID);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART1, command[4]);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	crc=ID^command[4];
	USART_SendData(USART1, crc);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
}
void command5(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=5;
	USART_SendData(USART1, ID);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART1, command[5]);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	crc=ID^command[5];
	USART_SendData(USART1, crc);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
}
void command6(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=6;
	USART_SendData(USART1, ID);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART1, command[6]);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	crc=ID^command[6];
	USART_SendData(USART1, crc);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
}
void command7(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=7;
	USART_SendData(USART1, ID);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART1, command[7]);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	crc=ID^command[7];
	USART_SendData(USART1, crc);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
}
void command8(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=8;
	USART_SendData(USART1, ID);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART1, command[8]);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	crc=ID^command[8];
	USART_SendData(USART1, crc);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
}
void command9(u8 ID)
{
//	FlagType   Flag;
	u8 crc=0;
 	flag_com=9;
	USART_SendData(USART1, ID);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART1, command[9]);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	crc=ID^command[9];
	USART_SendData(USART1, crc);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
}

