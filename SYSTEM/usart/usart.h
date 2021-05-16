#ifndef _USART_H
#define _USART_H
#include "stm32f4xx.h"
#include "stdio.h"	
#include "stm32f4xx_conf.h"

extern char 	buffer[20];
extern char 	rx_buffer[20];
extern char 	rx_count;
extern u8 		rx_i;
extern u8 		rx_flag;
extern u8			i;
extern u32 		data;
extern u32  uart1_data;

void usart_init(uint32_t BaudRate);//PC ½ÓTTL
void USART3_Init(uint32_t baud);//WIFI
void wifi_set(char *pstr);
uint64_t wifi_all_set(uint64_t flag_buf);
void usart3_send_str(uint8_t *pbuf)	;
void usart_init2(uint32_t BaudRate);

#endif



