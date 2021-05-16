#include "pca9685.h"
#include "myiic.h"
#include "delay.h"
#include "math.h"



/*---------------------------------------------------------------
                ��PCA9685��д��ַ������
----------------------------------------------------------------*/
void PCA9685_write(u8 adr,u8 data)
{ 
	 IIC_Start();
	
	 IIC_Send_Byte(PCA9685_adrr); //PCA9685��Ƭѡ��ַ
	 IIC_Wait_Ack();
	
	 IIC_Send_Byte(adr);//д��ַ�����ֽ�
	 IIC_Wait_Ack();
	
	 IIC_Send_Byte(data); //д����
	 IIC_Wait_Ack();
	
   IIC_Stop();
}

/*---------------------------------------------------------------
            ��PCA9685��ĵ�ֵַ�ж�����(�з���ֵ)
----------------------------------------------------------------*/
u8 PCA9685_read(u8 adr)
{
  u8 data;
	IIC_Start();
	
	IIC_Send_Byte(PCA9685_adrr);//PCA9685��Ƭѡ��ַ
  IIC_Wait_Ack();
	
	IIC_Send_Byte(adr);
	IIC_Wait_Ack();
	
	IIC_Start();
	
  IIC_Send_Byte(PCA9685_adrr|0x01);//��ַ�ĵڰ�λ�������������򣬾���д���
  IIC_Wait_Ack();
	
	data=IIC_Read_Byte(0);
  IIC_Stop();
	
	return data;

}

/*---------------------------------------------------------------
                        PCA9685��λ
----------------------------------------------------------------*/
void PCA9685_reset(void)
{
  PCA9685_write(PCA9685_MODE1,0x00);
}

/*---------------------------------------------------------------
											PCA9685�޸�Ƶ�ʺ���
----------------------------------------------------------------*/
void PCA9685_setPWMFreq(float freq) 
{
		u8 prescale,oldmode,newmode;
		double prescaleval;
	
		//���ýǶ�
		//60��=0.5ms+��60/180��*��2.5ms-0.5ms��=1.1666ms
		//����ռ�ձ�=1.1666ms/20ms=off/4096��off=239��50hz��Ӧ����20ms
		freq *= 0.92;//Ƶ��У׼��һ��Ҫ����0.915,����������� 
		prescaleval = 25000000;
		prescaleval /= 4096;
		prescaleval /= freq;
		prescaleval -= 1;
		prescale =floor(prescaleval + 0.5f);

		oldmode = PCA9685_read(PCA9685_MODE1);
	
		newmode = (oldmode&0x7F) | 0x10; // sleep
	
		PCA9685_write(PCA9685_MODE1, newmode); // go to sleep
	
		PCA9685_write(PCA9685_PRESCALE, prescale); // ����Ԥ��Ƶ��
	
		PCA9685_write(PCA9685_MODE1, oldmode);
		delay_ms(2);
	
		PCA9685_write(PCA9685_MODE1, oldmode | 0xa1); 
}

/*---------------------------------------------------------------
                     PCA9685�޸ĽǶȺ���
num:���PWM�������0~15
on:PWM��������ֵ0~4096
off:PWM�½�����ֵ0~4096

һ��PWM���ڷֳ�4096�ݣ���0��ʼ+1�������Ƶ�onʱ����Ϊ�ߵ�ƽ������������offʱ
����Ϊ�͵�ƽ��ֱ������4096���¿�ʼ��
���Ե�on������0ʱ������ʱ,��on����0ʱ��off/4096��ֵ����PWM��ռ�ձȡ�

//���п�ʼһ������Ϊ0��ֻ��Ҫȡ�½�ʱ�����
//�ú���Ϊ����PWM����ռ�ձȣ�һ�㶼��4096Ϊһ��PWM������
----------------------------------------------------------------*/
void PCA9685_setpwm(u8 num, u32 on, u32 off) 
{
			PCA9685_write(LED0_ON_L+4*num,on);
			PCA9685_write(LED0_ON_H+4*num,on>>8);
			PCA9685_write(LED0_OFF_L+4*num,off);
			PCA9685_write(LED0_OFF_H+4*num,off>>8);
}
/************************************************************************************************/


