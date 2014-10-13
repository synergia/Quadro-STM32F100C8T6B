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
	inicjalizacja_barometr();
}

void inicjalizacja_zyroskop()
{
	wyslij_I2C(I2C2, ZYRO_ADR, 0x20, 0b00001111);  //wlaczony zyroskop
	wyslij_I2C(I2C2, ZYRO_ADR, 0x21, 0b00000000); //filtry
	wyslij_I2C(I2C2, ZYRO_ADR, 0x22, 0b00000000);
	wyslij_I2C(I2C2, ZYRO_ADR, 0x23, 0b00000000); //250 dps
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
	wyslij_I2C(I2C2, AKCEL_ADR, 0x23, 0b10000000);
}

void inicjalizacja_barometr()
{
	wyslij_I2C(I2C2, BARO_ADR, 0x20, 0b11000100);
	wyslij_I2C(I2C2, BARO_ADR, 0x10, 0x7A);
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
			// Y
			temp = (dane.zyro.zyro_y_h << 8) + dane.zyro.zyro_y_l;
			//uwzglednienie ujemnych
			if (temp > 32768)
				temp_vdeg = temp - 65536;
			else
				temp_vdeg = temp;

			dane.zyro.zyro_y_kalibracja += temp_vdeg;
			dane.zyro.zyro_y_kat_mdeg = 0;

			// Z
			temp = (dane.zyro.zyro_z_h << 8) + dane.zyro.zyro_z_l;
			if (temp > 32768)
				temp_vdeg = temp - 65536;
			else
				temp_vdeg = temp;

			dane.zyro.zyro_z_kalibracja += temp_vdeg;
			dane.zyro.zyro_z_kat_mdeg = 0;

			// X
			temp = (dane.zyro.zyro_x_h << 8) + dane.zyro.zyro_z_l;
			if (temp > 32768)
				temp_vdeg = temp - 65536;
			else
				temp_vdeg = temp;

			dane.zyro.zyro_x_kalibracja += temp_vdeg;
			dane.zyro.zyro_x_kat_mdeg = 0;

			dane.zyro.zyro_kalibr_ktory++;
			if (dane.zyro.zyro_kalibr_ktory == (KALIBR + 10))
			{
				dane.zyro.zyro_y_kalibracja = (dane.zyro.zyro_y_kalibracja >> KALIBR_PRZESUN);
				dane.zyro.zyro_z_kalibracja = (dane.zyro.zyro_z_kalibracja >> KALIBR_PRZESUN);
				dane.zyro.zyro_x_kalibracja = (dane.zyro.zyro_x_kalibracja >> KALIBR_PRZESUN);
			}
		}
		else
			dane.zyro.zyro_kalibr_ktory++;
	}
	else
	{
		// Y --------------------
		//uwzglednienie kalibracji
		if (dane.zyro.zyro_y_kalibracja > 32768)
			dane.zyro.zyro_y_kalibracja -= 65536;
		temp = (dane.zyro.zyro_y_h << 8) + dane.zyro.zyro_y_l - dane.zyro.zyro_y_kalibracja;
		dane.zyro.zyro_y_h = temp >> 8;
		dane.zyro.zyro_y_l = temp;

		//obliczenie kata z zyroskopu

		if (temp > 32768)
			temp_vdeg = temp - 65536;
		else
			temp_vdeg = temp;
		dane.zyro.zyro_y_deg_sec = temp_vdeg * MDEG * 0.001; //w stopniach na sekunde
		dane.zyro.zyro_y_kat_mdeg = temp_vdeg * DT * MDEG; //w milistopniach

		// ------------------------

		// Z ----------------------
		if (dane.zyro.zyro_z_kalibracja > 32768)
			dane.zyro.zyro_z_kalibracja -= 65536;
		temp = (dane.zyro.zyro_z_h << 8) + dane.zyro.zyro_z_l - dane.zyro.zyro_z_kalibracja;
		dane.zyro.zyro_z_h = temp >> 8;
		dane.zyro.zyro_z_l = temp;

		if (temp > 32768)
			temp_vdeg = temp - 65536;
		else temp_vdeg = temp;
		dane.zyro.zyro_z_deg_sec = temp_vdeg * MDEG * 0.001;
		dane.zyro.zyro_z_kat_mdeg += temp_vdeg * DT * MDEG;

		// -------------------------

		// X -----------------------
		if (dane.zyro.zyro_x_kalibracja > 32768)
			dane.zyro.zyro_x_kalibracja -= 65536;
		temp = (dane.zyro.zyro_x_h << 8) + dane.zyro.zyro_x_l - dane.zyro.zyro_x_kalibracja;
		dane.zyro.zyro_x_h = temp >> 8;
		dane.zyro.zyro_x_l = temp;

		if(temp > 32768)
			temp_vdeg = temp - 65536;
		else
			temp_vdeg = temp;
		dane.zyro.zyro_x_deg_sec = temp_vdeg * MDEG * 0.001; //w stopniach na sekunde
		dane.zyro.zyro_x_kat_mdeg = temp_vdeg * DT * MDEG; //w milistopniach
		// -------------------------
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
	int16_t temp_deg = dane.akcel.akcel_x_srednia << 8;
	//int16_t temp_deg = (dane.akcel.akcel_x_h << 8) + dane.akcel.akcel_x_l;
	if (temp_deg < -AKC_SKALA)
		dane.akcel.akcel_x_kat_rad = -1.57*10000; //skrajny przypadek
	else if (temp_deg > AKC_SKALA)
		dane.akcel.akcel_x_kat_rad = 1.57*10000; //skrajny przypadek
	else
		dane.akcel.akcel_x_kat_rad = (int)(temp_deg/AKC_SKALA * 10000);

	temp_deg = dane.akcel.akcel_y_srednia << 8;
	//temp_deg = (dane.akcel.akcel_y_h << 8) + dane.akcel.akcel_y_l;
	if (temp_deg < -AKC_SKALA)
		dane.akcel.akcel_y_kat_rad = -1.57*10000;
	else if (temp_deg > AKC_SKALA)
		dane.akcel.akcel_y_kat_rad = 1.57*10000;
	else
		dane.akcel.akcel_y_kat_rad = (int)(temp_deg/AKC_SKALA * 10000);
	//----------------------------
}

void odczyt_barometr(uint8_t *bufor)
{
	odczyt_I2C(I2C2, BARO_ADR, 0xA8, 5, bufor);
	dane.baro.press_out_xl = bufor[0];
	dane.baro.press_out_l = bufor[1];
	dane.baro.press_out_h = bufor[2];
	dane.baro.temp_out_l = bufor[3];
	dane.baro.temp_out_h = bufor[4];

	int16_t temp = (dane.baro.temp_out_h << 8) + dane.baro.temp_out_l;

	dane.baro.temp_celsius = (uint8_t)(42.5 + temp * 0.002083); // strona 29

	//kalibracja barometru
	if (dane.baro.ktory < KALIBR)
			dane.baro.ktory++;
	else if (dane.baro.ktory == KALIBR)
	{
		wyslij_I2C(I2C2, BARO_ADR, 0x21, 0b00000010);
		dane.baro.ktory++;
	}
	else
		dane.baro.press_mbar = ((dane.baro.press_out_h << 24) + (dane.baro.press_out_l << 16) + dane.baro.press_out_xl)>>12;

}

void oblicz_kat()
{
	//kalman_x(dane.akcel.akcel_x_kat_deg, dane.zyro.zyro_y_deg_sec, DT);
	int32_t temp_rad;
	temp_rad = (double)(dane.zyro.zyro_y_kat_mdeg) * 0.1745; // milistopnie do radianow i jeszcze razy 10000
	dane.kat.kat_x += temp_rad;
	dane.kat.kat_x = 0.98*dane.kat.kat_x + 0.02*dane.akcel.akcel_x_kat_rad;

	temp_rad = (double)(dane.zyro.zyro_x_kat_mdeg) * 0.1745;
	dane.kat.kat_y -= temp_rad;
	dane.kat.kat_y = 0.98*dane.kat.kat_y + 0.02*dane.akcel.akcel_y_kat_rad;
}

void kalman_x(double newAngle, double newRate, double dt)
{
	// KasBot V2 - Kalman filter module - http://www.x-firm.com/?page_id=145
	// Modified by Kristian Lauszus
	// See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

	// Discrete Kalman filter time update equations - Time Update ("Predict")
	// Update xhat - Project the state ahead
	/* Step 1 */
	dane.kalman_x.rate = newRate - dane.kalman_x.bias;
	dane.kalman_x.angle += dt * dane.kalman_x.rate;

	// Update estimation error covariance - Project the error covariance ahead
	/* Step 2 */
	dane.kalman_x.P[0][0] += dt * (dt*dane.kalman_x.P[1][1] - dane.kalman_x.P[0][1] - dane.kalman_x.P[1][0] + dane.kalman_x.Q_angle);
	dane.kalman_x.P[0][1] -= dt * dane.kalman_x.P[1][1];
	dane.kalman_x.P[1][0] -= dt * dane.kalman_x.P[1][1];
	dane.kalman_x.P[1][1] += dane.kalman_x.Q_bias * dt;

	// Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
	// Calculate Kalman gain - Compute the Kalman gain
	/* Step 4 */
	dane.kalman_x.S = dane.kalman_x.P[0][0] + dane.kalman_x.R_measure;
	/* Step 5 */
	dane.kalman_x.K[0] = dane.kalman_x.P[0][0] / dane.kalman_x.S;
	dane.kalman_x.K[1] = dane.kalman_x.P[1][0] / dane.kalman_x.S;

	// Calculate angle and bias - Update estimate with measurement zk (newAngle)
	/* Step 3 */
	dane.kalman_x.y = newAngle - dane.kalman_x.angle;
	/* Step 6 */
	dane.kalman_x.angle += dane.kalman_x.K[0] * dane.kalman_x.y;
	dane.kalman_x.bias += dane.kalman_x.K[1] * dane.kalman_x.y;

	// Calculate estimation error covariance - Update the error covariance
	/* Step 7 */
	dane.kalman_x.P[0][0] -= dane.kalman_x.K[0] * dane.kalman_x.P[0][0];
	dane.kalman_x.P[0][1] -= dane.kalman_x.K[0] * dane.kalman_x.P[0][1];
	dane.kalman_x.P[1][0] -= dane.kalman_x.K[1] * dane.kalman_x.P[0][0];
	dane.kalman_x.P[1][1] -= dane.kalman_x.K[1] * dane.kalman_x.P[0][1];
}

void odczyt_sensory()
{
	uint8_t bufor[6];
	odczyt_zyroskop(bufor);
	odczyt_akcelerometr(bufor);
	//odczyt_magnetometr(bufor);
	//odczyt_barometr(bufor);

	oblicz_kat();

    if (dane.opoznienie > 250)
    {
    	//PID();
    	/*dane.pwm.pwm1 = 50;
    	dane.pwm.pwm2 = 50;
    	dane.pwm.pwm3 = 50;
    	dane.pwm.pwm4 = 50;*/
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
    	dane.pwm.pwm1 = 0;
		dane.pwm.pwm2 = 0;
		dane.pwm.pwm3 = 0;
		dane.pwm.pwm4 = 0;
		dane.kat.kat_x = 0;
    	dane.opoznienie++;
    }
}

