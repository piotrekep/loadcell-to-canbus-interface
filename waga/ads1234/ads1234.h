/*
 * ads1234.h
 *
 * Created: 2017-06-29 14:13:18
 *  Author: Ja
 */ 


#ifndef ADS1234_H_
#define ADS1234_H_

#include <inttypes.h>
#include <avr/io.h>

#define GAIN0_PORT PORTD
#define GAIN1_PORT PORTD
#define GAIN0_PIN 1
#define GAIN1_PIN 2
#define GAIN0_DDR DDRD
#define GAIN1_DDR DDRD


#define A0_PORT PORTC
#define A1_PORT PORTC
#define A0_PIN 1
#define A1_PIN 0
#define A0_DDR DDRC
#define A1_DDR DDRC

#define SPEED_PORT PORTC
#define SPEED_PIN 5
#define SPEED_DDR DDRC

#define DATA_PORT PINC
#define DATA_PIN 2
#define DATA_DDR DDRC

#define CLK_PORT PORTC
#define CLK_PIN 3
#define CLK_DDR DDRC


#define PWR_PORT PORTC
#define PWR_PIN 4
#define PWR_DDR DDRC



#define ADS1234_AIN1 0
#define ADS1234_AIN2 2
#define ADS1234_AIN3 1
#define ADS1234_AIN4 3


#define ADS1234_GAIN1 0
#define ADS1234_GAIN2 2
#define ADS1234_GAIN64 1
#define ADS1234_GAIN128 3


#define ADS1234_LOW_SPEED 0
#define ADS1234_HIGH_SPEED 1


#define ADS1234_PWR_ON 1
#define ADS1234_PWR_OFF 0

void ADS1234_init(void);
void ADS1234_PWR(uint8_t stat);
void ADS1234_input_select(uint8_t input);
void ADS1234_gain_select(uint8_t gain);
void ADS1234_Speed(uint8_t spd);
uint32_t ADS1234_read(void);
void ADS1234_clk_high(void);
void ADS1234_clk_low(void);

#endif /* ADS1234_H_ */