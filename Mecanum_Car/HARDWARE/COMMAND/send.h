#ifndef __SEND_H
#define __SEND_H 			   
#include "sys.h" 

extern u8 flag_com;

//typedef struct 
//{
//	unsigned char flag_0				: 1;
//	unsigned char flag_1				: 1;
//	unsigned char flag_2				: 1;
//	unsigned char flag_3				: 1;
//	unsigned char flag_4				: 1;
//	unsigned char flag_5				: 1;
//	unsigned char flag_6				: 1;
//	unsigned char flag_7				: 1;
//	unsigned char flag_8				: 1;
//	unsigned char flag_9				: 1;
//	
//}	FlagType;

void command_n(u8 num,u8 ID);

void command0(u8 ID);
void command1(u8 ID);
void command2(u8 ID);
void command3(u8 ID);
void command4(u8 ID);
void command5(u8 ID);
void command6(u8 ID);
void command7(u8 ID);
void command8(u8 ID);
void command9(u8 ID);

#endif
