#include "naglowki_include.h"
#include "inicjalizacja.h"
#include "timery.h"
#include "LED.h"
#include "silniki.h"
#include "ADC.h"
#include "NVIC.h"
#include "USART.h"
#include "dane.h"
#include "I2C.h"

int main(void)
{
	inicjalizacja_zasilania();
	inicjalizacja_dane();
	inicjalizacja_LED();
	inicjalizacja_SysTick();
	inicjalizacja_silniki();
//	inicjalizacja_ADC();
//	inicjalizacja_USART();
	inicjalizacja_I2C();
	/*inicjalizacja_TIM2();
	inicjalizacja_TIM4();
	inicjalizacja_TIM3();*/


	inicjalizacja_NVIC();

	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C1, 0x55, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED));
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	int i = 0;
	int dupa;

	while(i < 100000)
	{
		I2C_SendData(I2C1, 22);
		while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_BYTE_RECEIVED));
		dupa = I2C_ReceiveData(I2C2);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		if (dupa == 22)
			GPIOA->ODR |= LED_NIEB_1;
		i++;
	}
	I2C_GenerateSTOP(I2C1,ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_STOP_DETECTED));
	I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF);
	I2C_Cmd(I2C2, ENABLE);

    while(1)
    {
    }
}
