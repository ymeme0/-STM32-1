#include "portid.h"


PID_IncrementType MotorControlType;//����

void PID_init(void)
{
	MotorControlType.SetSpeed=0.0;//�趨�ٶ�
	MotorControlType.ActualSpeed=0.0;//ʵ���ٶ�
	MotorControlType.err=0.0;//ƫ��
	MotorControlType.err_last=0.0;
	MotorControlType.err_next=0.0;
	MotorControlType.Kp=0.2;//��������
	MotorControlType.Ki=0.015;//����ʱ�䳣��
	MotorControlType.Kd=0.2;//΢��ʱ�䳣��
}

//λ��ʽPID�㷨
float PID_realize_w(float wspeed)
{
	float incrementSpeed;
	
	MotorControlType.SetSpeed=wspeed;//�趨ֵ�������Ǵ��ε�PWM  ���ǵ��ת��Ԥ��ֵ
	
	MotorControlType.err=MotorControlType.SetSpeed-MotorControlType.ActualSpeed;//����ΪPID������
	
	incrementSpeed = MotorControlType.Kp*(MotorControlType.err-MotorControlType.err_next)\
									+MotorControlType.Ki*MotorControlType.err+MotorControlType.Kd*(MotorControlType.err-\
									2*MotorControlType.err_next+MotorControlType.err_last);
	
	MotorControlType.ActualSpeed+=incrementSpeed;//����ʵ���ٶ�
	
	MotorControlType.err_last=MotorControlType.err_next;//����һ��ƫ����Ϊ����ǰ��ƫ��ֵ
	
	MotorControlType.err_next=MotorControlType.err;//�����������Ϊ��һ��ƫ��ֵ

	return MotorControlType.ActualSpeed;
}

////����ʽPID�㷨
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
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
//void Xianfu_Pwm(void)
//{	
//	  int Amplitude=7100;    //===PWM������7200 ������7100
//    if(Moto1<-Amplitude) Moto1=-Amplitude;	
//		if(Moto1>Amplitude)  Moto1=Amplitude;	
//}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
int Position_PID (int Encoder,int Target)
{ 	
	 float Position_KP=80,Position_KI=0.1,Position_KD=500;
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}

/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI (int Encoder,int Target)
{ 	
   float Kp=20,Ki=30;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}

