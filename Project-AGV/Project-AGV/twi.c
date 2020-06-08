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
static inline void twiError(uint8_t code) {
	printf("Error\t%d, \tTWCR = 0x%x\n", code, (TWCR & 0xF8));
}

static inline void twiSucces(uint8_t code) {
	printf("Success\t%d, \tTWCR = 0x%x\n", code, (TWCR & 0xF8));
}
#else
static inline void twiError(uint8_t code) {
	
}

static inline void twiSucces(uint8_t code) {
	
}
#endif

static inline void asmNop(uint8_t times) {
	for (uint8_t i = 0; i < times; i++) {
		asm("nop");
	}
}

static volatile struct TwiData {
	uint8_t length;
	uint8_t dataIndex;
	uint8_t retries;
	uint8_t error;
	uint8_t data[ MAX_BUFFER_LENGTH];
    uint8_t watchdogOverflow;
} twiData;

static inline uint8_t twiTransceiverBusy( void ) {
	return ((TWCR & (1<<TWIE)) && (twiData.watchdogOverflow == 0));                  // IF TWI Interrupt is enabled then the Transceiver is busy
}

static uint8_t twiWriteError(void) {
	twiData.dataIndex = 0;
	while (twiTransceiverBusy());
	
	TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC);	//Send start bit
	return 0;
}

void initTwi(void) {
	TWBR = 72;
	TWSR = 0;
	TWDR = 0xFF;
	TWCR = (1 << TWEN) | (0 << TWIE) | (0 << TWWC) | (0 << TWSTO) | (0 << TWSTA) | (0 << TWEA) | (0 << TWINT);
	twiData.length = 0;
	twiData.dataIndex = 0;
	errorNt = 0;
	twiData.error = 0;
	DDRC |= (1 << DDC7);
	PORTD |= (1 << PORTD0) | (1 << PORTD1);
    
    //Setup watchdog timer for overflow
	twiData.watchdogOverflow = 0;
	initWatchdog();
    
}

/*
void twiWatchdog(void) {
	if (twiData.error && twiData.retries < 2) {
		#ifdef DEBUG
		printf("Failed\t0x%x\n",TWSR);
		#else 
		printf("Failed\t0x%x\n",TWSR);
		#endif
		twiData.retries++;
		twiData.error = 0;
		twiWriteError();
		
	}
	if (twiData.retries > 2) {
		#ifdef DEBUG
		printf("Failed to many times\n");
		#else
		printf("Failed to many times\n");
		#endif
		twiData.error = 0;
		twiData.retries = 0;
	}
}*/

uint8_t twiWrite(const uint8_t *data, uint8_t size) {
	if (size > MAX_BUFFER_LENGTH) {		//Data too big for register
		return 1;
	}
	
	twiData.length = size;
	
	for (int i = 0; i < size; i++) {	//Copy data in buffer
		twiData.data[i] = data[i];
	}
    
    twiData.watchdogOverflow = 0;
    startWatchdog((uint8_t*) &twiData.watchdogOverflow);
	while (twiTransceiverBusy());		//Wait for TWI is ready for next transmission
    stopWatchdog();
    if (twiData.watchdogOverflow == 1) {
        #ifdef DEBUG
        printf("Took to long to wait for empty TWI lane.\nERROR: 0x%x", (TWCR & 0xF8));
        #endif
        return 2;
        
    }
	
	TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC);	//Send start bit
	return 0;
}

ISR(TWI_vect){
	switch (TWSR & 0xF8) {
		case TWI_START:
		//start signal
		twiData.dataIndex = 0;		//reset index
		//twiSucces(0);
		errorNt = 1;
		case TWI_REP_START:
		case TWI_MTX_ADR_ACK:			//ACK on sending address
		case TWI_MTX_DATA_ACK:			//ACK on sending Data
		errorNt = 1;
		twiData.retries = 0;
		if (twiData.dataIndex < twiData.length) {
			#ifdef DEBUG
			printf("R\t0x%x\n", TWSR);
			#else
			asmNop(ASM_NOP_TIMES);
			#endif
			TWDR = twiData.data[twiData.dataIndex++];
			#ifdef DEBUG
			//printf("D0x%x\n", TWDR);
			printf("DS\t0x%x\n", twiData.data[twiData.dataIndex -1]);
			#endif
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC);	//Enable TWI
			} else {		//Send STop after last byte
			#ifdef DEBUG
			printf("RE0\tx%x\n", TWSR);
			#else
			asmNop(ASM_NOP_TIMES);
			#endif
			TWCR = (1 << TWEN) | (0 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC);	//Send stop bit
		}
		
		break;
		case TWI_MRX_ADR_ACK:		//Data byte has been received and ACK transmitted
		twiData.data[twiData.dataIndex++] = TWDR;
		case TWI_MRX_DATA_ACK:
		errorNt = 3;
		if (twiData.dataIndex < (twiData.length - 1)) {			//Check if more bytes need to be received
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send ACK and wait for next bit
			} else {		//Last data byte received
			errorNt = 20;
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC); //Send NACK and stop bit
		}
		break;
		case TWI_MRX_DATA_NACK:		//Data byte has been received and NACK transmitted
		twiData.data[twiData.dataIndex++] = TWDR;
		TWCR = (1 << TWEN) | (0 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC); //Send stop bit
		break;
		case TWI_SRX_ADR_DATA_ACK:	//Ignored
		#ifdef DEBUG
		printf("R\t0x%x\n", TWSR);
		#else
		asmNop(ASM_NOP_TIMES);
		#endif
		break;
		case TWI_MRX_ADR_NACK:
		case TWI_MTX_ADR_NACK:
		case TWI_MTX_DATA_NACK:
		if (twiData.retries < 2) {
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC);	//Send stop bit
			twiData.error = 1;
			break;
		}
		case TWI_BUS_ERROR:
		default:
		//twiError(5);
		
		TWCR = (1 << TWEN) | (0 << TWIE) | (0 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC);	//reset TWI interface
		#ifdef DEBUG
		printf("RQ\t0x%x\n", TWSR);
		#else
		asmNop(ASM_NOP_TIMES);
		#endif
		errorNt = 4;
		break;
	}
}