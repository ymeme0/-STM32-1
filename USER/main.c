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
	char *centralbuff=NULL;//��������
	char *colorblock=NULL;//ɫ������
  char *splitstr=",";//�и���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	
	delay_init(168);    	//��ʼ����ʱ����
	usart_init(9600);		//������ʶ��ģ��
	usart_init2(115200);	//��OpenMV
	USART3_Init(9600);		//������  
	IIC_Init();						//IIC��ʼ��
	PCA9685_reset();
	PCA9685_setPWMFreq(50);
	up();									//��е�۸�λ

	while(1)
	{
		/*************************************************************************************************************************************
		********************************************************�Ӿ����桢ץȡʵ�ֲ���********************************************************
		**************************************************************************************************************************************/
		if(strcmp(rx_buffer,","))
		{
			centralbuff = strtok( rx_buffer, splitstr);//�����ַ�������ȡ��һ�����
			colorblock = strtok(NULL, "");//ָ��ǰ�汻�и�����׵�ַ
			
			if(centralbuff != NULL && colorblock  != NULL)
			{		 
				center_buff = atoi(centralbuff);//��������
				block_buff = atoi(colorblock);//ɫ������

				memset(centralbuff, 0, sizeof((char)centralbuff));
				memset(colorblock, 0, sizeof((char)colorblock));
				memset(colorblock, 0, sizeof(rx_buffer));
				
				if(center_buff != 0 || block_buff != 0)
				{
					if(center_buff >= -35)//��ת
					{
						Car_right(CarSpeedControl);stop();
					}
					if(center_buff <= -37)//��ת
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
		
		if(strcmp(rx_buffer,"find_error\r\n")==0)//û��Ѱ�ҵ�
		{
			stop();
		}
		
		/*************************************************************************************************************************************
		************************************************************С����APPʵ�ֲ���*******************************************************
		**************************************************************************************************************************************/
		Car_Turn();
		
		/*************************************************************************************************************************************
		***************************************************************��������ʵ�ֲ���*******************************************************
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
		case 48:stop();break;//0,ֹͣ
		case 49:Car_go(CarSpeedControl);break;//1,ǰ��
		case 50:Car_back(CarSpeedControl);break;//2,����
		case 51:Car_left(CarSpeedControl);break;//3,��ת
		case 52:Car_right(CarSpeedControl);break;//4,��ת
		case 53:CarSpeedControl+=34;
				if(CarSpeedControl>2048)
				{
					CarSpeedControl=2048; //5,����
				}break;
		case 54:CarSpeedControl-=34; //6,����
				if(CarSpeedControl<1024)
				{
					CarSpeedControl=1024;
				}
				else if(CarSpeedControl<860)
				{
				 CarSpeedControl=860;
				}break;
		case 97:open();break;//a,��
		case 98:close();break; //b,�ر�
		case 99:up();break;//c,����
		case 100:down();break;//d,�½�
		case 101:up();break;//e,��ʼ��λ��
		case 102:
			for(i=0;i<2;i++)
			{
				Stacking_pile();
			}
			flags = 0;break;//f��һ��ץȡ		���
	}
}

void Voice_control(void)
{
	switch(uart1_data)
	{
		case 0x01:Car_go(CarSpeedControl);delay_ms(500);uart1_data=0xff;break;//1,ǰ��
		case 0x02:Car_back(CarSpeedControl);delay_ms(500);uart1_data=0xff;break;//2,����
		case 0x03:Car_left(CarSpeedControl);delay_ms(500);uart1_data=0xff;break;//3,��ת
		case 0x04:Car_right(CarSpeedControl);delay_ms(500);uart1_data=0xff;break;//4,��ת
		case 0x05:Song_steer();delay_ms(500);uart1_data=0xff;break;//5,ץȡ
		case 0x06:Song_down();delay_ms(500);uart1_data=0xff;break;//6,����
		default:stop();break;
	}
}
