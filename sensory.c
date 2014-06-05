#include "sensory.h"
#include "I2C.h"
#include "dane.h"
#include "PID.h"

extern volatile daneTypeDef dane;

void inicjalizacja_sensory()
{
	inicjalizacja_zyroskop();
	inicjalizacja_magnetometr();
}

void inicjalizacja_zyroskop()
{
	wyslij_I2C(I2C2, ZYRO_ADR, 0x20, 0b11111111);  //wlaczony zyroskop
	wyslij_I2C(I2C2, ZYRO_ADR, 0x21, 0b00100000); //filtry
	wyslij_I2C(I2C2, ZYRO_ADR, 0x22, 0b00000000);
	wyslij_I2C(I2C2, ZYRO_ADR, 0x23, 0b00000000); //250 dps
	wyslij_I2C(I2C2, ZYRO_ADR, 0x24, 0b00000000); //fifo, filtr wylaczone
}

void inicjalizacja_magnetometr()
{
	wyslij_I2C(I2C2, MAGNET_ADR, 0x00, 0b00011000); //75 Hz, normal measure
	wyslij_I2C(I2C2, MAGNET_ADR, 0x01, 0b11100000); //najmniejsza czulosc
	wyslij_I2C(I2C2, MAGNET_ADR, 0x02, 0b00000000); //tryb ciagly
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

void odczyt_magnetometr(uint8_t *bufor)
{
	odczyt_I2C(I2C2, MAGNET_ADR, 0x83,6,bufor);
	dane.magnet.magnet_x_l = bufor[0];
	dane.magnet.magnet_x_h = bufor[1];

	/*
	 * przy poziomie magnetometr daje okolo 18
	 * */

}

void odczyt_sensory()
{
	uint8_t bufor[6];
	//odczyt_zyroskop(bufor); na razie niepotrzebny
	odczyt_magnetometr(bufor);

    if (dane.opoznienie > 2000)
    	PID();
    else if (dane.opoznienie > 1000)
    {
    	dane.pwm.pwm1 = 1;
    	dane.pwm.pwm2 = 1;
    	dane.pwm.pwm3 = 1;
    	dane.pwm.pwm4 = 1;
    	dane.opoznienie++;
    }
    else
    {
    	dane.pwm.pwm1 = 50;
		dane.pwm.pwm2 = 50;
		dane.pwm.pwm3 = 50;
		dane.pwm.pwm4 = 50;
    	dane.opoznienie++;
    }
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

