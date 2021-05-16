#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "24cxx.h"
#include "key.h"  
#include "pca9685.h"
#include "car.h"
#include "stdlib.h"
#include "string.h"
#include "portid.h"

int CarSpeedControl = 2000;

void Car_Turn(void);
void Voice_control(void);

int main(void)
{ 
	int center_buff = 0;
	int block_buff = 0;
	char *centralbuff=NULL;//中心坐标
	char *colorblock=NULL;//色块像素
  char *splitstr=",";//切割标记
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	
	delay_init(168);    	//初始化延时函数
	usart_init(9600);		//接语音识别模块
	usart_init2(115200);	//接OpenMV
	USART3_Init(9600);		//接蓝牙  
	IIC_Init();						//IIC初始化
	PCA9685_reset();
	PCA9685_setPWMFreq(50);
	up();									//机械臂复位

	while(1)
	{
		/*************************************************************************************************************************************
		********************************************************视觉跟随、抓取实现部分********************************************************
		**************************************************************************************************************************************/
		if(strcmp(rx_buffer,","))
		{
			centralbuff = strtok( rx_buffer, splitstr);//建立字符串并获取第一个标记
			colorblock = strtok(NULL, "");//指向前面被切割掉的首地址
			
			if(centralbuff != NULL && colorblock  != NULL)
			{		 
				center_buff = atoi(centralbuff);//中心坐标
				block_buff = atoi(colorblock);//色块像素

				memset(centralbuff, 0, sizeof((char)centralbuff));
				memset(colorblock, 0, sizeof((char)colorblock));
				memset(colorblock, 0, sizeof(rx_buffer));
				
				if(center_buff != 0 || block_buff != 0)
				{
					if(center_buff >= -35)//右转
					{
						Car_right(CarSpeedControl);stop();
					}
					if(center_buff <= -37)//左转
					{
						Car_left(CarSpeedControl);stop();
					}
					
					if((block_buff >= -1290 && block_buff <= -1260)&& (center_buff <= -35 && center_buff >= -37))
					{
						stop();
						steer();
					}
					if(block_buff < -1290 && (center_buff <= -35 && center_buff >= -37))
					{
						Car_go(CarSpeedControl);stop();
					}
					if(block_buff > -1260 && (center_buff <= -35 && center_buff >= -37))
					{
						Car_back(CarSpeedControl);
					}
				}
				center_buff = 0;
				block_buff = 0;
			}		
		}
		
		if(strcmp(rx_buffer,"find_error\r\n")==0)//没有寻找到
		{
			stop();
		}
		
		/*************************************************************************************************************************************
		************************************************************小程序、APP实现部分*******************************************************
		**************************************************************************************************************************************/
		Car_Turn();
		
		/*************************************************************************************************************************************
		***************************************************************语音控制实现部分*******************************************************
		**************************************************************************************************************************************/
		Voice_control();
		
		delay_ms(15);
	} 	    
}


void Car_Turn(void)
{
	int i;
	switch(data)
	{
		case 48:stop();break;//0,停止
		case 49:Car_go(CarSpeedControl);break;//1,前进
		case 50:Car_back(CarSpeedControl);break;//2,后退
		case 51:Car_left(CarSpeedControl);break;//3,左转
		case 52:Car_right(CarSpeedControl);break;//4,右转
		case 53:CarSpeedControl+=34;
				if(CarSpeedControl>2048)
				{
					CarSpeedControl=2048; //5,加速
				}break;
		case 54:CarSpeedControl-=34; //6,减速
				if(CarSpeedControl<1024)
				{
					CarSpeedControl=1024;
				}
				else if(CarSpeedControl<860)
				{
				 CarSpeedControl=860;
				}break;
		case 97:open();break;//a,打开
		case 98:close();break; //b,关闭
		case 99:up();break;//c,上升
		case 100:down();break;//d,下降
		case 101:up();break;//e,初始化位置
		case 102:
			for(i=0;i<2;i++)
			{
				Stacking_pile();
			}
			flags = 0;break;//f，一键抓取		跺堆
	}
}

void Voice_control(void)
{
	switch(uart1_data)
	{
		case 0x01:Car_go(CarSpeedControl);delay_ms(500);uart1_data=0xff;break;//1,前进
		case 0x02:Car_back(CarSpeedControl);delay_ms(500);uart1_data=0xff;break;//2,后退
		case 0x03:Car_left(CarSpeedControl);delay_ms(500);uart1_data=0xff;break;//3,左转
		case 0x04:Car_right(CarSpeedControl);delay_ms(500);uart1_data=0xff;break;//4,右转
		case 0x05:Song_steer();delay_ms(500);uart1_data=0xff;break;//5,抓取
		case 0x06:Song_down();delay_ms(500);uart1_data=0xff;break;//6,放下
		default:stop();break;
	}
}
