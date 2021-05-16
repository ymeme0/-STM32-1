#include "pca9685.h"
#include "myiic.h"
#include "delay.h"
#include "math.h"



/*---------------------------------------------------------------
                向PCA9685里写地址，数据
----------------------------------------------------------------*/
void PCA9685_write(u8 adr,u8 data)
{ 
	 IIC_Start();
	
	 IIC_Send_Byte(PCA9685_adrr); //PCA9685的片选地址
	 IIC_Wait_Ack();
	
	 IIC_Send_Byte(adr);//写地址控制字节
	 IIC_Wait_Ack();
	
	 IIC_Send_Byte(data); //写数据
	 IIC_Wait_Ack();
	
   IIC_Stop();
}

/*---------------------------------------------------------------
            从PCA9685里的地址值中读数据(有返回值)
----------------------------------------------------------------*/
u8 PCA9685_read(u8 adr)
{
  u8 data;
	IIC_Start();
	
	IIC_Send_Byte(PCA9685_adrr);//PCA9685的片选地址
  IIC_Wait_Ack();
	
	IIC_Send_Byte(adr);
	IIC_Wait_Ack();
	
	IIC_Start();
	
  IIC_Send_Byte(PCA9685_adrr|0x01);//地址的第八位控制数据流方向，就是写或读
  IIC_Wait_Ack();
	
	data=IIC_Read_Byte(0);
  IIC_Stop();
	
	return data;

}

/*---------------------------------------------------------------
                        PCA9685复位
----------------------------------------------------------------*/
void PCA9685_reset(void)
{
  PCA9685_write(PCA9685_MODE1,0x00);
}

/*---------------------------------------------------------------
											PCA9685修改频率函数
----------------------------------------------------------------*/
void PCA9685_setPWMFreq(float freq) 
{
		u8 prescale,oldmode,newmode;
		double prescaleval;
	
		//设置角度
		//60度=0.5ms+（60/180）*（2.5ms-0.5ms）=1.1666ms
		//利用占空比=1.1666ms/20ms=off/4096，off=239，50hz对应周期20ms
		freq *= 0.92;//频率校准，一般要乘以0.915,否则会出现误差 
		prescaleval = 25000000;
		prescaleval /= 4096;
		prescaleval /= freq;
		prescaleval -= 1;
		prescale =floor(prescaleval + 0.5f);

		oldmode = PCA9685_read(PCA9685_MODE1);
	
		newmode = (oldmode&0x7F) | 0x10; // sleep
	
		PCA9685_write(PCA9685_MODE1, newmode); // go to sleep
	
		PCA9685_write(PCA9685_PRESCALE, prescale); // 设置预分频器
	
		PCA9685_write(PCA9685_MODE1, oldmode);
		delay_ms(2);
	
		PCA9685_write(PCA9685_MODE1, oldmode | 0xa1); 
}

/*---------------------------------------------------------------
                     PCA9685修改角度函数
num:舵机PWM输出引脚0~15
on:PWM上升计数值0~4096
off:PWM下降计数值0~4096

一个PWM周期分成4096份，由0开始+1计数，计到on时跳变为高电平，继续计数到off时
跳变为低电平，直到计满4096重新开始。
所以当on不等于0时可作延时,当on等于0时，off/4096的值就是PWM的占空比。

//其中开始一般设置为0，只需要取下降时间就行
//该函数为设置PWM波的占空比，一般都以4096为一个PWM波周期
----------------------------------------------------------------*/
void PCA9685_setpwm(u8 num, u32 on, u32 off) 
{
			PCA9685_write(LED0_ON_L+4*num,on);
			PCA9685_write(LED0_ON_H+4*num,on>>8);
			PCA9685_write(LED0_OFF_L+4*num,off);
			PCA9685_write(LED0_OFF_H+4*num,off>>8);
}
/************************************************************************************************/


