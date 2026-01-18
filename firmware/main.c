#include "ch32fun.h"
#include "funconfig.h"

//uint8_t ledStrip[8] = {PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7};
//uint8_t lifeLeds[3] = {PD4, PA1, PA2};
//uint8_t switches[2] = {PA1, PA2};


uint8_t ledStrip[8] = {PC5, PC6, PD2, PD3, PD4, PD5, PD6, PC7};
uint8_t lifeLeds[3] = {PC0, PC1, PC2};
#define SW1 PC3
#define SW2 PC4
uint8_t lives = 3;
uint16_t speed = 500;

uint32_t prevLedTime, prevSwitchTime;

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
	static uint8_t pos = 0, dir  = 1;

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
/*
bool debounceSwitch(uint8_t sw)
{
	if ((millis() - prevSwitchTime) >= DEBOUNCE)
	{
		prevSwitchTime += DEBOUNCE;

		return funDigitalRead(switches[sw]);
	}
	return true;
}
*/

int main(void)
{
	SystemInit();
	funGpioInitAll();
	//SysTick_Init();

	funPinMode(SW1, GPIO_Speed_In | GPIO_CNF_IN_FLOATING);
	funPinMode(SW2, GPIO_Speed_In | GPIO_CNF_IN_FLOATING);
	
	uint8_t sw1_state, sw2_state;
	uint32_t interval = Ticks_from_Ms(500);	// interval = n * 6000000
	uint32_t pos;

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

	for (uint8_t i = 0; i < 3; i++)
	{
		funDigitalWrite(lifeLeds[i], FUN_HIGH);
	}
	

	for (;;)
	{			
		sw1_state = funDigitalRead(SW1);
		sw2_state = funDigitalRead(SW2);

		funDigitalWrite(lifeLeds[0], sw1_state);
		funDigitalWrite(lifeLeds[2], sw2_state);
		
		pos = moveLeds(&interval);

		if ( (!sw1_state && (pos == 0)) || (!sw2_state && (pos == 7)) )
		{
			Delay_Ms(25);
			interval -= Ticks_from_Ms(10);
		}
		/*
		if ( (!sw1_state && (pos == 0) || (!sw2_state && (pos == 7)) )
			lives--;
		
		for (uint8_t i = 0; i < lives; i++)
			funDigitalWrite(lifeLeds[i]
		*/
		//leds(&interval);
		//clearLeds();
		//move(interval);
	}		

	return 0;
}
