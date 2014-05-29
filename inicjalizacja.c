#include "naglowki_include.h"
#include "inicjalizacja.h"

void inicjalizacja_zasilania()
{
	SystemInit(); //24 MHz - mno¿nik z 8 MHz kwarcu

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //diody
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //wyjscia na silniki
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ADC do baterii
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //USART2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //wlaczane funkcje alternatywne dla pinow
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE); //TIM15
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE); //TIM16
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //TIM16

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //TIM4

}
