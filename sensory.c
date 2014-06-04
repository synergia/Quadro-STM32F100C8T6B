#include "sensory.h"
#include "I2C.h"
#include "dane.h"

extern volatile daneTypeDef dane;

void inicjalizacja_sensory()
{
	inicjalizacja_zyroskop();
}

void inicjalizacja_zyroskop()
{
	wyslij_I2C(I2C2, ZYRO_ADR, 0x20, 0b11111111);  //wlaczony zyroskop
	wyslij_I2C(I2C2, ZYRO_ADR, 0x21, 0b00100000); //filtry
	wyslij_I2C(I2C2, ZYRO_ADR, 0x22, 0b00000000);// poki co pin zle poprowadzony wiec niepotrzebne, jakis dziwny
	wyslij_I2C(I2C2, ZYRO_ADR, 0x23, 0b00000000); //250 dps
	wyslij_I2C(I2C2, ZYRO_ADR, 0x24, 0b00000000); //fifo, filtr wylaczone
}

void odczyt_zyroskop(uint8_t *bufor)
{
	/*odczyt z zyroskopu
	* ------------------------
	*/
	odczyt_I2C(I2C2, ZYRO_ADR,0xA8,2,bufor);
	dane.zyro.zyro_x_l = bufor[0];
	dane.zyro.zyro_x_h = bufor[1];
	//--------------------------
}

void odczyt_sensory()
{
	uint8_t bufor[2];
	odczyt_zyroskop(bufor);
}

void TIM1_UP_TIM16_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET) //sprawdzenie zrodla
	{
		TIM_ClearFlag(TIM1, TIM_FLAG_Update); //wyzerowanie flagi przerwania

		//odebranie danych
		odczyt_sensory();
	}
}

