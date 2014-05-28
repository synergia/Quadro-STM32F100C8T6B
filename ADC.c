#include "naglowki_include.h"
#include "ADC.h"
#include "timery.h"
#include "LED.h"

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


		if (bateria >= 3370)
		{
			GPIO_WriteBit(LED_PORT, LED_NIEB_2, Bit_SET);
			GPIO_WriteBit(LED_PORT, LED_ZOL_1 | LED_ZOL_2 | LED_CZER_1, Bit_RESET);
		}
		else if (bateria >= 3130)
		{
			GPIO_WriteBit(LED_PORT, LED_ZOL_1, Bit_SET);
			GPIO_WriteBit(LED_PORT, LED_NIEB_2 | LED_ZOL_2 | LED_CZER_1, Bit_RESET);
		}
		else if (bateria >= 2930)
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
