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
#include <USBAPI.h>
#include <stdio.h>


#define START			0x08	//TWI start
#define REP_START		0x10	//TWI repeat start
#define MT_SLA_ACK		0x18	//SLA+W has been transmitted, ACK was received
#define MT_SLA_NACK		0x20	//SLA+W has been transmitted, NACK was received
#define MT_DATA_ACK		0x28	//data was transmitted, ACK received
#define MT_DATA_NACK	0x30	//data was transmitted, NACk received
#define READ_ONLY		0x00	//Read only pointer register mode
#define MR_SLA_ACK		0x40	//SLA+R has been transmitted, ACK was received
#define MR_SLA_NACK		0x48	//SLA+R has been transmitted, NACK was received
#define MR_DATA_ACK		0x50	//data was received, ACK send
#define MR_DATA_NACK	0x58	//data was received, NACK send


void initTwi(void);
void twiWrite(uint8_t addrs, uint8_t reg, uint8_t data);
uint8_t twiRead(uint8_t addrs, uint8_t reg);

#endif /* TWI_H_ */