#include "uart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "misc.h"

#define UART_TX_BUF_SIZE  64

static char tx_buf[UART_TX_BUF_SIZE];
static volatile uint16_t tx_head = 0;
static volatile uint16_t tx_tail = 0;


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

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel  =  USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);	//优先级设置


}

/*中断服务函数*/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET) 
	{
		if(tx_tail != tx_head)   
		{
			USART_SendData(USART1, tx_buf[tx_tail]);
			if(++tx_tail >= UART_TX_BUF_SIZE) tx_tail = 0;//前移一步，并环形

		}
		else
		{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE); //发完，关掉中断，别再触发 
		}
	}
}

/*阻塞式*/
void uart1_send_string(char *str)
{
	for(int i=0; str[i]!=0;i++)
		{
			USART_SendData(USART1, str[i]);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		}
}	

/*非阻塞*/	
void uart1_send_string_better(char *str)
{
    while(*str)
	{
	uint16_t next = tx_head + 1;
	if(next >= UART_TX_BUF_SIZE) next = 0;  
	
	if(next == tx_tail) return;  //满了全部丢弃
	tx_buf[tx_head] = *str++; //将数据填进缓冲区
	tx_head = next ; 
	}
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}	

