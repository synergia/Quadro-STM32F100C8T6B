#include "naglowki_include.h"
#include "ADC.h"
#include "timery.h"
#include "LED.h"
#include "dane.h"

extern volatile daneTypeDef dane;

void inicjalizacja_ADC()
{
	GPIO_InitTypeDef *adc_pin = malloc(sizeof(GPIO_InitTypeDef));
	adc_pin->GPIO_Pin = GPIO_Pin_2;
	adc_pin->GPIO_Mode = GPIO_Mode_AIN; //wejscie analog input
	adc_pin->GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, adc_pin);
	free (adc_pin);

	ADC1->CR2 = ADC_CR2_ADON | ADC_CR2_CONT; //tryb ciagly
	ADC1->SQR1 = 0; //jeden kana³
	ADC1->SQR3 = 2; //jako ten jeden kana³ wejscie nr 2
	ADC1->SMPR2 = 0b111000000; //najdluzszy cykl
	ADC1->CR1 = ADC_CR1_EOCIE; //wlaczane przerwanie

	//kalibracja
	ADC1->CR2 |= ADC_CR2_RSTCAL;
	while(ADC1->CR2 & ADC_CR2_RSTCAL);
	ADC1->CR2 |= ADC_CR2_CAL;
	while(ADC1->CR2 & ADC_CR2_CAL);

	ADC1->CR2 |= ADC_CR2_ADON; //wlaczenie konwersji
}

void ADC1_IRQHandler(void)
{
	if(ADC1->SR & ADC_SR_EOC) //na wszelki wypadek
	{
		uint16_t bateria = ADC1->DR;
		dane.bateria.poziom += bateria;
		dane.bateria.ktora++;

		if (dane.bateria.ktora >= SREDNIA)
		{
			/*
			 * V_in / V_ref * 2^12
			 *
			 * 8,41 V (100%) - 1882
			 * 6,8 V (0%) - 1521
			 *
			 * 1882 - 1521 = 361
			 * 361/100 = 3,61
			 * 1/3,61 = 0,277
			 */

			dane.bateria.poziom = dane.bateria.poziom >> PRZESUN;
			dane.bateria.ktora = 0;
			dane.bateria.poziom_procent = (uint8_t)((dane.bateria.poziom - 1521)*0.277);

			if (dane.bateria.poziom >= 1786) //8,0 V
			{
				GPIO_WriteBit(LED_PORT, LED_NIEB_2, Bit_SET);
				GPIO_WriteBit(LED_PORT, LED_ZOL_1 | LED_ZOL_2 | LED_CZER_1, Bit_RESET);
			}
			else if (dane.bateria.poziom >= 1723) //7,7 V
			{
				GPIO_WriteBit(LED_PORT, LED_ZOL_1, Bit_SET);
				GPIO_WriteBit(LED_PORT, LED_NIEB_2 | LED_ZOL_2 | LED_CZER_1, Bit_RESET);
			}
			else if (dane.bateria.poziom >= 1655) //7,4 V
			{
				GPIO_WriteBit(LED_PORT, LED_ZOL_2, Bit_SET);
				GPIO_WriteBit(LED_PORT, LED_NIEB_2 | LED_ZOL_1 | LED_CZER_1, Bit_RESET);
			}
			else
			{
				GPIO_WriteBit(LED_PORT, LED_CZER_1, Bit_SET);
				GPIO_WriteBit(LED_PORT, LED_NIEB_2 | LED_ZOL_2 | LED_ZOL_1, Bit_RESET);
			}
		}
	}
}
