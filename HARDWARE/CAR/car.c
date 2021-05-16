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
**爪子为舵机1，		对应驱动板S3
**下一个为舵机2，	对应驱动板S0
**再下一个为舵机3，对驱动板应S1
**再下一个为舵机4，对驱动板应S2
**底盘为舵机5，		对应驱动板S4

**例如要求舵机转到60度，这么算，
**60度对应的脉宽=0.5ms+(60/180)*(2.5ms-0.5ms)=1.1666ms
**利用占空比=1.1666ms/20ms=off/4096,off=239,50hz对应周期20ms
**PCA9685_setpwm(num,0,239);;;;当然也可以利用SERVO000和SERVO180计算
*************************************************************************************/
int Set_pwm(int num)
{
	 float angle = 0;
		angle = (num/180+num%180+0.25)*409.6;//转换后的公式
	return (int)angle;
}


/********前进********/
void Car_back(int CarSpeed)
{
		PCA9685_setpwm(9,0,CarSpeed);
		PCA9685_setpwm(10,0,CarSpeed);
		delay_ms(20); 
}

/********后退********/
void Car_go(int CarSpeed)
{
		PCA9685_setpwm(8,0,CarSpeed);
		PCA9685_setpwm(11,0,CarSpeed);
		delay_ms(20);
}

/**********左转*********/
void Car_left(int CarSpeed)
{
  	PCA9685_setpwm(10,0,CarSpeed);
		PCA9685_setpwm(8,0,CarSpeed);
  	delay_ms(20);

}	

/*******右转***********/
void Car_right(int CarSpeed)
{
    PCA9685_setpwm(9,0,CarSpeed);
    PCA9685_setpwm(11,0,CarSpeed);	
		delay_ms(20);

}
/******停止********/
void stop(void)
{
  PCA9685_setpwm(8,0,SERVO000);	
	PCA9685_setpwm(9,0,SERVO000);
	PCA9685_setpwm(10,0,SERVO000);
	PCA9685_setpwm(11,0,SERVO000);
}

/********舵机自动抓取**********/
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

/**********机械臂张开************/
void open(void)
{
  	PCA9685_setpwm(3,0,SERVO050);
}

/*******机械臂关闭********/
void  close(void)
{
		PCA9685_setpwm(3,0,SERVO130);
}

/********机械臂下，用于手动控制抓取**********/
void down(void)
{
		PCA9685_setpwm(0,0,SERVO105);//2
		delay_ms(300);//减缓惯性
	  PCA9685_setpwm(1,0,SERVO150);//3
		delay_ms(300);//减缓惯性
 		PCA9685_setpwm(2,0,SERVO015);//4
		delay_ms(300);//减缓惯性
}

/********机械臂下，用于手动控制抓取**********/
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
	delay_ms(300);//减缓惯性
	
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
	delay_ms(300);//减缓惯性
	
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
		delay_ms(300);//减缓惯性
}

/*********机械臂上/复位**********/
void up(void)
{
	  PCA9685_setpwm(0,0,SERVO135);//2
		delay_ms(200);//减缓惯性
	  PCA9685_setpwm(1,0,SERVO105);//3
		delay_ms(200);//减缓惯性
 		PCA9685_setpwm(2,0,SERVO060);//4
		delay_ms(200);//减缓惯性
		PCA9685_setpwm(3,0,SERVO115);//1
		delay_ms(200);//减缓惯性
		PCA9685_setpwm(4,0,SERVO090);//5
		delay_ms(200);//减缓惯性
}		

/*********机械臂上**********/
void down2up(void)
{
	PCA9685_setpwm(0,0,SERVO105);//2
	delay_ms(10);
	PCA9685_setpwm(0,0,SERVO115);//2
	delay_ms(10);
	PCA9685_setpwm(0,0,SERVO125);//2
	delay_ms(10);
	PCA9685_setpwm(0,0,SERVO135);//2
	delay_ms(200);//减缓惯性
	
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
	delay_ms(200);//减缓惯性
 		
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
	delay_ms(200);//减缓惯性

}		

/********机械臂躲堆-放下，用于垛堆第二个**********/
void down1(void)
{
		PCA9685_setpwm(0,0,SERVO160);//2
		delay_ms(100);//减缓惯性
	  PCA9685_setpwm(1,0,SERVO105);//3
		delay_ms(100);
 		PCA9685_setpwm(2,0,SERVO015);//4
		delay_ms(100);
}

/********机械臂抓取躲堆，自动抓取垛堆2个**********/
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

/********机械臂控制抓取**********/
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
/********机械臂控制放下**********/
void Drop_it(void)
{
	down();
	delay_ms(500);
	
	open();
	delay_ms(500);
	
	up();
	delay_ms(500);
}

/********语音控制抓取**********/
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

/********语音控制放下**********/
void Song_down(void)
{
	down();
	delay_ms(500);
	
	open();
	delay_ms(500);
	
	up();
	delay_ms(500);
}

