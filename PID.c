#include "naglowki_include.h"
#include "sensory.h"
#include "dane.h"

extern volatile daneTypeDef dane;

void PID()
{
	double temp;

	if (dane.magnet.magnet_x_l == 255)
		temp = dane.magnet.magnet_x_h - 255 + 23;
	else
		temp = dane.magnet.magnet_x_h + 23;

	temp *= dane.pid.kP;
	dane.pwm.pwm4 -= temp;

	if(dane.pwm.pwm4 > 100) dane.pwm.pwm4 = 100;
	else if (dane.pwm.pwm4 < 1) dane.pwm.pwm4 = 1;

	dane.pwm.pwm2 += temp;

	if(dane.pwm.pwm2 > 100) dane.pwm.pwm2 = 100;
	else if (dane.pwm.pwm2 < 1) dane.pwm.pwm2 = 1;
}
