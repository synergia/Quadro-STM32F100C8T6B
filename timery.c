#include "naglowki_include.h"
#include "timery.h"

void inicjalizacja_SysTick()
{
	SysTick_Config(SystemCoreClock/2); //co 0,5 s - nie może być więcej bo 24 bity
}

void inicjalizacja_TIM2() //timer odpowiedzialny za PWM
{
	TIM2->CR1 = TIM_CR1_URS | TIM_CR1_CEN; //tylko overflow
	TIM2->DIER = TIM_DIER_UIE; //interrupt enable
	TIM2->PSC = T_PSC2;
	TIM2->ARR = T_ARR2;
}

void inicjalizacja_TIM3() //timer odpowiedzialny za PWM
{
	TIM3->CR1 = TIM_CR1_URS | TIM_CR1_CEN; //tylko overflow
	TIM3->DIER = 0; //interrupt disable
	TIM3->PSC = T_PSC3;
	TIM3->ARR = T_ARR3;
}

void inicjalizacja_TIM4() //timer odpowiedzialny za PWM
{
	TIM4->CR1 = TIM_CR1_URS | TIM_CR1_CEN; //tylko overflow
	TIM4->DIER = 0; //interrupt disable
	TIM4->PSC = T_PSC4;
	TIM4->ARR = T_ARR4;
}
