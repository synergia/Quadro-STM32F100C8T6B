#include "naglowki_include.h"
#include "NVIC.h"

void inicjalizacja_NVIC()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	NVIC_InitTypeDef nvic_timer;
	nvic_timer.NVIC_IRQChannel = TIM2_IRQn; //timer2
	nvic_timer.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_timer.NVIC_IRQChannelSubPriority = 0;
	nvic_timer.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_timer); //inicjalizacja NVIC
	NVIC_EnableIRQ(TIM2_IRQn);

	nvic_timer.NVIC_IRQChannel = TIM3_IRQn; //timer2
	nvic_timer.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_timer.NVIC_IRQChannelSubPriority = 0;
	nvic_timer.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_timer); //inicjalizacja NVIC

	NVIC_InitTypeDef adc;
	adc.NVIC_IRQChannel = ADC1_IRQn;
	adc.NVIC_IRQChannelPreemptionPriority = 3;
	adc.NVIC_IRQChannelSubPriority = 0;
	adc.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&adc);
}
