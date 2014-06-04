#include "naglowki_include.h"

#ifndef _SENSORY_H_
#define _SENSORY_H_

#define ZYRO_ADR 0xD6
#define MAGNET_ADR 0x3C

#endif

void inicjalizacja_sensory();
void inicjalizacja_zyroskop();
void inicjalizacja_magnetometr();

void odczyt_zyroskop(uint8_t *bufor);
void odczyt_magnetometr(uint8_t *bufor);

void odczyt_sensory();
