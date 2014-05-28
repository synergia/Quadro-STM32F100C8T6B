#include "naglowki_include.h"

typedef struct
{
	uint8_t timer;
	uint8_t pwm1;
	uint8_t pwm2;
	uint8_t pwm3;
	uint8_t pwm4;
} PWMTypeDef;

typedef struct
{
	PWMTypeDef pwm; //struktura PWM
} daneTypeDef;


void inicjalizacja_dane();
