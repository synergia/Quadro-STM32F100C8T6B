#include "USART.h"
#include "dane.h"
#include "silniki.h"

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
	usart->USART_BaudRate = 115200;
	usart->USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart->USART_Parity = USART_Parity_No;
	usart->USART_StopBits = USART_StopBits_1;
	usart->USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1, usart);
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

	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //sprawdzenie czy aby na pewno odpowiednie przerwanie
	{
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		dane.czy_polaczony = 1;

		dane_usart = USART_ReceiveData(USART1);
		if (dane_usart == 'S') //ustawienia silnikow
		{
			USART_potwierdz();
			//ktory silnik
			while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
			dane_usart = USART_ReceiveData(USART1);

			if (dane_usart == '1')
			{
				USART_potwierdz();

				while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
				dane_usart = USART_ReceiveData(USART1); //wartosc PWM

				if (dane_usart >= 0 && dane_usart <= 100)
				{
					dane.pwm.pwm1 = dane_usart;
					USART_potwierdz();
				}
				else
					USART_blad();
			}
			else if (dane_usart == '2')
			{
				USART_potwierdz();

				while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
				dane_usart = USART_ReceiveData(USART1); //wartosc PWM

				if (dane_usart >= 0 && dane_usart <= 100)
				{
					dane.pwm.pwm2 = dane_usart;
					USART_potwierdz();
				}
				else
					USART_blad();
			}
			else if (dane_usart == '3')
			{
				USART_potwierdz();

				while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
				dane_usart = USART_ReceiveData(USART1); //wartosc PWM

				if (dane_usart >= 0 && dane_usart <= 100)
				{
					dane.pwm.pwm3 = dane_usart;
					USART_potwierdz();
				}
				else
					USART_blad();
			}
			else if (dane_usart == '4')
			{
				USART_potwierdz();

				while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
				dane_usart = USART_ReceiveData(USART1); //wartosc PWM

				if (dane_usart >= 0 && dane_usart <= 100)
				{
					dane.pwm.pwm4 = dane_usart;
					USART_potwierdz();
				}
				else
					USART_blad();
			}
			else if (dane_usart == '5') // wszystkie silniki
			{
				USART_potwierdz();

				while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
				dane_usart = USART_ReceiveData(USART1); //wartosc PWM

				if (dane_usart >= 0 && dane_usart <= 100)
				{
					dane.pwm.pwm1 = dane_usart;
					dane.pwm.pwm2 = dane_usart;
					dane.pwm.pwm3 = dane_usart;
					dane.pwm.pwm4 = dane_usart;

					USART_potwierdz();
				}
				else
					USART_blad();
			}
			else
				USART_blad();
		}
		else if (dane_usart == '0') //zadnych zmian lub dodajemy do garow!
		{
			USART_potwierdz();
			//czy dodajemy koksu?
			while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
			dane_usart = USART_ReceiveData(USART1);

			if(dane_usart == '+')
			{
				if (dane.pwm.pwm1 < PWM_MAX)
					dane.pwm.pwm1++;
				if (dane.pwm.pwm2 < PWM_MAX)
					dane.pwm.pwm2++;
				if (dane.pwm.pwm3 < PWM_MAX)
					dane.pwm.pwm3++;
				if (dane.pwm.pwm4 < PWM_MAX)
					dane.pwm.pwm4++;
				USART_potwierdz();
			}
			else if(dane_usart == '-')
			{
				if (dane.pwm.pwm1 > PWM_MIN)
					dane.pwm.pwm1--;
				if (dane.pwm.pwm2 > PWM_MIN)
					dane.pwm.pwm2--;
				if (dane.pwm.pwm3 > PWM_MIN)
					dane.pwm.pwm3--;
				if (dane.pwm.pwm4 > PWM_MIN)
					dane.pwm.pwm4--;
				USART_potwierdz();
			}
			else if (dane_usart == '0')
			{
				USART_potwierdz();
				
				/*
				* wysylanie danych z ZYROSKOPU
				*/
				
				USART1->DR = dane.zyro.zyro_x_l;
				while(!(USART1->SR & USART_SR_TC)) {}
				USART1->DR = dane.zyro.zyro_x_h;
				while(!(USART1->SR & USART_SR_TC)) {}
				
				/*
				* wysylanie danych z MAGNETOMETRU
				*/
				
				USART1->DR = dane.magnet.magnet_x_l;
				while(!(USART1->SR & USART_SR_TC)) {}
				USART1->DR = dane.magnet.magnet_x_h;
				while(!(USART1->SR & USART_SR_TC)) {}

				USART1->DR = dane.pwm.pwm4;
				while(!(USART1->SR & USART_SR_TC)) {}
			}
			else
				USART_blad();
		}
		else
			USART_blad();

		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	}
}

inline void USART_potwierdz()
{
	USART1->DR = '0';
	while(!(USART1->SR & USART_SR_TC)) {}
}

inline void USART_blad()
{
	USART1->DR = '1';
	while(!(USART1->SR & USART_SR_TC)) {}
}
