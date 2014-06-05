#include "naglowki_include.h"
#include "sensory.h"
#include "dane.h"

extern volatile daneTypeDef dane;

void PID()
{
	int temp2;
	double temp;
	if (dane.magnet.magnet_x_l == 255)
		temp = dane.magnet.magnet_x_h - 18 - 255;
	else
		temp = dane.magnet.magnet_x_h - 18;

	dane.pwm.pwm2 += dane.pid.kP * temp;
	if (dane.pid.kP * temp > dane.pwm.pwm4)
		dane.pwm.pwm4 = 0;
	else dane.pwm.pwm4 -= dane.pid.kP * temp;
	temp2 = dane.pwm.pwm4;
}
