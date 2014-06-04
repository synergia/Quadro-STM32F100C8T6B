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
	uint8_t zyro_x_l;
	uint8_t zyro_x_h;
} ZyroTypeDef;

typedef struct
{
	uint8_t magnet_x_l;
	uint8_t magnet_x_h;
} MagnetTypeDef;

typedef struct
{
	PWMTypeDef pwm; //struktura PWM
	ZyroTypeDef zyro;
	MagnetTypeDef magnet;

	uint8_t czy_polaczony; //zeby awaryjnie wylaczyc (systick)
} daneTypeDef;


void inicjalizacja_dane();
