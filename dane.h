#include "naglowki_include.h"

#ifndef _DANE_H_
#define _DANE_H_

#define SREDNIA 64
#define PRZESUN 6
#define BLEDY 64 //ilosc bledow gromadzonych w I

#endif

typedef struct
{
	uint8_t timer;
	uint8_t wartosc_pocz;
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
	signed int akcel_y_kat_deg;

	uint8_t akcel_ktora_srednia;
	uint8_t akcel_x_srednia;
	uint8_t akcel_x_srednia_tab[SREDNIA];
	uint8_t akcel_y_srednia;
	uint8_t akcel_y_srednia_tab[SREDNIA];
} AkcelTypeDef;

typedef struct
{
	uint8_t zyro_x_l;
	uint8_t zyro_x_h;
	uint8_t zyro_y_l;
	uint8_t zyro_y_h;
	uint8_t zyro_z_l;
	uint8_t zyro_z_h;
	uint16_t zyro_kalibr_ktory;

	int32_t zyro_y_kat_mdeg;
	int32_t zyro_x_kat_mdeg;

	int32_t zyro_y_kalibracja; //sluzy do kalibrowania odczytow, przez pierwsze 256 odczytow quadro MUSI sta�
} ZyroTypeDef;

typedef struct
{
	int32_t kat_x;
	int32_t kat_y;
} KatTypeDef;

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
	uint8_t kI;
	uint8_t kD;
	int32_t I_x_bledy_tab[BLEDY];
	int32_t I_y_bledy_tab[BLEDY];
	int32_t I_x_bledy_srednia;
	int32_t I_y_bledy_srednia;
	uint8_t I_ktory_blad;
} PIDTypeDef;

typedef struct
{
	uint32_t poziom;
	uint8_t poziom_procent;
	uint32_t ktora;
} BateriaTypeDef;

typedef struct
{
	PWMTypeDef pwm; //struktura PWM
	ZyroTypeDef zyro;
	MagnetTypeDef magnet;
	AkcelTypeDef akcel;
	PIDTypeDef pid;
	KatTypeDef kat;
	BateriaTypeDef bateria;

	signed int temp;
	uint8_t czy_polaczony; //zeby awaryjnie wylaczyc (systick)
	uint16_t opoznienie; //�eby zainicjalizowa� silniki oraz dla bezpiecze�stwa
} daneTypeDef;


void inicjalizacja_dane();
