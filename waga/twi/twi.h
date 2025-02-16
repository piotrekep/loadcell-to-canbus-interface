/*
 * twi.h
 *
 * Created: 2015-02-19 00:04:42
 *  Author: Ja
 */ 
#include <inttypes.h>

void TWIInit(void);
void TWIStart(void);
void TWIStop(void);
void TWIWrite(uint8_t u8data);
uint8_t TWIReadACK(void);
uint8_t TWIReadNACK(void);
uint8_t TWIGetStatus(void);