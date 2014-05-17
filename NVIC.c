#include "naglowki_include.h"
#include "NVIC.h"

void inicjalizacja_NVIC()
{
	NVIC_InitTypeDef *nvic_timer = malloc(sizeof(NVIC_InitTypeDef));
	nvic_timer->NVIC_IRQChannel = TIM2_IRQn; //timer2
	nvic_timer->NVIC_IRQChannelPreemptionPriority = 2;
	nvic_timer->NVIC_IRQChannelSubPriority = 0;
	nvic_timer->NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(nvic_timer); //inicjalizacja NVIC
	NVIC_EnableIRQ(TIM2_IRQn);
	free(nvic_timer);

	nvic_timer = malloc(sizeof(NVIC_InitTypeDef));
	nvic_timer->NVIC_IRQChannel = TIM3_IRQn; //timer2
	nvic_timer->NVIC_IRQChannelPreemptionPriority = 2;
	nvic_timer->NVIC_IRQChannelSubPriority = 1;
	nvic_timer->NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(nvic_timer); //inicjalizacja NVIC
	NVIC_EnableIRQ(TIM3_IRQn);
	free(nvic_timer);
}
