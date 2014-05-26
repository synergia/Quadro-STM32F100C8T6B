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
