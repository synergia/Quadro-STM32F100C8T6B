#include "naglowki_include.h"
#include "LED.h"
#include "timery.h"

int timer;
int pwm;

void inicjalizacja_LED()
{
	GPIOA->CRH = 0b00100010001000100010; //diody wyjscie, 2MHz
}

void SysTick_Handler(void)
{
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) //sprawdzenie zrodla
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update); //wyzerowanie flagi przerwania

		if (pwm >= timer)
			GPIO_WriteBit(GPIOA, LED_NIEB_1, Bit_SET);
		else
			GPIO_WriteBit(GPIOA, LED_NIEB_1, Bit_RESET);
		if (timer >= 99)
			timer = 0;
		timer++;
	}
}

void TIM1_BRK_TIM15_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM15, TIM_IT_Update) == SET) //sprawdzenie zrodla
	{
		TIM_ClearFlag(TIM15, TIM_FLAG_Update); //wyzerowanie flagi przerwania
	}
}

void TIM1_UP_TIM16_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM16, TIM_IT_Update) == SET) //sprawdzenie zrodla
	{
		//LED_PORT->ODR ^= LED_NIEB_1;
		TIM_ClearFlag(TIM16, TIM_FLAG_Update); //wyzerowanie flagi przerwania
	}
}
