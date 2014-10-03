#include "naglowki_include.h"

#ifndef _SENSORY_H_
#define _SENSORY_H_

#define ZYRO_ADR 0xD6
#define MAGNET_ADR 0x3C
#define AKCEL_ADR 0x32
#define BARO_ADR 0xBA
#define PI 3.14159265

#define DT 0.02 //20 ms do calkowania
#define MDEG 8.75 //na 1 cyfre 8.75 milistopnia (zyro)
#define AKC_SKALA 16000.0
#define DELTAZYROX 300 //eksperymentalnie dobrana stala bledu zyroskopu

#define KALIBR 128 //z tylu próbek jest kalibrowany zyroskop
#define KALIBR_PRZESUN 7
/*
 * ms * mdeg/s = udeg
 * ale podzielone przez 1000 wiec ms
 */

#endif

void inicjalizacja_sensory();
void inicjalizacja_zyroskop();
void inicjalizacja_magnetometr();
void inicjalizacja_akcelerometr();
void inicjalizacja_barometr(); //takze termometr

void odczyt_zyroskop(uint8_t *bufor);
void odczyt_magnetometr(uint8_t *bufor);
void odczyt_akcelerometr(uint8_t *bufor);
void odczyt_barometr(uint8_t *bufor);
void obliczenia_akcelerometr();

void odczyt_sensory();
void oblicz_kat();
void kalman_x(double newAngle, double newRate, double dt);
