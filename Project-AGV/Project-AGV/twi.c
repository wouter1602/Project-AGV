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

static volatile struct TwiData {
	uint8_t lenght;
	uint8_t dataIndex;
	uint8_t retries;
	uint8_t error;
	uint8_t data[ MAX_BUFFER_LENGTH];
} twiData;

/*
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
	
	twiError(6);
}

static void sendByteTwi(uint8_t data) {
	
	TWDR = data;
	TWCR |= (1 << TWINT) | (1 << TWEN);
	//printf("Data send: 0x%x\n", TWDR);
}

static uint8_t receiveByteTwi(void) {
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	sendACKData();
	return TWDR;
}*/

static inline uint8_t twiTransceiverBusy( void ) {
	return ( TWCR & (1<<TWIE) );                  // IF TWI Interrupt is enabled then the Transceiver is busy
}

void initTwi(void) {
	/*
	TWBR = 18; // Set SCL frequency
	TWSR = 0;
	TWSR = (0 << TWPS0) | (0 << TWPS1); //set SCL prescaler to 1
	TWAR &= ~(1 << TWGCE);*/
	TWBR = 72;
	TWSR = 0;
	TWDR = 0xFF;
	TWCR = (1 << TWEN) | (0 << TWIE) | (0 << TWWC) | (0 << TWSTO) | (0 << TWSTA) | (0 << TWEA) | (0 << TWINT); 
	twiData.lenght = 0;
	twiData.dataIndex = 0;
	errorNt = 0;
	twiData.error = 0;
	DDRC |= (1 << DDC7);
	PORTD |= (1 << PORTD0) | (1 << PORTD1);
	
}
/*
void twiWrite(uint8_t addrs, uint8_t reg, uint8_t data) {
	/*
	sendStart();
	sendByteTwi((addrs & ~1));
	waitForTransmitACKAddrs();
	sendByteTwi(reg);
	waitForTransmitACKData();
	sendByteTwi(data);
	waitForTransmitACKAddrs();
	sendStop();
	twiError(5);
	sendStart();
	sendByteTwi((addrs & ~1));
	waitForTransmitACKAddrs();
	sendByteTwi(data);
	waitForTransmitACKData();
	sendStop();
	_delay_ms(500);
	/*for (int i = 0; i < 10; ++i) {
		sendStart();
		sendByteTwi((addrs & ~1));
		sendStop();
	}
}*/

/*
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
}*/

static uint8_t twiWriteError(void) {
	twiData.dataIndex = 0;
	while (twiTransceiverBusy());
	
	TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC);	//Send start bit
	return 0;
}

void twiWatchdog(void) {
	if (twiData.error && twiData.retries < 2) {
		printf("Failed\n");
		twiWriteError();
		twiData.retries++;
		twiData.error = 0;
	}
	if (twiData.retries > 2) {
		printf("Failed to many times\n");
		twiData.error = 0;
		twiData.retries = 0;
	}
}

uint8_t twiWrite(uint8_t *data, uint8_t size) {
		if (size > MAX_BUFFER_LENGTH) {		//Data too big for register
			return 1;
		}
		
		twiData.lenght = size;
		
		for (int i = 0; i < size; size++) {	//Copy data in buffer
			twiData.data[i] = data[i];
		}
		
		PORTC |= (1 << PORTC7);
		while (twiTransceiverBusy());		//Wait for TWI is ready for next transmission
		
		//TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC);	//Send stop bit
		
		//while (twiTransceiverBusy());		//Wait for TWI is ready for next transmission
		printf("Error\t5, \tTWCR = 0x%x\n", (TWCR & 0xF8));
		PORTC &= ~(1 << PORTC7);
		
		TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC);	//Send start bit
		printf("Started you know\n");
		return 0;
}

/*
uint8_t twiTest(uint8_t addrs, uint8_t data) {
	
	while (TWCR & (1 <<TWIE));	//wait for TWI to be free
	
	
	TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN)) & ~(1 << TWSTO); //send START
	
	while (!(TWCR & (1 <<TWINT))); //wait for START ACK
	
	if ((TWCR & 0xF8) != START) { //Check if ACK = correct
		twiError(0);
	} else {
		twiSucces(0);
	}
	
	TWDR = (addrs & ~1);				//send ADDRES Write
	TWCR = (1 << TWINT) | (1 << TWEN);	//Start TWI
	
	while (!(TWCR & (1 << TWINT))); //Wait for ACK
	
	if ((TWSR & 0xF8) != MT_SLA_ACK) {		//Check if ACK = correct
		twiError(1);
	} else {
		twiSucces(1);
	}
	
	TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWSTO)) & ~(1 << TWSTA); //send Stop bit
	twiSucces(3);
	/*
	TWDR = data;					//send data
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	while (!(TWCR & (1 << TWINT)));	//Wait for ACK
	
	if ((TWSR & 0xF8) != MT_DATA_ACK) {
		twiError(2);
	} else {
		twiSucces(2);
	}
	/*
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); //send Stop bit
	twiSucces(3);
	
	return 0;
}*/

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
			if (twiData.dataIndex < twiData.lenght) {
				printf("%d\n", TWSR);
				TWDR = twiData.data[twiData.dataIndex++];
				printf("%d\n", TWDR);
				TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC);	//Enable TWI
			} else {
				errorNt = 20;					//Send STop after last byte
				TWCR = (1 << TWEN) | (0 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC);	//Send stop bit
			}
			
		break;
		case TWI_MRX_ADR_ACK:		//Data byte has been received and ACK transmitted
			twiData.data[twiData.dataIndex++] = TWDR;
		case TWI_MRX_DATA_ACK:
			errorNt = 3;
			if (twiData.dataIndex < (twiData.lenght - 1)) {			//Check if more bytes need to be received
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
			printf("%d\n", TWSR);
		break;
		case TWI_MRX_ADR_NACK:
		case TWI_MTX_ADR_NACK:
		case TWI_MTX_DATA_NACK:
			if (twiData.retries < 2) {
				//printf("R%d\n", TWSR);
				//TWDR = twiData.data[twiData.dataIndex -1];
				//TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC);	//Enable TWI
				//twiData.retries++;
				TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC);	//Send stop bit
				twiData.error = 1;
				break;
			}
		case TWI_BUS_ERROR:
		default:
			//twiError(5);
			
			TWCR = (1 << TWEN) | (0 << TWIE) | (0 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC);	//reset TWI interface
			printf("Q%d\n", TWSR);
			errorNt = 4;
		break;
	}
}