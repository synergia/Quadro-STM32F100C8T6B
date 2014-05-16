#include <stdlib.h>
#include <stdint.h>
#include "cmsis_boot/stm32f10x.h"
#include "cmsis_boot/system_stm32f10x.h"
#include "cmsis/core_cm3.h"
#include "stm_lib/inc/stm32f10x_rcc.h"
#include "stm_lib/inc/stm32f10x_gpio.h"

int main(void)
{
	SystemInit();


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef *led_ready = malloc(sizeof(GPIO_InitTypeDef));
	led_ready->GPIO_Pin = GPIO_Pin_All;
	led_ready->GPIO_Mode = GPIO_Mode_Out_PP; //wyjscie pull-up
	led_ready->GPIO_Speed = GPIO_Speed_2MHz; //mo¿na wybraæ 2,10,50 MHz
	GPIO_Init(GPIOA, led_ready);
	free (led_ready);

    while(1)
    {
    	GPIO_WriteBit(GPIOA, GPIO_Pin_All, Bit_SET);
    }
}
