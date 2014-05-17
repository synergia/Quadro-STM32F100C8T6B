#include "naglowki_include.h"
#include "inicjalizacja.h"

void inicjalizacja_zasilania()
{
	SystemInit(); //24 MHz - mno�nik z 8 MHz kwarcu

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //diody
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //TIM4

}
