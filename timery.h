#include "naglowki_include.h"

#ifndef TIMERY_H_
#define TIMERY_H_

/*
 * 50 Hz
 */

#define T_PSC2 10000
#define T_ARR2 47

#define T_PSC3 80
#define T_ARR3 2

/*
 * 1kHz, 100 kroków
 * 100 * ARR * PSC = 24 000 000 / 1 000
 * ARR * PSC = 240
 * ARR = 2 -> (0,1,2) -> 3
 * PSC = 80
 * 3 * 80 = 240
 */

#define T_PSC4 1000
#define T_ARR4 18//23 niby powinno przy 1 ms ale dwa regulatory na to nie reaguj¹ musi byæ krótszy :(
#endif

void inicjalizacja_SysTick(); //odpowiedzialny za mruganie ledem i wylaczanie awaryjne silnikow

void inicjalizacja_TIM1(); //odpowiedzialny za zbieranie danych z akcelerometru 1kHz

void inicjalizacja_TIM2(); //TIM2 odpowiedzialny za sygnal co 20 ms

void inicjalizacja_TIM3(); //TIM3 odpowiedzialny za szerokosc impulsu 1 - 2ms

void inicjalizacja_TIM4(); //TIM4 odpowiedzialny za impuls dokladnie 1 ms
