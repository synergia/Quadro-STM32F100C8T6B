#include "naglowki_include.h"

void inicjalizacja_I2C();
void odczyt_I2C(I2C_TypeDef *I2Cx, uint8_t adres, uint8_t rejestr, uint8_t dlugosc, uint8_t *bufor);
int sprawdz_blad_I2C(I2C_TypeDef *I2Cx);
void wyslij_I2C(I2C_TypeDef *I2Cx, uint8_t adres, uint8_t rejestr, uint8_t dane);
