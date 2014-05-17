#include "silniki.h"

void inicjalizacja_silniki()
{
	GPIO_InitTypeDef *silniki = malloc(sizeof(GPIO_InitTypeDef));
	silniki->GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	silniki->GPIO_Mode = GPIO_Mode_Out_PP; //wyjscie pull-up
	silniki->GPIO_Speed = GPIO_Speed_2MHz; //moÂżna wybraĂ¦ 2,10,50 MHz
	GPIO_Init(GPIOB, silniki);
	free (silniki);
	GPIO_WriteBit(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_RESET );
	GPIO_WriteBit(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_SET );
}
