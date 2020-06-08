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
#include <avr/interrupt.h>
#include <USBAPI.h>
#include <stdio.h>

#include "watchdog.h"


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

// General TWI Master status codes
#define TWI_START                  0x08  // START has been transmitted
#define TWI_REP_START              0x10  // Repeated START has been transmitted
#define TWI_ARB_LOST               0x38  // Arbitration lost

// TWI Master Transmitter status codes
#define TWI_MTX_ADR_ACK            0x18  // SLA+W has been transmitted and ACK received
#define TWI_MTX_ADR_NACK           0x20  // SLA+W has been transmitted and NACK received
#define TWI_MTX_DATA_ACK           0x28  // Data byte has been transmitted and ACK received
#define TWI_MTX_DATA_NACK          0x30  // Data byte has been transmitted and NACK received

// TWI Master Receiver status codes
#define TWI_MRX_ADR_ACK            0x40  // SLA+R has been transmitted and ACK received
#define TWI_MRX_ADR_NACK           0x48  // SLA+R has been transmitted and NACK received
#define TWI_MRX_DATA_ACK           0x50  // Data byte has been received and ACK transmitted
#define TWI_MRX_DATA_NACK          0x58  // Data byte has been received and NACK transmitted

// TWI Slave Transmitter status codes
#define TWI_STX_ADR_ACK            0xA8  // Own SLA+R has been received; ACK has been returned
#define TWI_STX_ADR_ACK_M_ARB_LOST 0xB0  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
#define TWI_STX_DATA_ACK           0xB8  // Data byte in TWDR has been transmitted; ACK has been received
#define TWI_STX_DATA_NACK          0xC0  // Data byte in TWDR has been transmitted; NOT ACK has been received
#define TWI_STX_DATA_ACK_LAST_BYTE 0xC8  // Last data byte in TWDR has been transmitted (TWEA = �0�); ACK has been received

// TWI Slave Receiver status codes
#define TWI_SRX_ADR_ACK            0x60  // Own SLA+W has been received ACK has been returned
#define TWI_SRX_ADR_ACK_M_ARB_LOST 0x68  // Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
#define TWI_SRX_GEN_ACK            0x70  // General call address has been received; ACK has been returned
#define TWI_SRX_GEN_ACK_M_ARB_LOST 0x78  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_ACK       0x80  // Previously addressed with own SLA+W; data has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_NACK      0x88  // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
#define TWI_SRX_GEN_DATA_ACK       0x90  // Previously addressed with general call; data has been received; ACK has been returned
#define TWI_SRX_GEN_DATA_NACK      0x98  // Previously addressed with general call; data has been received; NOT ACK has been returned
#define TWI_SRX_STOP_RESTART       0xA0  // A STOP condition or repeated START condition has been received while still addressed as Slave

// TWI Miscellaneous status codes
#define TWI_NO_STATE               0xF8  // No relevant state information available; TWINT = �0�
#define TWI_BUS_ERROR              0x00  // Bus error due to an illegal START or STOP condition

#define ASM_NOP_TIMES				2

#ifndef MAX_BUFFER_LENGTH
	#define MAX_BUFFER_LENGTH		10
#endif

void initTwi(void);
uint8_t twiWrite(const uint8_t data[], uint8_t size);
uint8_t twiRead(uint8_t *data, uint8_t writeSize, uint8_t readSize);
uint8_t twiReadRS(uint8_t *data, uint8_t writeSize, uint8_t readSize);

#endif /* TWI_H_ */