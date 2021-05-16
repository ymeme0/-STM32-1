#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "24cxx.h"
#include "key.h"  
#include "pca9685.h"
#include "car.h"

int flags=0;
/*************************************************************************************
**צ��Ϊ���1��		��Ӧ������S3
**��һ��Ϊ���2��	��Ӧ������S0
**����һ��Ϊ���3����������ӦS1
**����һ��Ϊ���4����������ӦS2
**����Ϊ���5��		��Ӧ������S4

**����Ҫ����ת��60�ȣ���ô�㣬
**60�ȶ�Ӧ������=0.5ms+(60/180)*(2.5ms-0.5ms)=1.1666ms
**����ռ�ձ�=1.1666ms/20ms=off/4096,off=239,50hz��Ӧ����20ms
**PCA9685_setpwm(num,0,239);;;;��ȻҲ��������SERVO000��SERVO180����
*************************************************************************************/
int Set_pwm(int num)
{
	 float angle = 0;
		angle = (num/180+num%180+0.25)*409.6;//ת����Ĺ�ʽ
	return (int)angle;
}


/********ǰ��********/
void Car_back(int CarSpeed)
{
		PCA9685_setpwm(9,0,CarSpeed);
		PCA9685_setpwm(10,0,CarSpeed);
		delay_ms(20); 
}

/********����********/
void Car_go(int CarSpeed)
{
		PCA9685_setpwm(8,0,CarSpeed);
		PCA9685_setpwm(11,0,CarSpeed);
		delay_ms(20);
}

/**********��ת*********/
void Car_left(int CarSpeed)
{
  	PCA9685_setpwm(10,0,CarSpeed);
		PCA9685_setpwm(8,0,CarSpeed);
  	delay_ms(20);

}	

/*******��ת***********/
void Car_right(int CarSpeed)
{
    PCA9685_setpwm(9,0,CarSpeed);
    PCA9685_setpwm(11,0,CarSpeed);	
		delay_ms(20);

}
/******ֹͣ********/
void stop(void)
{
  PCA9685_setpwm(8,0,SERVO000);	
	PCA9685_setpwm(9,0,SERVO000);
	PCA9685_setpwm(10,0,SERVO000);
	PCA9685_setpwm(11,0,SERVO000);
}

/********����Զ�ץȡ**********/
void steer(void)
{
	open();
	delay_ms(500);
	
	up2down();
	delay_ms(500);
	
	close();
	delay_ms(500);
	
	down2up();
	up();
	delay_ms(500);
	
	PCA9685_setpwm(4,0,SERVO120);//5
	close();
	delay_ms(1000);
	
	up2down();
	delay_ms(500);
	
	open();
	delay_ms(500);
	
	down2up();
	up();
	delay_ms(500);
}

/**********��е���ſ�************/
void open(void)
{
  	PCA9685_setpwm(3,0,SERVO050);
}

/*******��е�۹ر�********/
void  close(void)
{
		PCA9685_setpwm(3,0,SERVO130);
}

/********��е���£������ֶ�����ץȡ**********/
void down(void)
{
		PCA9685_setpwm(0,0,SERVO105);//2
		delay_ms(300);//��������
	  PCA9685_setpwm(1,0,SERVO150);//3
		delay_ms(300);//��������
 		PCA9685_setpwm(2,0,SERVO015);//4
		delay_ms(300);//��������
}

/********��е���£������ֶ�����ץȡ**********/
void up2down(void)
{
	PCA9685_setpwm(0,0,SERVO135);//2
	delay_ms(50);
	PCA9685_setpwm(0,0,SERVO130);//2
	delay_ms(50);
	PCA9685_setpwm(0,0,SERVO125);//2
	delay_ms(50);
	PCA9685_setpwm(0,0,SERVO120);//2
	delay_ms(50);
	PCA9685_setpwm(0,0,SERVO115);//2
	delay_ms(50);
	PCA9685_setpwm(0,0,SERVO110);//2
	delay_ms(50);
	PCA9685_setpwm(0,0,SERVO105);//2
	delay_ms(300);//��������
	
	PCA9685_setpwm(1,0,SERVO105);//3
	delay_ms(50);
	PCA9685_setpwm(1,0,SERVO110);//3
	delay_ms(50);
	PCA9685_setpwm(1,0,SERVO115);//3
	delay_ms(50);
	PCA9685_setpwm(1,0,SERVO120);//3
	delay_ms(50);
	PCA9685_setpwm(1,0,SERVO125);//3
	delay_ms(50);
	PCA9685_setpwm(1,0,SERVO130);//3
	delay_ms(50);
	PCA9685_setpwm(1,0,SERVO135);//3
	delay_ms(50);
	PCA9685_setpwm(1,0,SERVO140);//3
	delay_ms(50);
	PCA9685_setpwm(1,0,SERVO145);//3
	delay_ms(50);
	PCA9685_setpwm(1,0,SERVO150);//3
	delay_ms(300);//��������
	
	PCA9685_setpwm(2,0,SERVO060);//4
	delay_ms(50);
	PCA9685_setpwm(2,0,SERVO055);//4
	delay_ms(50);
	PCA9685_setpwm(2,0,SERVO050);//4
	delay_ms(50);
	PCA9685_setpwm(2,0,SERVO045);//4
	delay_ms(50);
	PCA9685_setpwm(2,0,SERVO040);//4
	delay_ms(50);
	PCA9685_setpwm(2,0,SERVO035);//4
	delay_ms(50);
	PCA9685_setpwm(2,0,SERVO030);//4
	delay_ms(50);
	PCA9685_setpwm(2,0,SERVO025);//4
	delay_ms(50);
	PCA9685_setpwm(2,0,SERVO020);//4
	delay_ms(50);
 	//PCA9685_setpwm(2,0,SERVO015);//4
		delay_ms(300);//��������
}

/*********��е����/��λ**********/
void up(void)
{
	  PCA9685_setpwm(0,0,SERVO135);//2
		delay_ms(200);//��������
	  PCA9685_setpwm(1,0,SERVO105);//3
		delay_ms(200);//��������
 		PCA9685_setpwm(2,0,SERVO060);//4
		delay_ms(200);//��������
		PCA9685_setpwm(3,0,SERVO115);//1
		delay_ms(200);//��������
		PCA9685_setpwm(4,0,SERVO090);//5
		delay_ms(200);//��������
}		

/*********��е����**********/
void down2up(void)
{
	PCA9685_setpwm(0,0,SERVO105);//2
	delay_ms(10);
	PCA9685_setpwm(0,0,SERVO115);//2
	delay_ms(10);
	PCA9685_setpwm(0,0,SERVO125);//2
	delay_ms(10);
	PCA9685_setpwm(0,0,SERVO135);//2
	delay_ms(200);//��������
	
	PCA9685_setpwm(1,0,SERVO150);//3
	delay_ms(10);
	PCA9685_setpwm(1,0,SERVO140);//3
	delay_ms(10);
	PCA9685_setpwm(1,0,SERVO130);//3
	delay_ms(10);
	PCA9685_setpwm(1,0,SERVO120);//3
	delay_ms(10);
	PCA9685_setpwm(1,0,SERVO110);//3
	delay_ms(10);
	PCA9685_setpwm(1,0,SERVO105);//3
	delay_ms(200);//��������
 		
	PCA9685_setpwm(2,0,SERVO015);//4
	delay_ms(10);
	PCA9685_setpwm(2,0,SERVO020);//4
	delay_ms(10);
	PCA9685_setpwm(2,0,SERVO025);//4
	delay_ms(10);
	PCA9685_setpwm(2,0,SERVO030);//4
	delay_ms(10);
	PCA9685_setpwm(2,0,SERVO035);//4
	delay_ms(10);
	PCA9685_setpwm(2,0,SERVO040);//4
	delay_ms(10);
	PCA9685_setpwm(2,0,SERVO045);//4
	delay_ms(10);
	PCA9685_setpwm(2,0,SERVO050);//4
	delay_ms(10);
	PCA9685_setpwm(2,0,SERVO055);//4
	delay_ms(10);
	PCA9685_setpwm(2,0,SERVO060);//4
	delay_ms(200);//��������

}		

/********��е�۶��-���£����ڶ�ѵڶ���**********/
void down1(void)
{
		PCA9685_setpwm(0,0,SERVO160);//2
		delay_ms(100);//��������
	  PCA9685_setpwm(1,0,SERVO105);//3
		delay_ms(100);
 		PCA9685_setpwm(2,0,SERVO015);//4
		delay_ms(100);
}

/********��е��ץȡ��ѣ��Զ�ץȡ���2��**********/
void Stacking_pile(void)
{
	open();
	delay_ms(500);
	
	down();
	delay_ms(500);
	
	close();
	delay_ms(500);
	
	up();
	delay_ms(500);
	
	PCA9685_setpwm(4,0,SERVO120);//5
	delay_ms(1000);
	
	if(flags==0)
	{
		down();
		delay_ms(500);
	}
	else if(flags==1)
	{
		down1();
		delay_ms(500);
	}
	
	open();
	delay_ms(500);
	
	up();
	delay_ms(500);
	flags++;
}

/********��е�ۿ���ץȡ**********/
void Grasp(void)
{
	open();
	delay_ms(500);
	
	down();
	delay_ms(500);
	
	close();
	delay_ms(500);
	
	up();
	delay_ms(500);
}
/********��е�ۿ��Ʒ���**********/
void Drop_it(void)
{
	down();
	delay_ms(500);
	
	open();
	delay_ms(500);
	
	up();
	delay_ms(500);
}

/********��������ץȡ**********/
void Song_steer(void)
{
	open();
	delay_ms(500);
	
	down();
	delay_ms(500);
	
	close();
	delay_ms(500);
	
	up();
	delay_ms(500);
}

/********�������Ʒ���**********/
void Song_down(void)
{
	down();
	delay_ms(500);
	
	open();
	delay_ms(500);
	
	up();
	delay_ms(500);
}

