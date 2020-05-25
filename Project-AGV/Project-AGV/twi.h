/*
 * twi.h
 *
 * Created: 25-5-2020 22:15:15
 *  Author: Wouter
 */ 


#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#include <util/delay.h>

void initTwi(void);
void twiWrite(uint8_t addrs, uint8_t reg, uint8_t data);
uint8_t twiRead(uint8_t addrs, uint8_t reg);

#endif /* TWI_H_ */