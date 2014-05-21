#include "naglowki_include.h"
#include "timery.h"

extern int timer;
extern int pwm;

void inicjalizacja_SysTick()
{
	SysTick_Config(SystemCoreClock/2); //co 0,5 s - nie może być więcej bo 24 bity
}

void inicjalizacja_TIM2()
{
	pwm = 3;
	TIM2->CR1 = TIM_CR1_URS | TIM_CR1_CEN; //tylko overflow
	TIM2->DIER = TIM_DIER_UIE; //interrupt enable
	TIM2->PSC = T_PSC;
	TIM2->ARR = T_ARR;
}

void inicjalizacja_TIM15()
{
	//struktura timera
	TIM_TimeBaseInitTypeDef *timer = malloc(sizeof(TIM_TimeBaseInitTypeDef));
	timer->TIM_CounterMode = TIM_CounterMode_Up; //zliczanie do gory
	timer->TIM_Prescaler = 2000; //preskaler
	timer->TIM_Period = 100; //do tej liczby zlicza
	timer->TIM_ClockDivision = 0;

	TIM_TimeBaseInit(TIM15, timer);
	TIM_ITConfig(TIM15, TIM_IT_Update, ENABLE); //wlaczenie przerwania TIM2
	TIM_Cmd(TIM15, ENABLE);

	free(timer);
}

void inicjalizacja_TIM16()
{
	//struktura timera
	TIM_TimeBaseInitTypeDef *timer = malloc(sizeof(TIM_TimeBaseInitTypeDef));
	timer->TIM_CounterMode = TIM_CounterMode_Up; //zliczanie do gory
	timer->TIM_Prescaler = 2000; //preskaler z 24 MHz
	timer->TIM_Period = 50; //do tej liczby zlicza
	timer->TIM_ClockDivision = 0;

	TIM_TimeBaseInit(TIM16, timer);
	TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE); //wlaczenie przerwania TIM2
	TIM_Cmd(TIM16, ENABLE);

	free(timer);
}
