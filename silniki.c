#include "silniki.h"
#include "dane.h"
#include "timery.h"
#include "sensory.h"

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
		if (dane.pwm.pwm1 == 0 && dane.pwm.pwm2 == 0 && dane.pwm.pwm3 == 0 && dane.pwm.pwm4 == 0)
			GPIO_WriteBit(GPIOB, SILNIK1 | SILNIK2 | SILNIK3 | SILNIK4, Bit_RESET);
		else
			GPIO_WriteBit(GPIOB, SILNIK1 | SILNIK2 | SILNIK3 | SILNIK4, Bit_SET);

		TIM4->CNT = 0;
		TIM4->DIER = TIM_DIER_UIE; //wlaczenie przerwania szerokosci impulsu
		TIM_ClearFlag(TIM4, TIM_FLAG_Update); //wyzerowanie flagi przerwania
	}
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //sprawdzenie zrodla
	{
		TIM_ClearFlag(TIM3, TIM_FLAG_Update); //wyzerowanie flagi przerwania

		if (dane.pwm.pwm1 > dane.pwm.timer)
			GPIO_WriteBit(GPIOB, SILNIK1, Bit_SET);
		else
			GPIO_WriteBit(GPIOB, SILNIK1, Bit_RESET);

		if (dane.pwm.pwm2 > dane.pwm.timer)
			GPIO_WriteBit(GPIOB, SILNIK2, Bit_SET);
		else
			GPIO_WriteBit(GPIOB, SILNIK2, Bit_RESET);

		if (dane.pwm.pwm3 > dane.pwm.timer)
			GPIO_WriteBit(GPIOB, SILNIK3, Bit_SET);
		else
			GPIO_WriteBit(GPIOB, SILNIK3, Bit_RESET);

		if (dane.pwm.pwm4 > dane.pwm.timer)
			GPIO_WriteBit(GPIOB, SILNIK4, Bit_SET);
		else
			GPIO_WriteBit(GPIOB, SILNIK4, Bit_RESET);

		if (dane.pwm.timer >= 99)
		{
			TIM3->DIER = 0; //wylaczenie przerwania
			GPIO_WriteBit(GPIOB, SILNIK1 | SILNIK2 | SILNIK3 | SILNIK4, Bit_RESET);
		}
		//dane.pwm.timer = 0;
		else
			dane.pwm.timer++;
	}
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) //sprawdzenie zrodla
	{
		TIM4->DIER = 0; // wylaczenie tego przerwania
		TIM_ClearFlag(TIM4, TIM_FLAG_Update); //wyzerowanie flagi przerwania

		dane.pwm.timer = 0;
		TIM3->CNT = 0;
		TIM3->DIER = TIM_DIER_UIE; //wlaczenie przerwania szerokosci impulsu
	}
}
