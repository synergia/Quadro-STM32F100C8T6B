#include "dane.h"

volatile daneTypeDef dane;

void inicjalizacja_dane()
{
	dane.pwm.timer = 0;
	dane.pwm.wartosc_pocz = 0;
	dane.pwm.pwm1 = 1;
	dane.pwm.pwm2 = 1;
	dane.pwm.pwm3 = 1;
	dane.pwm.pwm4 = 1;
	dane.pwm.licznik = 0;

	dane.zyro.zyro_x_h = 0;
	dane.zyro.zyro_x_l = 0;
	dane.zyro.zyro_y_h = 0;
	dane.zyro.zyro_y_l = 0;
	dane.zyro.zyro_z_h = 0;
	dane.zyro.zyro_z_l = 0;
	dane.zyro.zyro_y_kat_mdeg = 0;
	dane.zyro.zyro_x_kat_mdeg = 0;
	dane.zyro.zyro_y_kalibracja = 0;
	dane.zyro.zyro_kalibr_ktory = 0;
	
	dane.akcel.akcel_x_h = 0;
	dane.akcel.akcel_x_l = 0;
	dane.akcel.akcel_y_h = 0;
	dane.akcel.akcel_y_l = 0;
	dane.akcel.akcel_z_h = 0;
	dane.akcel.akcel_z_l = 0;
	dane.akcel.akcel_x_kat_deg = 0;
	dane.akcel.akcel_y_kat_deg = 0;
	dane.akcel.akcel_ktora_srednia = 0;
	dane.akcel.akcel_x_srednia = 0;
	dane.akcel.akcel_y_srednia = 0;
	dane.akcel.akcel_x_suma_prosta = 0;
	dane.akcel.akcel_y_suma_prosta = 0;

	dane.kat.kat_x = 0;
	dane.kat.kat_y = 0;

	dane.magnet.magnet_x_h = 0;
	dane.magnet.magnet_x_l = 0;
	dane.magnet.magnet_y_h = 0;
	dane.magnet.magnet_y_l = 0;
	dane.magnet.magnet_z_h = 0;
	dane.magnet.magnet_z_l = 0;

	dane.pid.kP = 1;
	dane.pid.kI = 1;
	dane.pid.kD = 1;

	int i;
	for (i = 0; i < BLEDY; i++)
	{
		dane.pid.I_x_bledy_tab[i] = 0;
		dane.pid.I_y_bledy_tab[i] = 0;
	}
	dane.pid.I_x_bledy_srednia = 0;
	dane.pid.I_y_bledy_srednia = 0;
	dane.pid.I_ktory_blad = 0;

	dane.bateria.poziom = 0;
	dane.bateria.ktora = 0;

	dane.czy_polaczony = 0;
	dane.opoznienie = 0;

	//Kalman
	dane.kalman_x.K[0] = 0;
	dane.kalman_x.K[1] = 0;

	dane.kalman_x.P[0][0] = 0;
	dane.kalman_x.P[0][1] = 0;
	dane.kalman_x.P[1][0] = 0;
	dane.kalman_x.P[1][1] = 0;

	dane.kalman_x.Q_angle = 0.001;
	dane.kalman_x.Q_bias = 0.003;
	dane.kalman_x.R_measure = 10.0;
	dane.kalman_x.S = 0;
	dane.kalman_x.angle = 0;
	dane.kalman_x.bias = 0;
	dane.kalman_x.rate = 0;
	dane.kalman_x.y = 0;
}
