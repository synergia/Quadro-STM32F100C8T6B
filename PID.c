#include "naglowki_include.h"
#include "sensory.h"
#include "dane.h"

extern volatile daneTypeDef dane;

void PID()
{
	double kP = (double)dane.pid.kP * 0.0001;
	int temp = (double)dane.kat.kat_x * kP;

	dane.pwm.pwm4 = 25 + temp;

	if(dane.pwm.pwm4 > 100) dane.pwm.pwm4 = 100;
	else if (dane.pwm.pwm4 < 1) dane.pwm.pwm4 = 1;

	dane.pwm.pwm2 = 25 - temp;

	if(dane.pwm.pwm2 > 100) dane.pwm.pwm2 = 100;
	else if (dane.pwm.pwm2 < 1) dane.pwm.pwm2 = 1;

}
