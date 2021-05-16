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
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       

//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);  
	
	return ch;
} 


//��ʼ��USART1
void usart_init(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 							//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);							//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 						//GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 						//GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 						//GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;									//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 									//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 									//����
	GPIO_Init(GPIOA,&GPIO_InitStructure); 											//��ʼ��PA9��PA10

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); 										//��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  													//ʹ�ܴ���1 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;							//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;								//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
}


//��ʼ��USART2
void usart_init2(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 							//ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);							//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 						//GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 						//GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 						//GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;									//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 									//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 									//����
	GPIO_Init(GPIOA,&GPIO_InitStructure); 											//��ʼ��PA9��PA10

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); 										//��ʼ������1
	
	USART_Cmd(USART2, ENABLE);  													//ʹ�ܴ���1 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;								//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;							//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;								//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

/*��ʼ������3*/
void USART3_Init(uint32_t baud)
{
	
	GPIO_InitTypeDef  	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;	\
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 							//ʹ��GPIOBʱ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);							//ʹ��USART3ʱ��
 
	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 						//GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 						//GPIOB11����ΪUSART3
	
	//USART3�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 						//GPIOB10��GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;									//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 									//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 									//����
	GPIO_Init(GPIOB,&GPIO_InitStructure); 											//��ʼ��PB10��PB11

	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = baud;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); 										//��ʼ������3
	
	USART_Cmd(USART3, ENABLE);  													//ʹ�ܴ���3 
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);									//��������ж�

	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;								//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;							//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;								//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);													//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

// USART1�жϷ�����:�����ַ���
void USART1_IRQHandler(void)
{	
	//���Ƿǿգ��򷵻�ֵΪ1����RESET��0���жϣ���������ж�Ϊ��
   if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
   {	
		//�ж�Ϊ���Ϊ�´��ж���׼��������Ҫ���жϵı�־����
		 USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	
		//��������ģ����Ӧ������
		uart1_data = USART_ReceiveData(USART1);
	}
			  
	//���͸�����1
	//USART_SendData(USART1, data);
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

void usart3_send_str(uint8_t *pbuf)	
{
	while(pbuf && *pbuf!='\0')
	{
		//��������
		USART_SendData(USART3,*pbuf++);
		
		//��������Ƿ������
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);	
	}
}

// USART1�жϷ�����:�����ַ���
void USART3_IRQHandler(void)
{	
	//printf("nnnn");
	//���Ƿǿգ��򷵻�ֵΪ1����RESET��0���жϣ���������ж�Ϊ��
   if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
   {	
		//�ж�Ϊ���Ϊ�´��ж���׼��������Ҫ���жϵı�־����
		 USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	
		//��������ģ����Ӧ������
		data = USART_ReceiveData(USART3);
	}
			
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

// USART1�жϷ�����
void USART2_IRQHandler(void)
{	
	 //���Ƿǿգ��򷵻�ֵΪ1����RESET��0���жϣ���������ж�Ϊ��
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{	
		//�ж�Ϊ���Ϊ�´��ж���׼��������Ҫ���жϵı�־����
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
		//��ȡ����1���͹���������
		buffer[rx_count++] = USART_ReceiveData(USART2);
			
		//�ж��Ƿ��ǻ��кͻس�
		if(buffer[rx_count-2] == 0x0D && buffer[rx_count-1] == 0x0A)
		{					
			for(rx_i = 0; rx_i<rx_count; rx_i++)
			{
					//��ת
					rx_buffer[rx_i] = buffer[rx_i];
					//���
					buffer[rx_i] = 0;
			}						 
			rx_count = 0;
		}
	}	
}
