#include "sensory.h"
#include "I2C.h"
#include "dane.h"
#include "PID.h"

#define PI 3.14159265

extern volatile daneTypeDef dane;

void inicjalizacja_sensory()
{
	inicjalizacja_zyroskop();
	inicjalizacja_magnetometr();
	inicjalizacja_akcelerometr();
}

void inicjalizacja_zyroskop()
{
	wyslij_I2C(I2C2, ZYRO_ADR, 0x20, 0b00001111);  //wlaczony zyroskop
	wyslij_I2C(I2C2, ZYRO_ADR, 0x21, 0b00000000); //filtry
	wyslij_I2C(I2C2, ZYRO_ADR, 0x22, 0b00000000);
	wyslij_I2C(I2C2, ZYRO_ADR, 0x23, 0b10000000); //250 dps
	wyslij_I2C(I2C2, ZYRO_ADR, 0x24, 0b00000000);
}

void inicjalizacja_magnetometr()
{
	wyslij_I2C(I2C2, MAGNET_ADR, 0x00, 0b00011000); //75 Hz, normal measure
	wyslij_I2C(I2C2, MAGNET_ADR, 0x01, 0b11100000); //najmniejsza czulosc
	wyslij_I2C(I2C2, MAGNET_ADR, 0x02, 0b00000000); //tryb ciagly
}

void inicjalizacja_akcelerometr()
{
	wyslij_I2C(I2C2, AKCEL_ADR, 0x20, 0b00100111);
	wyslij_I2C(I2C2, AKCEL_ADR, 0x21, 0b00000000);
	wyslij_I2C(I2C2, AKCEL_ADR, 0x23, 0b00010000);
}

void odczyt_zyroskop(uint8_t *bufor)
{
	/*odczyt z zyroskopu
	* ------------------------
	*/
	odczyt_I2C(I2C2, ZYRO_ADR,0xA8,6,bufor);
	dane.zyro.zyro_x_l = bufor[0];
	dane.zyro.zyro_x_h = bufor[1];
	dane.zyro.zyro_y_l = bufor[2];
	dane.zyro.zyro_y_h = bufor[3];
	dane.zyro.zyro_z_l = bufor[4];
	dane.zyro.zyro_z_h = bufor[5];
	//--------------------------
}

void odczyt_magnetometr(uint8_t *bufor)
{
	odczyt_I2C(I2C2, MAGNET_ADR, 0x83,6,bufor);
	dane.magnet.magnet_x_l = bufor[0];
	dane.magnet.magnet_x_h = bufor[1];

	dane.magnet.magnet_y_l = bufor[2];
	dane.magnet.magnet_y_h = bufor[3];

	dane.magnet.magnet_z_l = bufor[4];
	dane.magnet.magnet_z_h = bufor[5];


}

void odczyt_akcelerometr(uint8_t *bufor)
{
	odczyt_I2C(I2C2, AKCEL_ADR, 0xA8, 6, bufor);
	dane.akcel.akcel_x_l = bufor[0];
	dane.akcel.akcel_x_h = bufor[1];
	dane.akcel.akcel_y_l = bufor[2];
	dane.akcel.akcel_y_h = bufor[3];
	dane.akcel.akcel_z_l = bufor[4];
	dane.akcel.akcel_z_h = bufor[5];

	//oblicza kat -90 do 90 stopni
	uint16_t temp = (dane.akcel.akcel_x_h << 8) + dane.akcel.akcel_x_l;
	signed int temp_deg;
	if (temp > 32768)
		temp_deg = temp - 65535;
	else
		temp_deg = temp;
	dane.akcel.akcel_x_kat_deg = (int)(asin((double)temp_deg/8000.0) *180.0 / PI);
	//----------------------------

	/*
	//obliczanie sredniej
	//--------------------------------------------
	if (dane.akcel.akcel_ktora_srednia >= SREDNIA)
		dane.akcel.akcel_ktora_srednia = 0;
	dane.akcel.akcel_x_srednia_tab[dane.akcel.akcel_ktora_srednia] = dane.akcel.akcel_x_h;
	int i;
	for(i=0, dane.temp = 0; i < SREDNIA; i++)
	{
		if(dane.akcel.akcel_x_srednia_tab[i] < 127)
			dane.temp += dane.akcel.akcel_x_srednia_tab[i];
		else
			dane.temp -= (dane.akcel.akcel_x_srednia_tab[i] - 255);
	}
	dane.akcel.akcel_x_srednia = dane.temp >> PRZESUN;
	//---------------------------------------------

	//filtr mediany
	//---------------------------------------------
	uint8_t temp[3];
	uint8_t temp2;
	uint8_t czy_zmieniany;

	temp[0] = dane.akcel.akcel_x_mediana_tab[0];
	temp[1] = dane.akcel.akcel_x_mediana_tab[1];
	if (dane.akcel.akcel_x_h > 127)
		temp[2] = dane.akcel.akcel_x_mediana_tab[2] = 255 - dane.akcel.akcel_x_h;
	else
		temp[2] = dane.akcel.akcel_x_mediana_tab[2] = dane.akcel.akcel_x_h;

	//sortowanie babelkowe
	do
	{
		czy_zmieniany = 0;
		for (i = 0; i < 2; i++)
		{
			if (temp[i+1] < temp[i])
			{
				temp2 = temp[i];
				temp[i] = temp[i+1];
				temp[i+1] = temp2;
				czy_zmieniany = 1;
			}
		}
	} while (czy_zmieniany);

	dane.akcel.akcel_x_mediana = temp[1];

	dane.akcel.akcel_x_mediana_tab[0] = dane.akcel.akcel_x_mediana_tab[1];
	dane.akcel.akcel_x_mediana_tab[1] = dane.akcel.akcel_x_mediana_tab[2];
	dane.akcel.akcel_x_mediana_tab[2] = dane.akcel.akcel_x_mediana;
	//---------------------------------------------

	//filtr srednia z mediany
	//--------------------------------------------
	dane.akcel.akcel_x_srednia_mediana_tab[dane.akcel.akcel_ktora_srednia] = dane.akcel.akcel_x_mediana;
	for(i=0, dane.temp = 0; i < SREDNIA; i++)
		dane.temp += dane.akcel.akcel_x_srednia_mediana_tab[i];
	dane.akcel.akcel_x_srednia_mediana = dane.temp >> PRZESUN;
	//---------------------------------------------

	dane.akcel.akcel_ktora_srednia++;
	*/
}

void odczyt_sensory()
{
	uint8_t bufor[6];
	odczyt_zyroskop(bufor);
	odczyt_akcelerometr(bufor);
	odczyt_magnetometr(bufor);

    if (dane.opoznienie > 250)
    	PID();
    else if (dane.opoznienie > 100)
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

