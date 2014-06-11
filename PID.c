#include "naglowki_include.h"
#include "sensory.h"
#include "dane.h"

extern volatile daneTypeDef dane;

void PID()
{
	//obliczanie bledu D
	double kD = (double)dane.pid.kD * 0.00005;
	signed int kD_temp_x, kD_temp_y;
	if (dane.pid.I_ktory_blad == 0)
	{
		kD_temp_x = dane.kat.kat_x - dane.pid.I_x_bledy_tab[BLEDY-1];
		kD_temp_y = dane.kat.kat_y - dane.pid.I_y_bledy_tab[BLEDY-1];
	}
	else
	{
		kD_temp_x = dane.kat.kat_x - dane.pid.I_x_bledy_tab[dane.pid.I_ktory_blad - 1];
		kD_temp_y = dane.kat.kat_y - dane.pid.I_y_bledy_tab[dane.pid.I_ktory_blad - 1];
	}


	//obliczanie bledu I
	//----------------------
	double kI = (double)dane.pid.kI * 0.00005;
	int i;
	//dolaczanie aktualnego bledu do tablicy
	dane.pid.I_x_bledy_tab[dane.pid.I_ktory_blad] = dane.kat.kat_x;
	//obliczanie sredniej
	for (i = 0, dane.pid.I_x_bledy_srednia = 0; i < BLEDY; i++)
		dane.pid.I_x_bledy_srednia += dane.pid.I_x_bledy_tab[i];
	dane.pid.I_x_bledy_srednia = dane.pid.I_x_bledy_srednia >> PRZESUN;

	//dolaczanie aktualnego bledu do tablicy
	dane.pid.I_y_bledy_tab[dane.pid.I_ktory_blad] = dane.kat.kat_y;

	dane.pid.I_ktory_blad++;
	if (dane.pid.I_ktory_blad >= BLEDY)
		dane.pid.I_ktory_blad = 0;

	//obliczanie sredniej
	for (i = 0, dane.pid.I_y_bledy_srednia = 0; i < BLEDY; i++)
		dane.pid.I_y_bledy_srednia += dane.pid.I_y_bledy_tab[i];
	dane.pid.I_y_bledy_srednia = dane.pid.I_y_bledy_srednia >> PRZESUN;
	//-----------------------

	//dolaczenie wspolczynnika P
	double kP = (double)dane.pid.kP * 0.00005;

	//obliczenie dodania do garow silniki 2 i 4
	int temp = (double)dane.kat.kat_x * kP + (double)dane.pid.I_x_bledy_srednia * kI + (double)kD_temp_x * kD;

	if (temp <= -dane.pwm.wartosc_pocz)
		dane.pwm.pwm2 = 1;
	else
		dane.pwm.pwm2 = dane.pwm.wartosc_pocz + temp;
	if(dane.pwm.pwm2 > 100) dane.pwm.pwm2 = 100;

	if (temp >= dane.pwm.wartosc_pocz)
		dane.pwm.pwm4 = 1;
	else dane.pwm.pwm4 = dane.pwm.wartosc_pocz - temp;

	if(dane.pwm.pwm4 > 100) dane.pwm.pwm4 = 100;

	//silniki 1 i 3
	temp = (double)dane.kat.kat_y * kP + (double)dane.pid.I_y_bledy_srednia * kI + (double)kD_temp_y * kD;

	if (temp <= -dane.pwm.wartosc_pocz)
		dane.pwm.pwm1 = 1;
	else
		dane.pwm.pwm1 = dane.pwm.wartosc_pocz + temp;
	if(dane.pwm.pwm1 > 100) dane.pwm.pwm1 = 100;

	if (temp >= dane.pwm.wartosc_pocz)
		dane.pwm.pwm3 = 1;
	else dane.pwm.pwm3 = dane.pwm.wartosc_pocz - temp;

	if(dane.pwm.pwm3 > 100) dane.pwm.pwm3 = 100;

}
