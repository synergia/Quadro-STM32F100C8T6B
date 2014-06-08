#include "naglowki_include.h"

#ifndef _DANE_H_
#define _DANE_H_

#define SREDNIA 32
#define PRZESUN 5

#endif

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
	uint8_t akcel_x_l;
	uint8_t akcel_x_h;
	uint8_t akcel_y_l;
	uint8_t akcel_y_h;
	uint8_t akcel_z_l;
	uint8_t akcel_z_h;

	signed int akcel_x_kat_deg;


	/*uint8_t akcel_ktora_srednia;
	uint8_t akcel_x_srednia;
	uint8_t akcel_x_srednia_tab[SREDNIA];

	uint8_t akcel_x_srednia_mediana;
	uint8_t akcel_x_srednia_mediana_tab[SREDNIA];

	uint8_t akcel_x_mediana; //do filtru mediany
	uint8_t akcel_x_mediana_tab[3];*/
} AkcelTypeDef;

typedef struct
{
	uint8_t zyro_x_l;
	uint8_t zyro_x_h;
	uint8_t zyro_y_l;
	uint8_t zyro_y_h;
	uint8_t zyro_z_l;
	uint8_t zyro_z_h;
} ZyroTypeDef;

typedef struct
{
	uint8_t magnet_x_l;
	uint8_t magnet_x_h;
	uint8_t magnet_y_l;
	uint8_t magnet_y_h;
	uint8_t magnet_z_l;
	uint8_t magnet_z_h;
} MagnetTypeDef;

typedef struct
{
	uint8_t kP; //wspolczynnik regulacji P
} PIDTypeDef;

typedef struct
{
	PWMTypeDef pwm; //struktura PWM
	ZyroTypeDef zyro;
	MagnetTypeDef magnet;
	AkcelTypeDef akcel;
	PIDTypeDef pid;

	uint16_t temp;
	uint8_t czy_polaczony; //zeby awaryjnie wylaczyc (systick)
	uint8_t opoznienie; //¿eby zainicjalizowaæ silniki oraz dla bezpieczeñstwa
} daneTypeDef;


void inicjalizacja_dane();
