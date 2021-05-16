#include "portid.h"


PID_IncrementType MotorControlType;//别名

void PID_init(void)
{
	MotorControlType.SetSpeed=0.0;//设定速度
	MotorControlType.ActualSpeed=0.0;//实际速度
	MotorControlType.err=0.0;//偏差
	MotorControlType.err_last=0.0;
	MotorControlType.err_next=0.0;
	MotorControlType.Kp=0.2;//比例常数
	MotorControlType.Ki=0.015;//积分时间常数
	MotorControlType.Kd=0.2;//微分时间常数
}

//位置式PID算法
float PID_realize_w(float wspeed)
{
	float incrementSpeed;
	
	MotorControlType.SetSpeed=wspeed;//设定值，等于是传参的PWM  即是电机转速预定值
	
	MotorControlType.err=MotorControlType.SetSpeed-MotorControlType.ActualSpeed;//误差，作为PID的输入
	
	incrementSpeed = MotorControlType.Kp*(MotorControlType.err-MotorControlType.err_next)\
									+MotorControlType.Ki*MotorControlType.err+MotorControlType.Kd*(MotorControlType.err-\
									2*MotorControlType.err_next+MotorControlType.err_last);
	
	MotorControlType.ActualSpeed+=incrementSpeed;//保存实际速度
	
	MotorControlType.err_last=MotorControlType.err_next;//将上一个偏差作为最上前的偏差值
	
	MotorControlType.err_next=MotorControlType.err;//将本次误差作为上一个偏差值

	return MotorControlType.ActualSpeed;
}

////增量式PID算法
float PID_realize_z(float zspeed)
{
	float incrementSpeed;
	MotorControlType.SetSpeed=zspeed;
	MotorControlType.err=MotorControlType.SetSpeed-MotorControlType.ActualSpeed;
	incrementSpeed=MotorControlType.Kp*(MotorControlType.err-MotorControlType.err_next)\
								+MotorControlType.Ki*MotorControlType.err+MotorControlType.Kd*\
								(MotorControlType.err-2*MotorControlType.err_next+MotorControlType.err_last);
	MotorControlType.ActualSpeed+=incrementSpeed;
	MotorControlType.err_last=MotorControlType.err_next;
	MotorControlType.err_next=MotorControlType.err;
	return MotorControlType.ActualSpeed;
}

//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
//void Xianfu_Pwm(void)
//{	
//	  int Amplitude=7100;    //===PWM满幅是7200 限制在7100
//    if(Moto1<-Amplitude) Moto1=-Amplitude;	
//		if(Moto1>Amplitude)  Moto1=Amplitude;	
//}

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
int Position_PID (int Encoder,int Target)
{ 	
	 float Position_KP=80,Position_KI=0.1,Position_KD=500;
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}

/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI (int Encoder,int Target)
{ 	
   float Kp=20,Ki=30;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}

