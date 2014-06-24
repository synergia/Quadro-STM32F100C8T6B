#include "sensory.h"
#include "I2C.h"
#include "dane.h"
#include "PID.h"

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
	wyslij_I2C(I2C2, AKCEL_ADR, 0x23, 0b00110000);
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

	uint16_t temp;
	signed int temp_vdeg;

	//kalibracja zyroskopu
	if(dane.zyro.zyro_kalibr_ktory < KALIBR + 10)
	{
		if (dane.zyro.zyro_kalibr_ktory >= 10)
		{
			temp = (dane.zyro.zyro_y_h << 8) + dane.zyro.zyro_y_l;
			//uwzgl�dnienie ujemnych
			if (temp > 32768)
				temp_vdeg = temp - 65535;
			else
				temp_vdeg = temp;

			dane.zyro.zyro_y_kalibracja += temp_vdeg;
			dane.zyro.zyro_y_kat_mdeg = 0;

			dane.zyro.zyro_kalibr_ktory++;
			if (dane.zyro.zyro_kalibr_ktory == (KALIBR + 10))
				dane.zyro.zyro_y_kalibracja = (dane.zyro.zyro_y_kalibracja >> KALIBR_PRZESUN);
		}
		else
			dane.zyro.zyro_kalibr_ktory++;
	}
	else
	{
		//uwzglednienie kalibracji
		if (dane.zyro.zyro_y_kalibracja > 32768)
			dane.zyro.zyro_y_kalibracja -= 65536;
		temp = dane.zyro.zyro_y_kalibracja;
		temp = (dane.zyro.zyro_y_h << 8) + dane.zyro.zyro_y_l - dane.zyro.zyro_y_kalibracja;
		dane.zyro.zyro_y_h = temp >> 8;
		dane.zyro.zyro_y_l = temp;


		//obliczenie kata z zyroskopu

		if (temp > 32768)
			temp_vdeg = temp - 65536;
		else
			temp_vdeg = temp;
		dane.zyro.zyro_y_kat_mdeg = temp_vdeg * DT * MDEG * 0.001; //w milistopniach


		temp = (dane.zyro.zyro_x_h << 8) + dane.zyro.zyro_x_l;
		if(temp > 32768)
			temp_vdeg = temp - 65536;
		else
			temp_vdeg = temp;
		dane.zyro.zyro_x_kat_mdeg = (temp_vdeg - DELTAZYROX) * DT * MDEG * 0.001; //w milistopniach
	}
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
			dane.temp += (dane.akcel.akcel_x_srednia_tab[i] - 255);
	}
	dane.akcel.akcel_x_srednia = dane.temp >> PRZESUN;

	dane.akcel.akcel_y_srednia_tab[dane.akcel.akcel_ktora_srednia] = dane.akcel.akcel_y_h;
	for(i = 0, dane.temp = 0; i < SREDNIA; i++)
	{
		if (dane.akcel.akcel_y_srednia_tab[i] < 127)
			dane.temp += (dane.akcel.akcel_y_srednia_tab[i]);
		else
			dane.temp += (dane.akcel.akcel_y_srednia_tab[i] - 255);
	}
	dane.akcel.akcel_y_srednia = dane.temp >> PRZESUN;

	dane.akcel.akcel_ktora_srednia++;
	//---------------------------------------------

	//oblicza kat -90 do 90 stopni
	uint16_t temp = (dane.akcel.akcel_x_h << 8) + dane.akcel.akcel_x_l;
	//uint16_t temp = (dane.akcel.akcel_x_srednia << 8);// + dane.akcel.akcel_x_l;
	signed int temp_deg;
	if (temp > 32768)
		temp_deg = temp - 65536;
	else
		temp_deg = temp;
	if (temp_deg < -1280)
		dane.akcel.akcel_x_kat_deg = -90; //skrajny przypadek
	else if (temp_deg > 1280)
		dane.akcel.akcel_x_kat_deg = 90; //skrajny przypadek
	else
		dane.akcel.akcel_x_kat_deg = (int)(asin((double)temp_deg/1280.0) *180.0 / PI);

	temp = (dane.akcel.akcel_y_srednia << 8);
	if (temp > 32768)
		temp_deg = temp -65536;
	else
		temp_deg = temp;
	if (temp_deg < -1280)
		dane.akcel.akcel_y_kat_deg = -90;
	else if (temp_deg > 1280)
		dane.akcel.akcel_y_kat_deg = 90;
	else
		dane.akcel.akcel_y_kat_deg = (int)(asin((double)temp_deg/1280.0) * 180.0 / PI);
	//----------------------------
}

void oblicz_kat()
{
	dane.kat.kat_x += dane.zyro.zyro_y_kat_mdeg;
	dane.kat.kat_x = 0.999*dane.kat.kat_x + 0.001*dane.akcel.akcel_x_kat_deg*1000;

	dane.kat.kat_y -= dane.zyro.zyro_x_kat_mdeg;
	dane.kat.kat_y = 0.99*dane.kat.kat_y + 0.01*dane.akcel.akcel_y_kat_deg*1000;
}

void odczyt_sensory()
{
	uint8_t bufor[6];
	odczyt_zyroskop(bufor);
	odczyt_akcelerometr(bufor);
	odczyt_magnetometr(bufor);

	oblicz_kat();

    if (dane.opoznienie > 250)
    {
    	PID();
    }
    else if (dane.opoznienie > 100)
    {
    	dane.pwm.pwm1 = 0;
    	dane.pwm.pwm2 = 0;
    	dane.pwm.pwm3 = 0;
    	dane.pwm.pwm4 = 0;
    	dane.kat.kat_x = 0;
    	dane.opoznienie++;
    }
    else
    {
    	dane.pwm.pwm1 = 10;
		dane.pwm.pwm2 = 10;
		dane.pwm.pwm3 = 10;
		dane.pwm.pwm4 = 10;
		dane.kat.kat_x = 0;
    	dane.opoznienie++;
    }
}

