#ifndef  __CAR_H__
#define __CAR_H__

#include "stm32f4xx.h"

#define SERV000 	103  //0度对应4096的脉宽计数值
#define SERVO005 	114
#define SERVO010	125 	
#define SERVO015 	137
#define SERVO020	148
#define SERVO025	159
#define SERVO030 	171
#define SERVO035 	183
#define SERVO040 	194
#define SERVO045 	205
#define SERVO050 	216
#define SERVO055 	227
#define SERVO060 	239
#define SERVO065 	251
#define SERVO070 	262
#define SERVO075 	273
#define SERVO080 	284
#define SERVO085 	295
#define SERVO090 	307
#define SERVO095	318 
#define SERVO100 	329
#define SERVO105 	341
#define SERVO110 	352
#define SERVO115 	363
#define SERVO120 	375
#define SERVO125 	386
#define SERVO130 	397
#define SERVO135 	409
#define SERVO140	421 
#define SERVO145 	432
#define SERVO150 	443
#define SERVO155 	455
#define SERVO160 	466
#define SERVO165 	477
#define SERVO170 	489
#define SERVO175	500
#define SERV180 	512  //0度对应4096的脉宽计数值，四个值可根据不同舵机修改

extern int flags;

void block_exit(void);

// 函数声明区
void Car_go(int CarSpeed);
void Car_back(int CarSpeed);
void Car_left(int CarSpeed);
void Car_right(int CarSpeed);
void steer(void);
void stop(void);
void open(void);
void close(void);
void down(void);
void up(void);

int Set_pwm(int num);
void down1(void);
void Stacking_pile(void);

void Song_steer(void);
void Song_down(void);

void up2down(void);
void down2up(void);

#endif





