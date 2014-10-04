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

		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)== RESET);
		dane_usart = USART_ReceiveData(USART1);
		USART_ClearFlag(USART1,USART_FLAG_RXNE);
		if (dane_usart == '0') // bez wyslania czegos komunikacja czasem sie zawala
		{
			USART_potwierdz();
			/*
			 * wysylanie danych z AKCELEROMETRU
			 */

			USART1->DR = dane.akcel.akcel_x_kat_rad >> 8; //najpierw najstarsze
			while(!(USART1->SR & USART_SR_TC)) {}
			USART1->DR = dane.akcel.akcel_x_kat_rad;
			while(!(USART1->SR & USART_SR_TC)) {}

			USART1->DR = dane.akcel.akcel_y_kat_rad >> 8;
			while(!(USART1->SR & USART_SR_TC)) {}
			USART1->DR = dane.akcel.akcel_y_kat_rad;
			while(!(USART1->SR & USART_SR_TC)) {}

			USART1->DR = dane.zyro.zyro_z_kat_mdeg >> 24;
			while(!(USART1->SR & USART_SR_TC)) {}
			USART1->DR = dane.zyro.zyro_z_kat_mdeg >> 16;
			while(!(USART1->SR & USART_SR_TC)) {}
			USART1->DR = dane.zyro.zyro_z_kat_mdeg >> 8;
			while(!(USART1->SR & USART_SR_TC)) {}
			USART1->DR = dane.zyro.zyro_z_kat_mdeg;
			while(!(USART1->SR & USART_SR_TC)) {}

			//bateria
			USART1->DR = dane.bateria.poziom_procent;
			while(!(USART1->SR & USART_SR_TC)) {}

			//temperatura
			USART1->DR = dane.baro.temp_celsius;
			while(!(USART1->SR & USART_SR_TC)) {}

			//cisnienie
			USART1->DR = dane.baro.press_mbar >> 8;
			while(!(USART1->SR & USART_SR_TC)) {}
			USART1->DR = dane.baro.press_mbar;
			while(!(USART1->SR & USART_SR_TC)) {}

			//ustawianie silnikow
			while (!(USART1->SR & USART_SR_RXNE));
			dane.pwm.pwm1 = USART1->DR;

			while (!(USART1->SR & USART_SR_RXNE));
			dane.pwm.pwm2 = USART1->DR;

			while (!(USART1->SR & USART_SR_RXNE));
			dane.pwm.pwm3 = USART1->DR;

			while (!(USART1->SR & USART_SR_RXNE));
			dane.pwm.pwm4 = USART1->DR;
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
