/*
 * j1939ecu.c
 *
 * Created: 2017-05-02 16:46:30
 *  Author: Ja
 */ 
#define FOSC 20000000// Clock Speed
#define BAUD 115200
#define MYUBRR FOSC/16/BAUD-1


#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "types/tdef.h"
#include "SPI_drv/SPIm328.h"
#include "CAN_drv//can_drv.h"
#include "ads1234/ads1234.h"


ISR(INT0_vect );
ISR(TIMER0_COMPA_vect );


volatile CAN_msg tx_msg1;
volatile CAN_msg tx_msg2;
volatile uint32_t i=0;
volatile uint8_t data_rdy=0;
	
int main(void)
{
   	wdt_enable(WDTO_2S);
	 
   	tx_msg1.ident=0x250; //message 1 ID, supports only standard id's adding extended should be trivial
   	tx_msg1.RTR=0;
   	tx_msg1.DLC=8;
	   
	tx_msg2.ident=0x251; //message 2 ID
	tx_msg2.RTR=0;
	tx_msg2.DLC=8;
  
// can transmit interupt timer
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<CS02) | (0<<CS01) | (1<<CS00);
	TIMSK0 |= (1<<OCIE0A); 
	OCR0A=195; 
	
	EIMSK |= (1<<INT1);
	//EIMSK = 0;
	DDRD=0;
	PORTD=0;

	DDRD |= (1<<PIND4);
	PORTD |= (1<<PORTD0);
	   
	   
	SPI_MasterInit();
   	CAN_reset();
	 _delay_ms(10);
	//can init
	CAN_setBaud20(2);
	CAN_int_en(0x21);
	//CAN_int_en(0x03);
	CAN_mode(0);
	
		if(CAN_check_mode()==0)
		{
					//todo: add error handling
		}


	ADS1234_init();
	ADS1234_PWR(ADS1234_PWR_ON);
	ADS1234_input_select(ADS1234_AIN1);
	ADS1234_Speed(ADS1234_HIGH_SPEED);
	ADS1234_gain_select(ADS1234_GAIN128);

	sei();
 
 // uint32_t temp=0;
  uint32_t d0=0;
  uint32_t d1=0;
  uint32_t d2=0;
  uint32_t d3=0;
  
 // int32_t srednia=0;
 
  
  
    while(1)
    {
       wdt_reset();
	   
	 
	 ADS1234_input_select(ADS1234_AIN1);  
	 while(((DATA_PORT & (1<<DATA_PIN))!=0));
	d0=ADS1234_read();
		//srednia=temp;
			d0=d0<<8;
			d0=d0>>8;
			// srednia=temp<<8;
			// srednia=srednia>>8;
			tx_msg1.data[0]=d0;
			tx_msg1.data[1]=d0>>8;
			tx_msg1.data[2]=d0>>16;
			tx_msg1.data[3]=d0>>24;
					
	 ADS1234_input_select(ADS1234_AIN2);
	 while(((DATA_PORT & (1<<DATA_PIN))!=0));
	 d1=ADS1234_read();
	 //srednia=temp;
	 	 d1=d1<<8;
	 	 d1=d1>>8;
		 tx_msg1.data[4]=d1;
		 tx_msg1.data[5]=d1>>8;
		 tx_msg1.data[6]=d1>>16;
		 tx_msg1.data[7]=d1>>24;

	 ADS1234_input_select(ADS1234_AIN3);
	 while(((DATA_PORT & (1<<DATA_PIN))!=0));
	 d2=ADS1234_read();
	 d2=d2<<8;
	 d2=d2>>8;
		 tx_msg2.data[0]=d2;
		 tx_msg2.data[1]=d2>>8;
		 tx_msg2.data[2]=d2>>16;
		 tx_msg2.data[3]=d2>>24;
	 
	 ADS1234_input_select(ADS1234_AIN4);
	 while(((DATA_PORT & (1<<DATA_PIN))!=0));
	 d3=ADS1234_read();
	 //srednia=temp;
	 	 d3=d3<<8;
	 	 d3=d3>>8;
		 tx_msg2.data[4]=d3;
		 tx_msg2.data[5]=d3>>8;
		 tx_msg2.data[6]=d3>>16;
		 tx_msg2.data[7]=d3>>24; 
	 
data_rdy=1;

	   
    }
}




ISR(INT1_vect )
{
	uint8_t flag;

	flag = CAN_get_pint();


	switch(flag)
	{
		case 1: //error interrupt
		CAN_clear_int(5);
		break;
		
		case 2: //wake up
		CAN_clear_int(6);
		
		break;
		case 3: //TXB0 empty		
		CAN_clear_int(2);
		
		break;
		case 4: //TXB1 empty
		CAN_clear_int(3);
		
		break;
		case 5: //TXB2 empty
		CAN_clear_int(4);
		
		break;
		case 6: //RXB0 full
		CAN_clear_int(0);
		
		
		break;
		case 7: //RXB1 full
		CAN_clear_int(1);
		
		
		break;
	}


}



ISR(TIMER0_COMPA_vect ) //10ms
{

// 	   

if((i>=10) & (data_rdy>0))
{
		CAN_TX_Write(tx_msg1,0);
		CAN_RTS(0);
		CAN_TX_Write(tx_msg2,1);
		CAN_RTS(1);
i=0;
data_rdy=0;
}
	   
	i++;

}

