#include "../ch32fun/ch32fun.h"
#include "funconfig.h"

/*

Buttons: PD5:6	
LED strip: PC0:7
Life LED's: PD4, PA1:2
*/

//uint8_t ledStrip[8] = {PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7};
//uint8_t lifeLeds[3] = {PD4, PA1, PA2};

#define LED_PIN	PA2


uint8_t ledStrip[8] = {PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7};
uint8_t lifeLeds[3] = {PD4, PA1, PA2};

int main(void)
{
	SystemInit();
	funGpioInitAll();

	//funPinMode(PC0, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
	
	//funPinMode(PA2, GPIO_Speed_In | GPIO_CNF_IN_FLOATING);
	
	for (int i = 0; i < 8; i++)
	{
		funPinMode(ledStrip[i], GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
	}


	while(1)
	{
		//uint8_t input = funDigitalRead(PA2);

		//funDigitalWrite(PC0, input);

		for (int i = 0; i < 8; i++)
		{
			funDigitalWrite(ledStrip[i], FUN_HIGH);
			Delay_Ms(250);
		}

		for (int i = 0; i < 8; i++)
		{
			funDigitalWrite(ledStrip[i], FUN_LOW);
		}
		
	}

	return 0;
}
