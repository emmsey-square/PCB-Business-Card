#include "../ch32fun/ch32fun.h"
#include "funconfig.h"

/*

Buttons: PD5:6	
LED strip: PC0:7
Life LED's: PD4, PA1:2
*/

//uint8_t ledStrip[8] = {PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7};
//uint8_t lifeLeds[3] = {PD4, PA1, PA2};

uint8_t ledStrip[8] = {PC5, PC6, PD2, PD3, PD4, PD5, PD6, PC7};
uint8_t lifeLeds[3] = {PD4, PA1, PA2};
#define SW1	PA2
#define SW2 PA1


typedef enum 
{
	LEFT,
	RIGHT,
} 
Direction;


void move(Direction dir, uint8_t pos, uint8_t speed)
{
	if (dir == LEFT)
	{
		if (pos > 0) 
			--pos;
		else 
			pos = 8;
	}
	else 
	{
		if (pos < 8)
			++pos;
		else 
			pos = 0;
	}

	Delay_Ms(speed);
}


int main(void)
{
	SystemInit();
	funGpioInitAll();

	uint16_t speed = 500;
	uint8_t pos = 0;
	Direction dir = LEFT;
	//funPinMode(PA2, GPIO_Speed_In | GPIO_CNF_IN_FLOATING);
	

	//Initialise GPIO's for each strip LED as push-pull output

	for (uint8_t i = 0; i < 8; i++)
	{
		funPinMode(ledStrip[i], GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
	}
	
	// Initialise GPIO's for life LEDs as push-pull outputs

	for (uint8_t i = 0; i < 3; i++)
	{
		funPinMode(lifeLeds[i], GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
	}


	while(1)
	{			
		uint8_t button1 = funDigitalRead(SW1);
		uint8_t button2 = funDigitalRead(SW2);

		for (uint8_t i = 0; i < 8; i++)
		{
			funDigitalWrite(ledStrip[i], FUN_HIGH);
		}

		funDigitalWrite(ledStrip[pos], FUN_LOW);

		if (pos == 0 && button1)
		{
			dir = RIGHT;
			speed -= 50;
		}
		else if (pos == 0 && !button1)
		{
			if (lives > 0)
				--lives;
			else 
				lives = 0;
		}
		else 
		{
			move(dir, pos, speed);
		}

		if (pos == 7 && button2)
		{
			dir = LEFT;
			speed -= 50;
		}
		else if (pos == 7 && !button2)
		{
			if (lives > 0)
				--lives;
			else 
				lives = 0;
		}
		else 
		{
			move(dir, pos, speed);
		}

	}	

	return 0;
}
