#ifndef  __PORTID_H__
#define  __PORTID_H__

#include "stdio.h"

//会报PID重复定义
//struct _pid
//{
//	float SetSpeed; //定义设定值
//	float ActualSpeed; //定义实际值
//	float err; //定义偏差值
//	float err_next; //定义上一个偏差值
//	float err_last; //定义最上前的偏差值
//	float Kp,Ki,Kd; //定义比例、积分、微分系数
//}pid;


typedef struct
{
	float SetSpeed; //定义设定值
	float ActualSpeed; //定义实际值
	float err; //定义偏差值
	float err_next; //定义上一个偏差值
	float err_last; //定义最上前的偏差值
	float Kp,Ki,Kd; //定义比例、积分、微分系数
}PID_IncrementType;


void PID_init(void);
float PID_realize_w(float wspeed);
float PID_realize_z(float zspeed);
int myabs(int a);
void Xianfu_Pwm(void);


int Position_PID (int Encoder,int Target);
#endif


