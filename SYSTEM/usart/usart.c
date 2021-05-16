#include "sys.h"
#include "usart.h"	
#include "string.h"
#include "stdlib.h"

char 	buffer[20] 		= {0};
char 	rx_buffer[20] 	= {0};
char 	rx_count 		= 0;
u8 		rx_i 			= 0;
u8 		rx_flag 		= 0;
u8		i 			= 0;
u32 data = 0;

u32 uart1_data = 0;

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       

//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);  
	
	return ch;
} 


//初始化USART1
void usart_init(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 							//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);							//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 						//GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 						//GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 						//GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;									//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 									//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 									//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 											//初始化PA9，PA10

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;										//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init(USART1, &USART_InitStructure); 										//初始化串口1
	
	USART_Cmd(USART1, ENABLE);  													//使能串口1 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;							//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;								//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
}


//初始化USART2
void usart_init2(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 							//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);							//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 						//GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 						//GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 						//GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;									//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 									//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 									//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 											//初始化PA9，PA10

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;										//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init(USART2, &USART_InitStructure); 										//初始化串口1
	
	USART_Cmd(USART2, ENABLE);  													//使能串口1 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;								//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;							//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;								//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
}

/*初始化串口3*/
void USART3_Init(uint32_t baud)
{
	
	GPIO_InitTypeDef  	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;	\
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 							//使能GPIOB时钟
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);							//使能USART3时钟
 
	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 						//GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 						//GPIOB11复用为USART3
	
	//USART3端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 						//GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;									//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 									//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 									//上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); 											//初始化PB10，PB11

	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = baud;										//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init(USART3, &USART_InitStructure); 										//初始化串口3
	
	USART_Cmd(USART3, ENABLE);  													//使能串口3 
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);									//开启相关中断

	//Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;								//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;							//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;								//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化VIC寄存器
}

// USART1中断服务函数:接收字符串
void USART1_IRQHandler(void)
{	
	//若是非空，则返回值为1，与RESET（0）判断，不相等则判断为真
   if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
   {	
		//判断为真后，为下次中断做准备，则需要对中断的标志清零
		 USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	
		//接收蓝牙模块响应的数据
		uart1_data = USART_ReceiveData(USART1);
	}
			  
	//发送给串口1
	//USART_SendData(USART1, data);
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

void usart3_send_str(uint8_t *pbuf)	
{
	while(pbuf && *pbuf!='\0')
	{
		//发送数据
		USART_SendData(USART3,*pbuf++);
		
		//检查数据是否发送完毕
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);	
	}
}

// USART1中断服务函数:接收字符串
void USART3_IRQHandler(void)
{	
	//printf("nnnn");
	//若是非空，则返回值为1，与RESET（0）判断，不相等则判断为真
   if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
   {	
		//判断为真后，为下次中断做准备，则需要对中断的标志清零
		 USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	
		//接收蓝牙模块响应的数据
		data = USART_ReceiveData(USART3);
	}
			
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

// USART1中断服务函数
void USART2_IRQHandler(void)
{	
	 //若是非空，则返回值为1，与RESET（0）判断，不相等则判断为真
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{	
		//判断为真后，为下次中断做准备，则需要对中断的标志清零
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
		//读取串口1发送过来的数据
		buffer[rx_count++] = USART_ReceiveData(USART2);
			
		//判断是否是换行和回车
		if(buffer[rx_count-2] == 0x0D && buffer[rx_count-1] == 0x0A)
		{					
			for(rx_i = 0; rx_i<rx_count; rx_i++)
			{
					//中转
					rx_buffer[rx_i] = buffer[rx_i];
					//清空
					buffer[rx_i] = 0;
			}						 
			rx_count = 0;
		}
	}	
}
