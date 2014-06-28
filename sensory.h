#include "naglowki_include.h"
#include "timery.h"

#ifndef _SENSORY_H_
#define _SENSORY_H_

#define ZYRO_ADR 0xD6
#define MAGNET_ADR 0x3C
#define AKCEL_ADR 0x32
#define PI 3.14159265

#define DT 0.02 //20 ms do calkowania
#define MDEG 8.75 //na 1 cyfre 8.75 milistopnia
#define AKC_SKALA 8000.0
#define DELTAZYROX 300 //eksperymentalnie dobrana stala bledu zyroskopu

#define KALIBR 32 //z tylu próbek jest kalibrowany zyroskop
#define KALIBR_PRZESUN 5
/*
 * ms * mdeg/s = udeg
 * ale podzielone przez 1000 wiec ms
 */

#endif

void inicjalizacja_sensory();
void inicjalizacja_zyroskop();
void inicjalizacja_magnetometr();
void inicjalizacja_akcelerometr();

void odczyt_zyroskop(uint8_t *bufor);
void odczyt_magnetometr(uint8_t *bufor);
void odczyt_akcelerometr(uint8_t *bufor);
void odczyt_akcelerometr_prosty(); //jak najszybszy odczyt, bez obliczen

void odczyt_sensory();
void oblicz_kat();
void kalman_x(double newAngle, double newRate, double dt);
