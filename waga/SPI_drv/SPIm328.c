#include <avr/io.h>
#include "../SPI_drv/SPIm328.h"

void SPI_MasterInit( void )
{
	/* Set MOSI and SCK output, all others input */
	DDRB |= (1<<PB3)|(1<<PB5);
	/* Enable SPI, Master, set clock rate fck/16 */
	
	SPCR |= (1<<SPE)|(1<<MSTR)|(0<<SPR0);
	SPSR |= (1<<SPI2X);
	
	DDRB |=(1<<PB2); // cs wyjscie
	PORTB|=(1<<PB2); // cs wysoki wysoki
}

void SPI_MasterTransmit( char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	
	while(!(SPSR & (1<<SPIF)));
}

char SPI_MasterRecieve(void)
{
	/*dummy write*/
	SPDR=0xff;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
	return SPDR;
}

void SPI_setCS(void)
{
	PORTB &= ~(1<<PB2); //cs na zero
}

void SPI_clearCS(void)
{
	PORTB |=(1<<PB2);
}
