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
