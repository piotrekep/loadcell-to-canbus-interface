/*
 * CFile1.c
 *
 * Created: 2015-02-18 22:34:14
 *  Author: Ja
 */ 


#include <avr/io.h>
#include "usartsimple.h"


void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data );
void USART_TransmitString(char const *string );
void USART_Flush(void);
unsigned char USART_Receive( void );

void USART_Init( unsigned int ubrr)
{
	/*
	Set baud rate
	*/
	UBRR0H = (	unsigned char	)(ubrr>>8);
	UBRR0L = (	unsigned char	)ubrr;
	/*
	Enable receiver and transmitter
	*/
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/*
	Set frame format: 8data, 2stop bit
	*/
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	}
	
void  USART_Transmit(  unsigned char  data )
{
/* 
Wait for empty transmit buffer
 */
while
 ( !( UCSR0A & (1<<UDRE0)) );
/* 
Put data into buffer, sends the data
 */
UDR0 = data;
}


void USART_Flush(void)
{
/*	unsigned char
	dummy=0;
	while
	( UCSRA & (1<<RXC) ) dummy = UDR;*/
}

void USART_TransmitString(char const *string )
{

	while(*string) USART_Transmit(*string++);
	//while(*string) uart_send_byte(*string++);
	

	
}

unsigned char USART_Receive( void )
{
/* 
Wait for data to be received
 */
while
 ( !(UCSR0A & (1<<RXC0)) );
/* 
Get and return received data from buffer
 */
return UDR0;
}

