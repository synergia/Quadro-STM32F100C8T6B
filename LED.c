#include "naglowki_include.h"
#include "LED.h"
#include "timery.h"
#include "dane.h"

extern volatile daneTypeDef dane;

void inicjalizacja_LED()
{
	GPIOA->CRH = 0b00100010001000100010; //diody wyjscie, 2MHz
	GPIO_WriteBit(GPIOA, LED_NIEB_1, Bit_SET);
}

void SysTick_Handler(void)
{
	//GPIOA->ODR ^= LED_NIEB_1;
	/*if (!dane.czy_polaczony)
	{
		dane.pwm.pwm1 = 0;
		dane.pwm.pwm2 = 0;
		dane.pwm.pwm3 = 0;
		dane.pwm.pwm4 = 0;
	}*/
	dane.czy_polaczony = 0;
}
