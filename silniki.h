#include "naglowki_include.h"

#ifndef _SILNIKI_H_
#define _SILNIKI_H_

#define SILNIK1  GPIO_Pin_12
#define SILNIK2  GPIO_Pin_13
#define SILNIK3	 GPIO_Pin_14
#define SILNIK4  GPIO_Pin_15

#endif

void inicjalizacja_silniki();

void ustaw_silnik(uint8_t ktory, uint8_t wartosc);
