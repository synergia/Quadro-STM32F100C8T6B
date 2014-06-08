#include "naglowki_include.h"

#ifndef _SENSORY_H_
#define _SENSORY_H_

#define ZYRO_ADR 0xD6
#define MAGNET_ADR 0x3C
#define AKCEL_ADR 0x32
#define PI 3.14159265

#define DT 20 //20 ms do calkowania
#define MDEG 8.75 //na 1 cyfre 8.75 milistopnia
#define DELTAZYRO 300 //eksperymentalnie dobrana stala bledu zyroskopu
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

void odczyt_sensory();
void oblicz_kat();
