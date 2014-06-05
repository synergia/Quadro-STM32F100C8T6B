#include "naglowki_include.h"
#include "inicjalizacja.h"
#include "timery.h"
#include "LED.h"
#include "silniki.h"
#include "ADC.h"
#include "NVIC.h"
#include "USART.h"
#include "dane.h"
#include "I2C.h"
#include "sensory.h"

int main(void)
{
	inicjalizacja_zasilania();
	inicjalizacja_silniki();
	inicjalizacja_dane();
	inicjalizacja_LED();
	inicjalizacja_SysTick();
	inicjalizacja_ADC();
	inicjalizacja_USART();
	inicjalizacja_I2C();
	inicjalizacja_TIM2();
	inicjalizacja_TIM4();
	inicjalizacja_TIM3();
	inicjalizacja_sensory();
	inicjalizacja_TIM1();

	inicjalizacja_NVIC();

    while(1)
    {
    }
}
