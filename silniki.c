#include "silniki.h"
#include "dane.h"

extern volatile daneTypeDef dane;

void inicjalizacja_silniki()
{
	GPIO_InitTypeDef *silniki = malloc(sizeof(GPIO_InitTypeDef));
	silniki->GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	silniki->GPIO_Mode = GPIO_Mode_Out_PP; //wyjscie pull-up
	silniki->GPIO_Speed = GPIO_Speed_2MHz; //moÂżna wybraĂ¦ 2,10,50 MHz
	GPIO_Init(GPIOB, silniki);
	free (silniki);
	GPIO_WriteBit(GPIOB, SILNIK1 | SILNIK2 | SILNIK3 | SILNIK4, Bit_RESET );
}

void ustaw_silnik(uint8_t ktory, uint8_t wartosc)
{

}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) //sprawdzenie zrodla
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update); //wyzerowanie flagi przerwania

		if (dane.pwm.pwm1 >= dane.pwm.timer)
			GPIO_WriteBit(GPIOB, SILNIK1 | SILNIK2 | SILNIK3 | SILNIK4, Bit_SET);
		else
			GPIO_WriteBit(GPIOB, SILNIK1 | SILNIK2 | SILNIK3 | SILNIK4, Bit_RESET);

		/*if (dane.pwm.pwm2 >= dane.pwm.timer)
			GPIO_WriteBit(GPIOB, SILNIK2, Bit_SET);
		else
			GPIO_WriteBit(GPIOB, SILNIK2, Bit_RESET);

		if (dane.pwm.pwm3 >= dane.pwm.timer)
			GPIO_WriteBit(GPIOB, SILNIK3, Bit_SET);
		else
			GPIO_WriteBit(GPIOB, SILNIK3, Bit_RESET);

		if (dane.pwm.pwm4 >= dane.pwm.timer)
			GPIO_WriteBit(GPIOB, SILNIK4, Bit_SET);
		else
			GPIO_WriteBit(GPIOB, SILNIK4, Bit_RESET);*/

		if (dane.pwm.timer >= 99)
			dane.pwm.timer = 0;
		dane.pwm.timer++;
	}
}
