#include "dane.h"

volatile daneTypeDef dane;

void inicjalizacja_dane()
{
	dane.pwm.timer = 0;
	dane.pwm.pwm1 = 0;
	dane.pwm.pwm2 = 0;
	dane.pwm.pwm3 = 0;
	dane.pwm.pwm4 = 0;

	dane.zyro.zyro_x_h = 0;
	dane.zyro.zyro_x_l = 0;
	
	dane.magnet.magnet_x_h = 0;
	dane.magnet.magnet_x_l = 0;

	dane.czy_polaczony = 0;
}
