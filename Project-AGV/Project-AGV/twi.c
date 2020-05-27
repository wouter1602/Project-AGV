/*
 * twi.c
 *
 * Created: 25-5-2020 22:15:07
 *  Author: Wouter
 */ 

#include "twi.h"

//TWI max 100KHz
// SCL = (F_CPU / (16+2(TWBR)*4^(TWPS))
//TWBR should be higher than 10
#ifdef DEBUG
static inline void twiError(uint8_t Code) {
	printf("Error %d, \tTWCR = 0x%x\n", Code, (TWCR & 0xF8));
}
#else
static inline void twiError(uint8_t code) {
	
}
#endif


static uint8_t sendACKAddrs(void) {
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) != MR_SLA_ACK) {
		//error
		return 1;
	}
	return 0;
}

static uint8_t sendACKData(void) {
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) != MR_DATA_ACK) {
		//error
		return 1;
	}
	return 0;
}

static uint8_t waitForTransmitACKAddrs(void) {
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) != MT_SLA_ACK) {
		twiError(1);
		return 1;
	}
	return 0;
}

static uint8_t waitForTransmitACKData(void) {
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) != MT_DATA_ACK) {
		twiError(2);
		return 1;
	}
	return 0;
}

static void sendStart(void) {
	TWCR |= (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);	//Send start
	while (!(TWCR & (1 << TWINT)));			//Wait for TWINT flag, if Flag set start bit has been send
	
	if ((TWSR & 0xF8) != START)
	{
		twiError(0);
	}
}

static void sendStop(void) {
	TWCR |= (1 << TWINT) | (1 << TWSTO);	//send stop
	
	while (!(TWCR & (1 << TWINT)));			//Wait for TWINT flag, if Flag set stop bit has been send
	twiError(6);
}

static void sendByteTwi(uint8_t data) {
	
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	printf("Data send: 0x%x\n", TWDR);
}

static uint8_t receiveByteTwi(void) {
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	sendACKData();
	return TWDR;
}

void initTwi(void) {
	TWBR = 18; // Set SCL frequency
	TWSR = (0 << TWPS0) | (0 << TWPS1); //set SCL prescaler to 1
	//SCL = (160000000 / (16+2*18*4^(1)))
	//SCL is set to 100 KHz
	
	//TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE); //Enable ACK, enable TWI, enable TWI interrupt
}

void twiWrite(uint8_t addrs, uint8_t reg, uint8_t data) {
	/*
	sendStart();
	sendByteTwi((addrs & ~1));
	waitForTransmitACKAddrs();
	sendByteTwi(reg);
	waitForTransmitACKData();
	sendByteTwi(data);
	waitForTransmitACKAddrs();
	sendStop();*/
	twiError(5);
	sendStart();
	sendByteTwi((addrs & ~1));
	waitForTransmitACKAddrs();
	sendStop();
	_delay_ms(500);
	/*for (int i = 0; i < 10; ++i) {
		sendStart();
		sendByteTwi((addrs & ~1));
		sendStop();
	}*/
}

uint8_t twiRead(uint8_t addrs, uint8_t reg) {
	uint8_t data = 0;
	//order, Start, addr + W,			Reg,		SR, addr+ R,				NACK, Stop
	//							ACK,			ACK,				Ack, Data
	sendStart();
	sendByteTwi((addrs & ~1));
	waitForTransmitACKAddrs();
	sendByteTwi(reg);
	waitForTransmitACKAddrs();
	sendStart();
	sendByteTwi((addrs | 1));
	sendACKAddrs();
	data = receiveByteTwi();
	sendStop();
	return data;
}