#include "ch32fun.h"
#include "funconfig.h"


#define SW1 PC3
#define SW2 PC4

uint8_t ledStrip[8] = {PC5, PC6, PD2, PD3, PD4, PD5, PD6, PC7};
uint8_t lifeLeds[3] = {PC0, PC1, PC2};

uint8_t lives = 3;
uint32_t interval;
uint32_t pos = 0;
uint32_t prevLedTime, prevSwitchTime;
uint8_t sw1_state, sw2_state;
uint8_t started = 0;

inline uint32_t millis(void)
{
	return SysTick->CNT;
}


void clearLeds(void)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		funDigitalWrite(ledStrip[i], FUN_HIGH);
	}
}


uint32_t moveLeds(uint32_t* interval)
{
	static uint32_t pos = 0, dir  = 1;

	if ((millis() - prevLedTime) >= *interval)
	{
		prevLedTime += *interval;
		
		clearLeds();

		if (dir)
			pos++;
		else 
			pos--;


		if (pos >= 7)
			dir = 0;
		else if (pos <= 0)
			dir = 1;
	}
	
	funDigitalWrite(ledStrip[pos], FUN_LOW);

	return pos;
}

void showLives(void)
{
	for (uint8_t i = 0; i < 3; i++)
		funDigitalWrite(lifeLeds[i], FUN_HIGH);

	if (lives <= 0)
	{
		for (uint8_t i = 0; i < 3; i++)
			funDigitalWrite(lifeLeds[i], FUN_HIGH);
	}
	else 
	{
		for (uint8_t i = 0; i < lives; i++)
			funDigitalWrite(lifeLeds[i], FUN_LOW);
	}
}


int main(void)
{
	SystemInit();
	funGpioInitAll();

	funPinMode(SW1, GPIO_Speed_In | GPIO_CNF_IN_FLOATING);
	funPinMode(SW2, GPIO_Speed_In | GPIO_CNF_IN_FLOATING);
	
	interval = Ticks_from_Ms(500);	// interval = n * 6000000

	prevLedTime = millis();

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

	clearLeds();

	for (;;)
	{			
		sw1_state = funDigitalRead(SW1);
		sw2_state = funDigitalRead(SW2);

		//funDigitalWrite(lifeLeds[0], sw1_state);
		//funDigitalWrite(lifeLeds[2], sw2_state);
		
		pos = moveLeds(&interval);
		
		// Check if buttons are pressed when pos hits equals either end
		
		if ((millis() > 2000000000) && (started == 0))
			started = 1;

		if ( (!sw1_state && (pos == 0)) || (!sw2_state && (pos == 7)) )
		{
			Delay_Ms(25);
			interval -= Ticks_from_Ms(10);
		}
		
		if ((pos == 0) && (sw1_state == 1))
		{
			Delay_Ms(500);
			lives--;
		}

		if ((pos == 7) && (sw2_state == 1))
		{
			Delay_Ms(500);
			lives--;
		}

		for (uint8_t i = 0; i < 3; i++)
			funDigitalWrite(lifeLeds[i], FUN_HIGH);
		
		switch (lives)
		{
			case 0:
				for (uint8_t i = 0; i < 3; i++)
					funDigitalWrite(lifeLeds[i], FUN_HIGH);
				break;
			case 1:
				funDigitalWrite(lifeLeds[0], FUN_LOW);
				break;
			case 2:
				for (uint8_t i = 0; i < 2; i++)
					funDigitalWrite(lifeLeds[i], FUN_LOW);
				break;
			case 3:
				for (uint8_t i = 0; i < 3; i++)
					funDigitalWrite(lifeLeds[i], FUN_LOW);
				break;
			default:
				lives = 3;
				break;
		}
	}

	return 0;
}
