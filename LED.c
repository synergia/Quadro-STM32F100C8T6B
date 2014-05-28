#include "naglowki_include.h"
#include "LED.h"
#include "timery.h"

void inicjalizacja_LED()
{
	GPIOA->CRH = 0b00100010001000100010; //diody wyjscie, 2MHz
	GPIO_WriteBit(GPIOA, LED_NIEB_1, Bit_SET);
}
