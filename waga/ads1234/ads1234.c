/*
 * ads1234.c
 *
 * Created: 2017-06-29 14:13:07
 *  Author: Ja
 */ 

#include "ads1234.h"
#include <inttypes.h>


void ADS1234_init(void)
{
	GAIN0_DDR |= (1<<GAIN0_PIN);
	GAIN1_DDR |= (1<<GAIN1_PIN);
	
	A0_DDR |= (1<<A0_PIN);
	A1_DDR |= (1<<A1_PIN);
	
	SPEED_DDR |= (1<<SPEED_PIN);
	
	DATA_DDR &= ~(1<<DATA_PIN);
	CLK_DDR |= (1<<CLK_PIN);
	
	PWR_DDR |=(1<<PWR_PIN);
	
	
}


void ADS1234_PWR(uint8_t stat)
{

switch(stat)
 {
	case 0: 
		PWR_PORT &= ~(1<<PWR_PIN);
		break;
	case 1:
		PWR_PORT |= (1<<PWR_PIN);
		break;
	default:
		break;
 }
}

void ADS1234_input_select(uint8_t input)
{

	switch(input)
	{
		case 0:
			A0_PORT &= ~(1<<A0_PIN);
			A1_PORT &= ~(1<<A1_PIN);
			break;
		case 1:
			A0_PORT &= ~(1<<A0_PIN);
			A1_PORT |= (1<<A1_PIN);
			break;
		case 2:
			A0_PORT |= (1<<A0_PIN);
			A1_PORT &= ~(1<<A1_PIN);
			break;
		case 3:
			A0_PORT |= (1<<A0_PIN);
			A1_PORT |= (1<<A1_PIN);;
			break;
		default:
			break;
	}
}

void ADS1234_gain_select(uint8_t gain)
{

	switch(gain)
	{
		case 0:
		GAIN0_PORT &= ~(1<<GAIN0_PIN);
		GAIN1_PORT &= ~(1<<GAIN1_PIN);
		break;
		case 1:
		GAIN0_PORT &= ~(1<<GAIN0_PIN);
		GAIN1_PORT |= (1<<GAIN1_PIN);
		break;
		case 2:
		GAIN0_PORT |= (1<<GAIN0_PIN);
		GAIN1_PORT &= ~(1<<GAIN1_PIN);
		break;
		case 3:
		GAIN0_PORT |= (1<<GAIN0_PIN);
		GAIN1_PORT |= (1<<GAIN1_PIN);;
		break;
		default:
		break;
	}
}


void ADS1234_Speed(uint8_t spd)
{

	switch(spd)
	{
		case 0:
		SPEED_PORT &= ~(1<<SPEED_PIN);
		break;
		case 1:
		SPEED_PORT |= (1<<SPEED_PIN);
		break;
		default:
		break;
	}
}

uint32_t ADS1234_read(void)
{
	
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;
	uint32_t r;
	for(uint8_t i =8; i>0; i--)
	{
		ADS1234_clk_high();
		if (DATA_PORT & (1<<DATA_PIN)) x |= 1 << (i-1);
		ADS1234_clk_low();
		
	}
	
	for(uint8_t i =8; i>0; i--)
	{
		ADS1234_clk_high();
		if (DATA_PORT & (1<<DATA_PIN)) y |= 1 << (i-1);
		ADS1234_clk_low();
		
	}
	
	for(uint8_t i =8; i>0; i--)
	{
		ADS1234_clk_high();
		if (DATA_PORT & (1<<DATA_PIN)) z |= 1 << (i-1);
		ADS1234_clk_low();
		
	}
	
	ADS1234_clk_high();
	ADS1234_clk_low();



	r=x;
	r=(r<<8)|y;
	r=(r<<8)|z;

	return r;
	
}


void ADS1234_clk_high(void)
{
	CLK_PORT |= (1<<CLK_PIN);

	
}

void ADS1234_clk_low(void)
{
	CLK_PORT &= ~(1<<CLK_PIN);

	
}
