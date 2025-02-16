/*
 * tdef.h
 *
 * Created: 2016-03-30 00:20:34
 *  Author: Ja
 */ 
#include <inttypes.h>


#ifndef TDEF_H_
#define TDEF_H_

  typedef struct{
	  unsigned    ident    :11;   /* Standard Identifier */
	  unsigned    RTR      :1;    /* Remote Transmission Request bit */
	  unsigned    DLC      :4;    /* Data length code (bits 0...3) */
	  uint8_t     data[8];        /* 8 data bytes */
  }CAN_msg;
  
  typedef struct{
	  unsigned long   ident    :29;   /* Extended Identifier */
	  unsigned    RTR      :1;    /* Remote Transmission Request bit */
	  unsigned    DLC      :4;    /* Data length code (bits 0...3) */
	  uint8_t     data[8];        /* 8 data bytes */
  }CANB_msg;
    



#endif /* TDEF_H_ */