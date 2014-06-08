#include "dane.h"

volatile daneTypeDef dane;

void inicjalizacja_dane()
{
	dane.pwm.timer = 0;
	dane.pwm.pwm1 = 1;
	dane.pwm.pwm2 = 1;
	dane.pwm.pwm3 = 1;
	dane.pwm.pwm4 = 1;

	dane.zyro.zyro_x_h = 0;
	dane.zyro.zyro_x_l = 0;
	dane.zyro.zyro_y_h = 0;
	dane.zyro.zyro_y_l = 0;
	dane.zyro.zyro_z_h = 0;
	dane.zyro.zyro_z_l = 0;
	
	dane.akcel.akcel_x_h = 0;
	dane.akcel.akcel_x_l = 0;
	dane.akcel.akcel_y_h = 0;
	dane.akcel.akcel_y_l = 0;
	dane.akcel.akcel_z_h = 0;
	dane.akcel.akcel_z_l = 0;
	dane.akcel.akcel_x_kat_deg = 0;

	/*int i;
	for (i = 0; i < SREDNIA; i++)
		dane.akcel.akcel_x_srednia_tab[i] = 0;
	dane.akcel.akcel_x_srednia = 0;
	dane.akcel.akcel_ktora_srednia = 0;*/

	dane.magnet.magnet_x_h = 0;
	dane.magnet.magnet_x_l = 0;
	dane.magnet.magnet_y_h = 0;
	dane.magnet.magnet_y_l = 0;
	dane.magnet.magnet_z_h = 0;
	dane.magnet.magnet_z_l = 0;

	dane.pid.kP = 1;

	dane.czy_polaczony = 0;
	dane.opoznienie = 0;
}
