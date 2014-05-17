#include "naglowki_include.h"

#ifndef TIMERY_H_
#define TIMERY_H_
#define T_PSC 6000 //24MHz / 6000 = 4000
#define T_ARR 80 //4000 / 80 = 50Hz
#endif

void inicjalizacja_SysTick();

void inicjalizacja_TIM2();

void inicjalizacja_TIM15();
void inicjalizacja_TIM16();
