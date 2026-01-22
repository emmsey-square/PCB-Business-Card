#include "ch32fun.h"
#include "funconfig.h"

#define SW1 PC3
#define SW2 PC4

uint8_t ledStrip[8] = {PC5, PC6, PD2, PD3, PD4, PD5, PD6, PC7};
uint8_t lifeLeds[3] = {PC0, PC1, PC2};

uint8_t lives = 4;	
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

	if (lives == 0)
	{
		clearLeds();
	}
	else
	{
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
	}

	return pos;
}

void loseLife(void)
{
	Delay_Ms(500);
	lives--;
}

void increaseSpeed(void)
{
	Delay_Ms(50);
	interval -= Ticks_from_Ms(5);
}

void gameOver(void)
{
	clearLeds();
	lives = 0;
}

void showLives(void)
{
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
			gameOver();
			break;
	}
}


int main(void)
{
	SystemInit();
	funGpioInitAll();
	
	funPinMode(SW1, GPIO_Speed_In | GPIO_CNF_IN_FLOATING);
	funPinMode(SW2, GPIO_Speed_In | GPIO_CNF_IN_FLOATING);
	
	interval = Ticks_from_Ms(500);	

	prevLedTime = millis();

	for (uint8_t i = 0; i < 8; i++)
	{
		funPinMode(ledStrip[i], GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
	}
	
	for (uint8_t i = 0; i < 3; i++)
	{
		funPinMode(lifeLeds[i], GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
	}

	clearLeds();

	for (;;)
	{			
		sw1_state = funDigitalRead(SW1);
		sw2_state = funDigitalRead(SW2);

		pos = moveLeds(&interval);
		
		if ( (!sw1_state && (pos == 0)) || (!sw2_state && (pos == 7)) )
		{
			increaseSpeed();
		}
		else if ( ((pos == 0) && (sw1_state == 1)) || ((pos == 7) && (sw2_state == 1)) )	
		{
			loseLife();
		}
		else if (!sw1_state && !sw2_state)
		{
			loseLife();
		}
		
		showLives();
	}

	return 0;
}
