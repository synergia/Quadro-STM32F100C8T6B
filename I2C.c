#include "I2C.h"

void inicjalizacja_I2C_GPIO()
{
	GPIO_PinRemapConfig(AFIO_MAPR_I2C1_REMAP, ENABLE ); //remap na wlasciwe piny

	//konfiguracja portów - SPRAWDZONE jest OK
	GPIO_InitTypeDef  *i2c_gpio = malloc (sizeof (GPIO_InitTypeDef));
	i2c_gpio->GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11; //I2C1 i I2C2
	i2c_gpio->GPIO_Speed = GPIO_Speed_2MHz;
	i2c_gpio->GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, i2c_gpio);

	free(i2c_gpio);
}

void inicjalizacja_I2C()
{
	inicjalizacja_I2C_GPIO();

	//SPRAWDZONE jest OK
	I2C_InitTypeDef  *i2c = malloc (sizeof (I2C_InitTypeDef));

	/* I2C configuration */
	i2c->I2C_Mode = I2C_Mode_I2C;
	i2c->I2C_DutyCycle = I2C_DutyCycle_2;
	i2c->I2C_OwnAddress1 = 0x55;
	i2c->I2C_Ack = I2C_Ack_Enable;
	i2c->I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c->I2C_ClockSpeed = 400000;

	/* I2C Peripheral Enable */
	I2C_Cmd(I2C2, ENABLE);
	/* Apply I2C configuration after enabling it */
	I2C_Init(I2C2, i2c);

	i2c->I2C_OwnAddress1 = 0x65;
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, i2c);

	//free(i2c);
}

void odczyt_I2C(I2C_TypeDef *I2Cx, uint8_t adres, uint8_t rejestr, uint8_t dlugosc, uint8_t *bufor)
{
	while(I2Cx->SR2 & I2C_SR2_BUSY) //sprawdzanie czy I2C wolny
	{
		if(sprawdz_blad_I2C(I2Cx))
		{
			//i2c_config();
			return;
		}
	}

	I2Cx->CR1 |= I2C_CR1_START; //Wyslanie START
	while( !( I2Cx->SR1 & I2C_SR1_SB )) //sprawdzanie czy siê wyslalo
	{
		if(sprawdz_blad_I2C(I2Cx))
		{
			//i2c_config();
			return;
		}
	}
	I2Cx->DR = adres; //przed zapisaniem SR2, trzeba do DR zapisaæ adres
	while( !( I2Cx->SR1 & I2C_SR1_ADDR ))
	{
		if(sprawdz_blad_I2C(I2Cx))
		{
			//i2c_config();
			return;
		}
	}
	I2Cx->SR2; //konieczne wg datasheet, czyszczenie rejestru SR1
	while( !( I2Cx->SR1 & I2C_SR1_TXE ))
	{
		if(sprawdz_blad_I2C(I2Cx))
		{
			//i2c_config();
			return;
		}
	}

	/*
	wys³anie nr rejestru do czytania, najbardziej
	znacz¹cy bit musi byæ równy 1 */

	I2Cx->DR = rejestr;

	while( !( I2Cx->SR1 & I2C_SR1_BTF ))
	{
		if(sprawdz_blad_I2C(I2Cx))
		{
			//i2c_config();
			return;
		}
	}
	I2Cx->CR1 |= I2C_CR1_START; //Wyslanie START
	while( !( I2Cx->SR1 & I2C_SR1_SB ))
	{
		if(sprawdz_blad_I2C(I2Cx))
		{
			//i2c_config();
			return;
		}
	}
	I2Cx->DR = adres | 0x01; //Wyslanie adresu czytania
	while( !( I2Cx->SR1 & I2C_SR1_ADDR ))
	{
		if(sprawdz_blad_I2C(I2Cx))
		{
			//i2c_config();
			return;
		}
	}
	I2Cx->SR2; //konieczne wg datasheet, czyszczenie rejestru SR1

	I2Cx->CR1 |= I2C_CR1_ACK; //wlaczenie potwierdzenia master
	for(; dlugosc > 0; dlugosc-- )
	{
		if( dlugosc == 1 )
		I2Cx->CR1 &= ~I2C_CR1_ACK; //przy ostatnim wylaczenie potwierdzenia

		while( !( I2Cx->SR1 & I2C_SR1_RXNE ))
		{
		   if(sprawdz_blad_I2C(I2Cx))
		   {
			   //i2c_config();
			   return;
		   }
		}
		*(bufor++) = I2Cx->DR;
	}

	I2Cx->CR1 |= I2C_CR1_STOP;
}

int sprawdz_blad_I2C(I2C_TypeDef *I2Cx)
{
	if((I2Cx->SR1 & I2C_SR1_TIMEOUT) | (I2Cx->SR1 & I2C_SR1_PECERR) | (I2Cx->SR1 & I2C_SR1_AF) | (I2Cx->SR1 & I2C_SR1_ARLO) |
			(I2Cx->SR1 & I2C_SR1_BERR))
	{
		//sensors_error_flag = 1;
		return 1;
	}
	return 0;

}

void wyslij_I2C(I2C_TypeDef *I2Cx, uint8_t adres, uint8_t rejestr, uint8_t dane)
{
	while(I2Cx->SR2 & I2C_SR2_BUSY)
	{
		if(sprawdz_blad_I2C(I2Cx))
		{
			inicjalizacja_I2C();
			return;
		}
	}

	I2Cx->CR1 |= I2C_CR1_START;

	while(!(I2Cx->SR1 & I2C_SR1_SB))
	{
		if(sprawdz_blad_I2C(I2Cx))
		{
			inicjalizacja_I2C();
			return;
		}
	}

	I2Cx->SR1;
	I2Cx->DR = adres;
	while (!(I2Cx->SR1 & I2C_SR1_ADDR))
	{
		if(sprawdz_blad_I2C(I2Cx))
		{
			inicjalizacja_I2C();
			return;
		}
	}
	I2Cx->SR1;
	I2Cx->SR2;


		while (!(I2Cx->SR1 & I2C_SR1_TXE))
		{
			if(sprawdz_blad_I2C(I2Cx))
			{
				inicjalizacja_I2C();
				return;
			}
		}
		I2Cx->DR = rejestr;


		while (!(I2Cx->SR1 & I2C_SR1_TXE))
		{
			if(sprawdz_blad_I2C(I2Cx))
			{
				inicjalizacja_I2C();
				return;
			}
		}
		I2Cx->DR = dane;


	while (!(I2Cx->SR1 & I2C_SR1_TXE) || !(I2Cx->SR1 & I2C_SR1_BTF))
	{
		if(sprawdz_blad_I2C(I2Cx))
		{
			inicjalizacja_I2C();
			return;
		}
	}

	I2Cx->CR1 |= I2C_CR1_STOP;
}
