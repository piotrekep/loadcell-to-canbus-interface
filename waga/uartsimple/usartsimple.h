/*
 * usartsimple.h
 *
 * Created: 2015-02-19 00:06:29
 *  Author: Ja
 */ 

void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data );
void USART_TransmitString(char const *string );
void USART_Flush(void);
unsigned char USART_Receive( void );