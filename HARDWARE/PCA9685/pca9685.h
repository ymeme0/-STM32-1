#ifndef __PCA_H
#define __PCA_H	
#include "stm32f4xx.h"


//#define PCA9685_adrr 0x80	//��ַ���䣺ģ�����Ϸ������̽�ʱ�ĵ�ַ
#define PCA9685_adrr 0x82		//  1+A5+A4+A3+A2+A1+A0+w/r 
  													//	Ƭѡ��ַ�������ӵ���1�ɸı��ַ��
  													//  ��IIC�������ж�ƬPCA9685����ͬ��ַʱ���躸��

#define PCA9685_MODE1 0x00	//���üĴ���1
#define PCA9685_PRESCALE 0xFE	//�������ڼĴ���

#define LED0_ON_L 0x06	//LED_ON_L
#define LED0_ON_H 0x07	//LED0_ON_H
#define LED0_OFF_L 0x08//LED0_OFF_L
#define LED0_OFF_H 0x09//LED0_OFF_H

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

#define SERVOMIN  115 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  590 // this is the 'maximum' pulse length count (out of 4096)
#define SERVO000  130 //0�ȶ�Ӧ4096���������ֵ
#define SERVO180  520 //180�ȶ�Ӧ4096���������ֵ���ĸ�ֵ�ɸ��ݲ�ͬ����޸�

void PCA9685_write(u8 adr,u8 data);
u8 PCA9685_read(u8 adr);
void PCA9685_reset(void);
void PCA9685_setPWMFreq(float freq) ;
void PCA9685_setpwm(u8 num, u32 on, u32 off) ;

#endif



