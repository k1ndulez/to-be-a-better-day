#include "Delay.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "uart.h"
#include <stdio.h>

uint16_t phase = 0;
uint16_t ramp = 0;
uint16_t cnt = 0;
char buf[16];
volatile uint32_t loop_cnt = 0;

const uint16_t sin_tab[64] = {
    512, 562, 612, 660, 707, 751, 792, 829,
    862, 890, 913, 930, 943, 950, 953, 950,
    943, 930, 913, 890, 862, 829, 792, 751,
    707, 660, 612, 562, 512, 461, 411, 363,
    316, 272, 231, 194, 161, 133, 110, 93,
    80, 73, 70, 73, 80, 93, 110, 133,
    161, 194, 231, 272, 316, 363, 411, 461
}; // 正弦表

int main()
{
    uart1_init();

    while (1)
    {

        phase = (phase + 1) % 64; // 快正弦

        cnt++; // 数帧
        if (cnt >= 10)
        {
            cnt = 0;
            ramp += 4;
            if (ramp > 1023) ramp = 0;
        } // 锯齿值
        snprintf(buf, sizeof(buf), "%d,%d\r\n", sin_tab[phase], ramp);
        uart1_send_string_better(buf);
        Delay_ms(10);
    }
}
