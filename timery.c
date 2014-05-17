#include "naglowki_include.h"
#include "timery.h"

void inicjalizacja_SysTick()
{
	SysTick_Config(SystemCoreClock/2); //co 0,5 s - nie może być więcej bo 24 bity
}

void inicjalizacja_TIM2()
{
	//struktura timera
	TIM_TimeBaseInitTypeDef *timer = malloc(sizeof(TIM_TimeBaseInitTypeDef));
	timer->TIM_CounterMode = TIM_CounterMode_Up; //zliczanie do gory
	timer->TIM_Prescaler = 24000; //preskaler z 24 MHz
	timer->TIM_Period = 1000; //do tej liczby zlicza
	timer->TIM_ClockDivision = 0;

	TIM_TimeBaseInit(TIM2, timer);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //wlaczenie przerwania TIM2
	TIM_Cmd(TIM2, ENABLE);

	free(timer);
}

void inicjalizacja_TIM3()
{
	//struktura timera
	TIM_TimeBaseInitTypeDef *timer = malloc(sizeof(TIM_TimeBaseInitTypeDef));
	timer->TIM_CounterMode = TIM_CounterMode_Up; //zliczanie do gory
	timer->TIM_Prescaler = 24000; //preskaler z 24 MHz
	timer->TIM_Period = 2000; //do tej liczby zlicza
	timer->TIM_ClockDivision = 0;

	TIM_TimeBaseInit(TIM3, timer);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //wlaczenie przerwania TIM2
	TIM_Cmd(TIM3, ENABLE);

	free(timer);
}
