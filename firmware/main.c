#include "ch32fun.h"
#include "funconfig.h"

//uint8_t ledStrip[8] = {PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7};
//uint8_t lifeLeds[3] = {PD4, PA1, PA2};
//uint8_t switches[2] = {PA1, PA2};

#define LEFT 0
#define RIGHT 1

uint8_t ledStrip[8] = {PC5, PC6, PD2, PD3, PD4, PD5, PD6, PC7};
uint8_t lifeLeds[3] = {PC0, PC1, PC2};
uint8_t switches[2] = {PC3, PC4};

uint8_t lives = 3;
uint16_t speed = 500;
uint8_t pos = 0;

uint32_t last;
/*
void SysTick_Init(void)
{
	SysTick->CTLR = 0x0000;
	SysTick->CMP = DELAY_MS_TIME - 1;
	SysTick->CNT - 0x00000000;
	millis = 0x00000000;
	SysTick->CTLR |= (SYSTICK_CTLR_STE | SYSTICK_CTLR_STIE | SYSTICK_CTLR_STCLK);
	NVIC_EnableIRQ(SysTick_IRQn);
}

void SysTick_Handler(void) __attribute__((interrupt));
void SysTick_Handler(void)
{
	SysTick->CMP += DELAY_MS_TIME;
	SysTick->SR = 0x00000000;
	millis++;
}
*/

static inline uint32_t millis(void)
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

/*
void move(void)
{
	clearLeds();
		
	for (uint8_t i = 0; i < 8; i++)
	{	
		if (i == 0) 
		{
			funDigitalWrite(ledStrip[i], FUN_LOW);
		}
		else 
		{
			funDigitalWrite(ledStrip[i], FUN_LOW);
			funDigitalWrite(ledStrip[i-1], FUN_HIGH);
		}
		Delay_Ms(speed);
	}

	clearLeds();

	for (uint8_t i = 7; i > 0; i--)
	{
		if (i == 8 || i == 0) 
		{
			funDigitalWrite(ledStrip[i], FUN_LOW);
		}
		else
		{
			funDigitalWrite(ledStrip[i], FUN_LOW);
			funDigitalWrite(ledStrip[i+1], FUN_HIGH);
		}
		//Delay_Ms(speed);
	}

	clearLeds();

	if (speed > 0)
		speed -= 50;
	else 
		speed = 500;
}
*/

void move(uint16_t interval)
{
	
	clearLeds();

	for (uint8_t i = 0; i < 8; i++)
	{
		if ((millis() - last) >= interval)
		{

			last += interval;

			if (i == 0) 
			{
				funDigitalWrite(ledStrip[i], FUN_LOW);
			}
			else 
			{
				funDigitalWrite(ledStrip[i], FUN_LOW);
				funDigitalWrite(ledStrip[i-1], FUN_HIGH);
			}
		}
	}

	clearLeds();

	for (uint8_t i = 7; i > 0; i--)
	{
		if ((millis() - last) >= interval)
		{
			last += interval;

			if (i == 0) 
			{
				funDigitalWrite(ledStrip[i], FUN_LOW);
			}
			else 
			{
				funDigitalWrite(ledStrip[i], FUN_LOW);
				funDigitalWrite(ledStrip[i+1], FUN_HIGH);
			}
		}
	}
	clearLeds();
	
	if (interval > 0)
		interval -= 50;
	else 
		interval = 500;
}

void leds(uint32_t interval)
{
	static uint8_t pos = 0, dir  = 1;
	/*
	for (uint8_t i = 0; i < 8; i++)
	{
		if ((millis() - last) >= interval)
		{
			last += interval;
			funDigitalWrite(ledStrip[i], FUN_LOW);
		}

		if (i == 7)
			clearLeds();
	}
	*/
	/*
	for (uint8_t i = 0; i < 8; i++)
	{
		if ((millis() - last) >= interval)
		{
			last += interval;
			funDigitalWrite(ledStrip[i], FUN_LOW);
		}
		else
		{
			funDigitalWrite(ledStrip[i], FUN_HIGH);
		}
	}
	*/

	if ((millis() - last) >= interval)
	{
		last += interval;

		if (dir)
			pos++;
		else 
			pos--;

		if (pos > 7)
		{
			clearLeds();
			dir = 0;
		}
		else if (pos < 0)
		{
			clearLeds();
			dir = 1;
		}
	}
	
	funDigitalWrite(ledStrip[pos], FUN_LOW);
}


int main(void)
{
	SystemInit();
	funGpioInitAll();
	//SysTick_Init();

	funPinMode(switches[0], GPIO_Speed_In | GPIO_CNF_IN_FLOATING);
	funPinMode(switches[1], GPIO_Speed_In | GPIO_CNF_IN_FLOATING);
	
	uint8_t sw1_state, sw2_state;

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
	
	
	last = millis();
	uint32_t interval = Ticks_from_Ms(500);

	while(1)
	{			
		sw1_state = funDigitalRead(switches[0]);
		sw2_state = funDigitalRead(switches[1]);

		funDigitalWrite(lifeLeds[0], sw1_state);
		funDigitalWrite(lifeLeds[2], sw2_state);
		
		leds(interval);
		//clearLeds();
		//move(interval);
	}		

	return 0;
}
