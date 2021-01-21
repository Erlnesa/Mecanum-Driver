#include "pid.h"

#define Pid_Limit_Top 360

//PID变量
float Kp = 30.2;
float Ki = 7.0;
float Td = 0.45;

int M1_PID_ERR[3] = {0x00,0x00,0x00};
int M1_Pid_Out = 0x00;

//速度PI控制
int M1_UpdatePID_OUT(u16 set,u16 yout){
	//u16 out;
	M1_PID_ERR[2] = set - yout;
	
	M1_Pid_Out = Kp * (M1_PID_ERR[2] - M1_PID_ERR[1]);
	M1_Pid_Out += Ki * M1_PID_ERR[2] * Td;
	
	M1_PID_ERR[0] = M1_PID_ERR[1];
	M1_PID_ERR[1] = M1_PID_ERR[2];
	if(M1_Pid_Out > Pid_Limit_Top){
		return Pid_Limit_Top;
	}else if(M1_Pid_Out < 0){
		return 0;
	}else{
		return M1_Pid_Out;
	}
}

int M2_PID_ERR[3] = {0x00,0x00,0x00};
int M2_Pid_Out = 0x00;

//速度PI控制
int M2_UpdatePID_OUT(u16 set,u16 yout){
	//u16 out;
	M2_PID_ERR[2] = set - yout;
	
	M2_Pid_Out = Kp * (M2_PID_ERR[2] - M2_PID_ERR[1]);
	M2_Pid_Out += Ki * M2_PID_ERR[2] * Td;
	
	M2_PID_ERR[0] = M2_PID_ERR[1];
	M2_PID_ERR[1] = M2_PID_ERR[2];
	if(M2_Pid_Out > Pid_Limit_Top){
		return Pid_Limit_Top;
	}else if(M2_Pid_Out < 0){
		return 0;
	}else{
		return M2_Pid_Out;
	}
}

int M3_PID_ERR[3] = {0x00,0x00,0x00};
int M3_Pid_Out = 0x00;

//速度PI控制
int M3_UpdatePID_OUT(u16 set,u16 yout){
	//u16 out;
	M3_PID_ERR[2] = set - yout;
	
	M3_Pid_Out = Kp * (M3_PID_ERR[2] - M3_PID_ERR[1]);
	M3_Pid_Out += Ki * M3_PID_ERR[2] * Td;
	
	M3_PID_ERR[0] = M3_PID_ERR[1];
	M3_PID_ERR[1] = M3_PID_ERR[2];
	if(M3_Pid_Out > Pid_Limit_Top){
		return Pid_Limit_Top;
	}else if(M3_Pid_Out < 0){
		return 0;
	}else{
		return M3_Pid_Out;
	}
}

int M4_PID_ERR[3] = {0x00,0x00,0x00};
int M4_Pid_Out = 0x00;

//速度PI控制
int M4_UpdatePID_OUT(u16 set,u16 yout){
	//u16 out;
	M4_PID_ERR[2] = set - yout;
	
	M4_Pid_Out = Kp * (M4_PID_ERR[2] - M4_PID_ERR[1]);
	M4_Pid_Out += Ki * M4_PID_ERR[2] * Td;
	
	M4_PID_ERR[0] = M4_PID_ERR[1];
	M4_PID_ERR[1] = M4_PID_ERR[2];
	if(M4_Pid_Out > Pid_Limit_Top){
		return Pid_Limit_Top;
	}else if(M4_Pid_Out < 0){
		return 0;
	}else{
		return M4_Pid_Out;
	}
}

