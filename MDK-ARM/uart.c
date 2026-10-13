#include "uart.h"

void uart1_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

	 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

	 GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_9;
	 GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF_PP;		
	 GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;

	 GPIO_Init(GPIOA , &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_10;
	 GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_IN_FLOATING;		
	 GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;

	 GPIO_Init(GPIOA , &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate             	= 115200;
	USART_InitStructure.USART_WordLength			= USART_WordLength_8b;
	USART_InitStructure.USART_Mode        			= USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity                = USART_Parity_No;
	USART_InitStructure.USART_StopBits    			= USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;

	USART_Init(USART1 , &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);
}

void uart1_send_string(char *str)
{
	for(int i=0; str[i]!=0;i++)
		{
			USART_SendData(USART1, str[i]);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		}
}	
	
	
