#include "naglowki_include.h"

#ifndef _SENSORY_H_
#define _SENSORY_H_

#define ZYRO_ADR 0xD6

#endif

void inicjalizacja_sensory();
void inicjalizacja_zyroskop();
void inicjalizacja_magnetometr();

void odczyt_zyroskop();
void odczyt_magnetometr();

void odczyt_sensory();
