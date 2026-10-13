#include "stm32f10x.h"
#include "Delay.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>

uint16_t phase 	= 0;
uint16_t ramp 	= 0;
uint16_t cnt    = 0;
uint16_t value1 = 0;
char buf[16];

const uint16_t sin_tab[64] = {
    512, 562, 612, 660, 707, 751, 792, 829,
    862, 890, 913, 930, 943, 950, 953, 950,
    943, 930, 913, 890, 862, 829, 792, 751,
    707, 660, 612, 562, 512, 461, 411, 363,
    316, 272, 231, 194, 161, 133, 110,  93,
     80,  73,  70,  73,  80,  93, 110, 133,
    161, 194, 231, 272, 316, 363, 411, 461
};//正弦表



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

int main()
{
uart1_init();	
	
	while(1)
	{ 

		phase=(phase+1)%64;//快正弦
		
		cnt++; //数帧                       
		if(cnt >= 10)               
		{
    	cnt = 0;                 
    	ramp+=4;                   
    	if(ramp > 1023) ramp = 0;
		}//锯齿值

		snprintf(buf, sizeof(buf), "%d,%d\r\n" , sin_tab[phase], ramp);
		for(int i=0; buf[i]!=0;i++)
		{
			USART_SendData(USART1, buf[i]);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		}
		Delay_ms(10);
	}		
}



