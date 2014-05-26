#include "naglowki_include.h"
#include "inicjalizacja.h"
#include "timery.h"
#include "LED.h"
#include "silniki.h"
#include "ADC.h"
#include "NVIC.h"
#include "USART.h"

int main(void)
{
	inicjalizacja_zasilania();
	inicjalizacja_LED();
	inicjalizacja_SysTick();
	inicjalizacja_TIM2();
	inicjalizacja_silniki();
	inicjalizacja_ADC();
	inicjalizacja_USART();

	inicjalizacja_NVIC();

    while(1)
    {
    	USART1->DR = 'a';
    	while(!(USART1->SR & USART_SR_TC)) {}
    }
}
