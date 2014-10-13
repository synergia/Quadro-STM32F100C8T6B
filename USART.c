#include "USART.h"
#include "dane.h"
#include "silniki.h"
#include "LED.h"

extern volatile daneTypeDef dane;

void inicjalizacja_USART()
{
	GPIO_PinRemapConfig(AFIO_MAPR_USART1_REMAP, ENABLE ); //remap na wlasciwe piny

	GPIO_InitTypeDef *usart_gpio = malloc(sizeof (GPIO_InitTypeDef));
	//PB6 jako TX
	usart_gpio->GPIO_Pin = GPIO_Pin_6;
	usart_gpio->GPIO_Mode = GPIO_Mode_AF_PP;
	usart_gpio->GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, usart_gpio);
	//PB7 jako RX
	usart_gpio->GPIO_Pin = GPIO_Pin_7;
	usart_gpio->GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, usart_gpio);
	free(usart_gpio);

	USART_Cmd(USART1, ENABLE);

	USART_InitTypeDef *usart = malloc(sizeof(USART_InitTypeDef));
	usart->USART_BaudRate = 9600;
	usart->USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart->USART_Parity = USART_Parity_No;
	usart->USART_StopBits = USART_StopBits_1;
	usart->USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1, usart);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	free(usart);
}

/*
 * S - ustawienia silnikow
 * 0 - przyjeto polecenie (ASCII 48)
 * 1 - blad (49)
 */

void USART1_IRQHandler(void)
{
	uint8_t dane_usart=0;

	if (USART_GetFlagStatus(USART1, USART_IT_ORE) != RESET) // overrun error
		dane_usart = (int8_t)(USART1->SR & (uint8_t)0xFF);

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //sprawdzenie czy aby na pewno odpowiednie przerwanie
	{
		dane.czy_polaczony = 1;

		dane_usart = USART1->DR;

		if (dane_usart == 's') // rozpocznij odbieranie danych
		{
			dane.usart.bufor = 0;
			GPIOA->ODR |= LED_NIEB_1;
		}
		else if(dane.usart.bufor == 0) // pwm1
		{
			dane.pwm.pwm1 = dane_usart;
			dane.usart.bufor++;
		}
		else if(dane.usart.bufor == 1) // pwm2
		{
			dane.pwm.pwm2 = dane_usart;
			dane.usart.bufor++;
		}
		else if(dane.usart.bufor == 2) // pwm3
		{
			dane.pwm.pwm3 = dane_usart;
			dane.usart.bufor++;
		}
		else if(dane.usart.bufor == 3) // pwm4, wlacz przerwanie Transmit
		{
			dane.pwm.pwm4 = dane_usart;
			dane.usart.bufor++;

			USART1->DR = dane.kat.kat_x >> 8;
			USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		}
	}
    else if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET) // wysylanie danych
    {
    	switch(dane.usart.bufor)
    	{
		// AKCELEROMETR
    	case (4):
			USART1->DR = dane.kat.kat_x;
    		break;
    	case (5):
			USART1->DR = dane.kat.kat_y >> 8;
    		break;
    	case (6):
			USART1->DR = dane.kat.kat_y;
    		break;
    	// ZYROSKOP
    	case (7):
			USART1->DR = dane.zyro.zyro_z_kat_mdeg >> 24;
    		break;
    	case (8):
			USART1->DR = dane.zyro.zyro_z_kat_mdeg >> 16;
			break;
    	case (9):
			USART1->DR = dane.zyro.zyro_z_kat_mdeg >> 8;
			break;
    	case (10):
			USART1->DR = dane.zyro.zyro_z_kat_mdeg;
			break;
		// BATERIA
    	case (11):
			USART1->DR = dane.bateria.poziom_procent;
			break;
		// TEMPERATURA
    	case (12):
			USART1->DR = dane.baro.temp_celsius;
    		break;
    	// CISNIENIE
    	case (13):
			USART1->DR = dane.baro.press_mbar >> 8;
    		break;
    	case (14):
			USART1->DR = dane.baro.press_mbar;
    		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    		break;
    	}

    	dane.usart.bufor++;
    }
}

inline void USART_potwierdz()
{
	USART1->DR = '0';
	while(!(USART1->SR & USART_SR_TXE)) {}
}

inline void USART_blad()
{
	USART1->DR = '1';
	while(!(USART1->SR & USART_SR_TXE)) {}
}
