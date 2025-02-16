/*
 * can_drv.c
 *
 * Created: 2015-11-19 15:34:22
 *  Author: Ja
 */ 

#include "../CAN_drv/can_drv.h"
#include "../types/tdef.h"
#include "../SPI_drv/SPIm328.h"
#include <avr/io.h>
#include <inttypes.h>

//Reset the MCP2515
void CAN_reset(void)
{
	SPI_setCS();
	SPI_MasterTransmit(0xC0); //reset command
	SPI_clearCS();
}

//reads set amount of registers of MCP2515
//startadress - number of register to start reading from
//BytesToRead - number of registers to read
//*buf - pointer to read registers

void CAN_Read(uint8_t startadress, uint8_t BytesToRead, uint8_t *buf)
{
	
	uint8_t i;
	
	SPI_setCS();
	SPI_MasterTransmit(0x03);         //read command
	SPI_MasterTransmit(startadress);  //first register adress
	for(i=0;i<BytesToRead;i++)       // increment until requested amount of adresses have been read
	{
		buf[i]=SPI_MasterRecieve();  //save read value in buffer
	}
	
	SPI_clearCS();
	
}
//reads a single register of MCP2515
//startadress - register to read

uint8_t CAN_Read_single(uint8_t startadress)
{
		uint8_t byte;
		SPI_setCS();
		SPI_MasterTransmit(0x03);       //read command
		SPI_MasterTransmit(startadress);//register address
		byte=SPI_MasterRecieve();       //save register state to buffer
		SPI_clearCS();
		return byte;                    //returns read value
}

//writes data from buffer to MCP2515
//startadres   - address of the first register
//BytesToWrite - number of bytes to write
//*buf         - pointer to buffer with data

void CAN_Write(uint8_t startadres, uint8_t BytesToWrite, uint8_t *buf)
{
	
	uint8_t i;
	
	SPI_setCS();
	SPI_MasterTransmit(0x02);      //write command
	SPI_MasterTransmit(startadres);//register to begin writing
	for(i=0;i<BytesToWrite;i++)    // increment until requested amount of addresses have been written
	{
		SPI_MasterTransmit(buf[i]); //transmit data from buffer
	}
	
	SPI_clearCS();
	
}

//writes a single address to MCP2515
//startadress - adress of the register to be written
//byte        - data to write

void CAN_Write_single(uint8_t startadress,uint8_t byte)
{
	
	SPI_setCS();
	SPI_MasterTransmit(0x02);       //write command
	SPI_MasterTransmit(startadress);//address of register to be written
	SPI_MasterTransmit(byte);       //write data
	SPI_clearCS();
	
}

void CAN_modify(uint8_t startadress, uint8_t mask, uint8_t data)
{
	SPI_setCS();
	SPI_MasterTransmit(0x05);
	SPI_MasterTransmit(startadress); 
	SPI_MasterTransmit(mask);
	SPI_MasterTransmit(data);  	
	SPI_clearCS(); 
	
}

//reads can message from selected buffer
//buff_number - number of buffer to be read (0 or 1)
//returns Can_msg


CAN_msg CAN_RX_Read(uint8_t buff_number)
{
	uint8_t buffor;    //buffer selection
	uint8_t i=0;
	uint8_t buf[13];  //data buffer
	CAN_msg msg;      //can message
	
	if(buff_number==0)buffor=0b10010000; //selecting the 0 buffer
	else buffor=0b10010100;              //selecting the 1 buffer
	
		SPI_setCS();                    //reads data from buffer TODO: change to CAN_read
		SPI_MasterTransmit(buffor);    
		
		for(i=0;i<13;i++)
		{
			buf[i]=SPI_MasterRecieve();
		}
		
		SPI_clearCS();
// can mesage assembly		
	msg.ident=buf[0];              //standard ID assebmly                  
	msg.ident=msg.ident<<3;
	msg.ident=msg.ident+(buf[1]>>5);
	msg.RTR=buf[1]>>4;            //RTR flag
	msg.DLC=buf[4];               //Data length code
	
	for(i=0;i<8;i++)              //data part of the CAN message TODO: add DLC check
	{
		if(i<msg.DLC) msg.data[i]=buf[i+5];
		else msg.data[i]=0;
	}
	return msg;                  //returns assembled message
}

//reads status of the MCP2515
//returns status byte:
//0-CANINTF.RX0IF
//1-CANINTFL.RX1IF
//2-TXB0CNTRL.TXREQ
//3-CANINTF.TX0IF
//4-TXB1CNTRL.TXREQ
//5-CANINTF.TX1IF
//6-TXB2CNTRL.TXREQ
//7-CANINTF.TX2IF

uint8_t CAN_status(void)
{
	uint8_t byte;
	SPI_setCS();
	SPI_MasterTransmit(0xB0);
	byte=SPI_MasterRecieve();
	SPI_clearCS();
	return byte;
}

//sets the mode of operation for the MCP2515
//0-normal
//1-sleep
//2-loopback
//3-configuration

void CAN_mode(uint8_t mode)
{
	switch(mode)
	{
		case 0:
		  CAN_Write_single(0x0F,0x00); //normal
		  break;
		case 1:
		  CAN_Write_single(0x0F,0x20); //sleep
		  break;
		case 2:
		  CAN_Write_single(0x0F,0x40); //loopback
		  break;
		case 3:
		  CAN_Write_single(0x0F,0x60); //listen
		  break;
	    case 4:
	      CAN_Write_single(0x0F,0x80); //config
	      break;
		default:
		 break;
	}
}

//checks the current mode of operation of the MCP2515
//look in CAN_mode description

uint8_t CAN_check_mode(void)
{
	uint8_t mode;
	mode=CAN_Read_single(0x0E);
	mode=mode & 0b11100000;
	
		switch(mode)
		{
			case 0x00:
			 return 0;
			 break;
			
			case 0x20:
			 return 1;
			 break;
			
			case 0x40:
			 return 2;
			 break;
			
			case 0x60:
			 return 3;
			 break;
			
			case 0x80:
			 return 4;
		     break;
			
			default:
			 return 255;
			 break;
		}
	
}


/* 
16mhz
125kbps  cnf1=0x03 cnf2=0xB8 cnf3=0x05
250kbps  cnf1=0x01 cnf2=0xB8 cnf3=0x05
500kbps  cnf1=0x00 cnf2=0xB8 cnf3=0x05
20mhz
125kbps  cnf1=0x03 cnf2=0xBA cnf3=0x07
250kbps  cnf1=0x01 cnf2=0xBA cnf3=0x07
500kbps  cnf1=0x00 cnf2=0xBA cnf3=0x07
*/
//sets baud rate of the can bus

void CAN_setBaud(uint8_t baud)
{
	uint8_t buf[3];
	switch(baud)
	{
		case 0: //125
		  buf[0]=0x05;
		  buf[1]=0xB8;
		  buf[2]=0x03;
		  break;
		  
		case 1: //250
		  buf[0]=0x05;
		  buf[1]=0xB8;
		  buf[2]=0x01;
		  break;
		 
		case 2: //500
		  buf[0]=0x05;
		  buf[1]=0xB8;
		  buf[2]=0x00;
		  break;
	}

	CAN_Write(0x28,3,buf);
}


void CAN_setBaud20(uint8_t baud)
{
	uint8_t buf[3];
	switch(baud)
	{
		case 0: //125
		buf[0]=0x07;
		buf[1]=0xBA;
		buf[2]=0x03;
		break;
		
		case 1: //250
		buf[0]=0x07;
		buf[1]=0xBA;
		buf[2]=0x01;
		break;
		
		case 2: //500
		buf[0]=0x07;
		buf[1]=0xBA;
		buf[2]=0x00;
		break;
	}

	CAN_Write(0x28,3,buf);
}



//rx buffer config
//mode:
//0-standard or extended matching filters
//1-extended matching filters
//2-standard matching filters
//3-don't use filters

//rolloaver 1 to enable only for buffer 0;
//buffer 0 or 1

void CAN_rx_cfg(uint8_t mode, uint8_t rollover, uint8_t buffer)
{
	uint8_t buf;
	if(buffer==0)
	{
		buf=mode<<6;
		buf=buf+(rollover<<3);
		CAN_Write_single(0x60,buf);	
	}
    else
	{
		if(rollover>0)CAN_Write_single(0x60,0x04);
		buf=mode<<6;
		CAN_Write_single(0x70,buf);
	}
	
}

//writes a can message on to can network
//msg- can message 
//buffer - message buffer 0,1 or 2

void CAN_TX_Write(CAN_msg msg, uint8_t buffor)
{
	uint8_t buf[13];
	uint8_t i;
	uint8_t address;

	
	address=0x31+(buffor*0x10); //select the message buffer
	
	
	//prepare the SPI data from can message
	buf[0]=msg.ident>>3;
	buf[1]=msg.ident<<5;
	buf[2]=0;
	buf[3]=0;
	buf[4]=msg.RTR<<6;
	buf[4]=buf[4]+msg.DLC;
	
		for(i=0;i<8;i++)
		{
			buf[i+5]=msg.data[i]; //add DLC check
		}
	CAN_Write(address,13,buf);	//write message to buffer
	 
}

//request to send command
//requests the MCP2515 to send the message from buffer
//buff - buffer to request send from 0,1,2 
void CAN_RTS(uint8_t buff) 
{
	uint8_t cmd=0x80;
	SPI_setCS();
	SPI_MasterTransmit(cmd+(1<<buff));
	SPI_clearCS();
}

//check tx buffer status
//buff - buffer number 0,1 or 2
//returns status byte
//0-txreq 1- pending transmition 0- not pending
//1-txerr 1- error transmiting   0- no error
//2-MLOA  1- message lost arbitration 0- message transmited
//3-ABTF  1- message aborted     0-message sent

uint8_t CAN_TX_CHECK(uint8_t buff)
{
	uint8_t status;
	status=CAN_Read_single(0x30+(buff<<4));
	//status=status & 0b00000011;
	status=status>>3;
	return status;	
}


//send message from first free buffer
//returns 0 when succesfull
//returns 255 if not

uint8_t CAN_TX_msg(CAN_msg msg)
{
	uint8_t buff=0;
	while(((CAN_TX_CHECK(buff) & ~0b11111110 ) == 1) & (buff<3))
	{
		buff++;		
	}
	
	if(buff<3)
	{
		CAN_TX_Write(msg,buff);
		CAN_RTS(buff);
		return 0;
	}
	else return 255;
}

//enable interrupt
//int:
//0-rx buffer 0 full
//1-rx buffer 1 full
//2-tx buffer 0 empty
//3-tx buffer 1 empty
//4-tx buffer 2 empty
//5-error
//6-wake
//7-msg error
void CAN_int_en(uint8_t interrupt)
{
	CAN_Write_single(0x2B,interrupt);
	
}

//reads interupts flags
uint8_t CAN_get_intf(void)
{
	return CAN_Read_single(0x2C);
	
}
//gets pending interrupt
uint8_t CAN_get_pint(void)
{
	return (CAN_Read_single(0x0E)>>1) & ~0xF8;
}
//clears flag
//numbers 0 to 7 look above
void CAN_clear_int(uint8_t flag)
{
	uint8_t mask;
	mask=1<<flag;
	CAN_modify(0x2c,mask,0x00);
}


void CAN_sdis_int(uint8_t flag)
{
	uint8_t mask;
	mask=1<<flag;
	CAN_modify(0x2B,mask,0x00);
}

void CAN_sen_int(uint8_t flag)
{
	uint8_t mask;
	mask=1<<flag;
	CAN_modify(0x2B,mask,0xff);
}


void CAN_clear_all_int(void)
{
	CAN_Write_single(0x2c,0);
}

void CAN_set_int(uint8_t flag)
{
	uint8_t mask;
	mask=1<<flag;
	CAN_modify(0x2C,mask,0xff);
}