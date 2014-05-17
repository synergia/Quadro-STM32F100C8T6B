#include "naglowki_include.h"
#include "inicjalizacja.h"

void inicjalizacja_zasilania()
{
	SystemInit(); //24 MHz - mno¿nik z 8 MHz kwarcu

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //diody
}
