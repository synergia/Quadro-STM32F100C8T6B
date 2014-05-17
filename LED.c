#include "naglowki_include.h"
#include "LED.h"
#include "timery.h"

void inicjalizacja_LED()
{
	GPIO_InitTypeDef *led_ready = malloc(sizeof(GPIO_InitTypeDef));
	led_ready->GPIO_Pin = LED_WSZYSTKO;
	led_ready->GPIO_Mode = GPIO_Mode_Out_PP; //wyjscie pull-up
	led_ready->GPIO_Speed = GPIO_Speed_2MHz; //moÂżna wybraĂ¦ 2,10,50 MHz
	GPIO_Init(LED_PORT, led_ready);
	free (led_ready);
}

void SysTick_Handler(void)
{
}

void TIM1_BRK_TIM15_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM15, TIM_IT_Update) == SET) //sprawdzenie zrodla
	{
		LED_PORT->ODR ^= LED_NIEB_1;
		TIM_ClearFlag(TIM15, TIM_FLAG_Update); //wyzerowanie flagi przerwania
	}
}

void TIM1_UP_TIM16_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM16, TIM_IT_Update) == SET) //sprawdzenie zrodla
	{
		TIM_ClearFlag(TIM16, TIM_FLAG_Update); //wyzerowanie flagi przerwania
	}
}
