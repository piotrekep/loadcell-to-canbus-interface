  #include <inttypes.h>
  #include "../types/tdef.h"
  
//   typedef struct{
// 	    unsigned    ident    :11;   /* Standard Identifier */
// 	    unsigned    RTR      :1;    /* Remote Transmission Request bit */
// 	    unsigned    DLC      :4;    /* Data length code (bits 0...3) */
// 	    uint8_t     data[8];        /* 8 data bytes */
// 	    unsigned    crc      :16;   /* CRC */
//     }CAN_msg;
//     
	
void CAN_reset(void);
void CAN_Read(uint8_t startadres, uint8_t BytesToRead, uint8_t *buf);
uint8_t CAN_Read_single(uint8_t startadress);
void CAN_Write(uint8_t startadres, uint8_t BytesToWrite, uint8_t *buf);
void CAN_Write_single(uint8_t startadress,uint8_t byte);
void CAN_modify(uint8_t startadress, uint8_t mask, uint8_t data);
CAN_msg CAN_RX_Read(uint8_t buff_number);
uint8_t CAN_status(void);
void CAN_mode(uint8_t mode);
uint8_t CAN_check_mode(void);
void CAN_setBaud(uint8_t baud);
void CAN_setBaud20(uint8_t baud);
void CAN_rx_cfg(uint8_t mode, uint8_t rollover, uint8_t buffer);
void CAN_TX_Write(CAN_msg msg, uint8_t buffor);
void CAN_RTS(uint8_t buff);
uint8_t CAN_TX_CHECK(uint8_t buff);
uint8_t CAN_TX_msg(CAN_msg msg);
void CAN_int_en(uint8_t interrupt);
uint8_t CAN_get_intf(void);
uint8_t CAN_get_pint(void);
void CAN_clear_int(uint8_t flag);
void CAN_sdis_int(uint8_t flag);
void CAN_sen_int(uint8_t flag);
void CAN_clear_all_int(void);
void CAN_set_int(uint8_t flag);