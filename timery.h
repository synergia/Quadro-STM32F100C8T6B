#include "naglowki_include.h"

#ifndef TIMERY_H_
#define TIMERY_H_
#define T_PSC 80
#define T_ARR 2
/*
 * 1kHz, 100 kroków
 * 100 * ARR * PSC = 24 000 000 / 1 000
 * ARR * PSC = 240
 * ARR = 2 -> (0,1,2) -> 3
 * PSC = 80
 * 3 * 80 = 240
 */
#endif

void inicjalizacja_SysTick();

void inicjalizacja_TIM2();

void inicjalizacja_TIM15();
void inicjalizacja_TIM16();
