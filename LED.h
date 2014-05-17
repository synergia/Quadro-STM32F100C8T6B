#ifndef LED_H_
#define LED_H_

#include "naglowki_include.h"
#define LED_PORT GPIOA

#define LED_NIEB_1 GPIO_Pin_8
#define LED_NIEB_2 GPIO_Pin_9
#define LED_ZOL_1 GPIO_Pin_10
#define LED_ZOL_2 GPIO_Pin_11
#define LED_CZER_1 GPIO_Pin_12

#define LED_WSZYSTKO GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12

#endif

void inicjalizacja_LED();
