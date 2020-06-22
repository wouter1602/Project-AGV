/*
 * twi.c
 *
 * Created: 25-5-2020 22:15:07
 *  Author: Wouter
 */

#include "twi.h"


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

/*
 * This function adds "asm("nop");" to places where the Arduino is to fast for the communication.
 * asm("nop") is an function that literally does nothing for a clock cycle.
 */
static inline void asmNop(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        asm("nop");
    }
}

/*
 * Static struct needed for the TWI coeducation.
 */
static volatile struct TwiData {
    uint8_t length;
    uint8_t dataIndex;
    uint8_t retries;
    uint8_t error;
    uint8_t data[ MAX_BUFFER_LENGTH];
    uint8_t watchdogOverflow;
    uint8_t rs : 1;
} twiData;

/*
 * Static inline function that returns if the TWI communication is busy or if the watchdog has overflown.
 * It returns 1 if it is still busy and the watchdog isn't triggert.
 * It returns 0 if the communication is done or the watchdog is triggert.
 */
static inline uint8_t twiTransceiverBusy(void) {
    return ((TWCR & (1 << TWIE)) && (twiData.watchdogOverflow == 0)); // IF TWI Interrupt is enabled then the Transceiver is busy
}

/*
 * Setup an TWI compunction at 100kHz
 * SCL = (F_CPU / (16+2(TWBR)*4^(TWPS))
 * TWBR should be higher than 10
 * Set all the struct value's to zero
 */
void initTwi(void) {
    TWBR = 72;
    TWSR = 0;
    TWDR = 0xFF;
    TWCR = (1 << TWEN) | (0 << TWIE) | (0 << TWWC) | (0 << TWSTO) | (0 << TWSTA) | (0 << TWEA) | (0 << TWINT);		//Enable TWI interrupt
    twiData.length = 0;
    twiData.dataIndex = 0;
    twiData.error = 0;
    twiData.rs = 0;
    DDRC |= (1 << DDC7);
    PORTD |= (1 << PORTD0) | (1 << PORTD1);

    //Setup watchdog timer for overflow
    twiData.watchdogOverflow = 0;
    initWatchdog();			//Makes sure the watchdog can be used if the communication takes too long
}

/*
 * Writes what's in the unsigned 8-bit data[] array to the TWI pins.
 * array can't be bigger than the "MAX_BUFFER_LENGTH" define.
 * the first byte in the array should be the address to the slave you want to talk to.
 * The rest of the data is up to datasheet of the IC that you communicate with.
 * Returns 0 if it stared successfully.
 * Returns 1 if the array is too big.
 * Returns 2 if the TWI lane isn't clear within a reasonable time.
 * Returns 3 if the IC address is in read mode.
 * Communication after the start is send is handled in ISR(TWI_vect)
 */
uint8_t twiWrite(const uint8_t *data, uint8_t size) {
    if (size > MAX_BUFFER_LENGTH) {								//Data too big for register
#ifdef DEBUG
        printf("twiWrite data too big\n");
#endif
        return 1;
    }

    twiData.watchdogOverflow = 0;
    startWatchdog((uint8_t*) &twiData.watchdogOverflow);		//Start watchdog for if it takes to long to wait for a empty TWI lane.
    while (twiTransceiverBusy());								//Wait for TWI is ready for next transmission
    stopWatchdog();												//Stop watchdog when done waiting
    if (twiData.watchdogOverflow == 1) {						//Return 2 if it took too long
#ifdef DEBUG
        printf("Took to long to wait for empty TWI lane.\nERROR: 0x%x", (TWCR & 0xF8));
#endif
        return 2;
    }

    twiData.length = size;										//Set array length.

    if ((data[0] & 1) == 1) {									//Return 3 if address is read command
        return 3;
    }
	
    twiData.rs = 0;												//Disables the Read resend start function
    
	for (int i = 0; i < size; i++) { 
        twiData.data[i] = data[i];								//Copy data in buffer
    }

    TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send start bit
    return 0;
}

/*
 * Read from IC and puts it in the unsigned 8-bit data[] array.
 * data[] should have the length to store the data you are trying to read +1.
 * writeSize is the amount of bytes you want to write to the IC before reading.
 * e.g.
 * data[0] = address (write mode)
 * data[1] = register you want to read from
 * data[2] = address (read mode)
 * For this example the writeSize is 2 and the readSize can't be greater than 3.
 * Some IC don't need a register you want to read from then you can use this example.
 * e.g.
 * data[0] = address (read mode)
 * data[1] = [empty will be filed with read data]
 * data[2] = [empty will be filed with read data]
 * data[3] = [empty will be filed with read data]
 * For this example the writeSize is 0 and the readSize can't be greater than 4.
 * readSize is the amount of bytes the IC is going to send back and needs to be put in the array.
 * The readSize should also count the address of the IC as a byte it's going to read. This could be used to verify it has used the correct address.
 * When first writing and afterward reading a stop bit is send in between.
 * If your IC is not expecting a stop bit between selecting a register and reading its data use the "twiReadRS()" function.
 * Returns 0 if it read data successfully.
 * Returns 1 if the array send with this function is bigger than MAX_BUFFER_LENGHT.
 * Returns 2 if the TWI lane isn't clear within a reasonable time.
 * Returns 3 if it took too long to write to IC.
 * Returns 4 if it took too long to read from IC.
 */
uint8_t twiRead(uint8_t *data, uint8_t writeSize, uint8_t readSize) {
    if (max(readSize, writeSize) > MAX_BUFFER_LENGTH) {			//Data too big for register
#ifdef DEBUG
        printf("twiRead data too big\n");
#endif
        return 1;
    }
	
    twiData.watchdogOverflow = 0;
    startWatchdog((uint8_t*) & twiData.watchdogOverflow);		//Start watchdog for if it takes to long to wait for an empty TWI lane
    while (twiTransceiverBusy());								//Wait for TWI is ready for next transmission
    stopWatchdog();												//Stop watchdog when done waiting
    if (twiData.watchdogOverflow == 1) {						//Return 2 when it took too long
#ifdef DEBUG
        printf("Took to long to wait for empty TWI lane.\nERROR: 0x%x", (TWCR & 0xF8));
#endif
        return 2;
    }
	
    twiData.rs = 0;												//Disable the Read resend start function
    if ((data[0] & 1) == 0) {									//first byte is a write address
        twiData.length = writeSize;
        for (int i = 0; i < writeSize; i++) {
            twiData.data[i] = data[i];							//Copy data in buffer
        }
		
        TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send start bit
        
		twiData.watchdogOverflow = 0;
        startWatchdog((uint8_t*) & twiData.watchdogOverflow);	//Start watchdog for if it takes too long to wait for sending the data.
        while (twiTransceiverBusy());							//Wait for TWI is ready for next transmission
        stopWatchdog();											//Stop watchdog when done waiting
        if (twiData.watchdogOverflow == 1) {					//Return 3 when it took too long
#ifdef DEBUG
            printf("Took to long to write to IC.\nERROR: 0x%x", (TWCR & 0xF8));
#endif
            return 3;
        }
    }

    twiData.data[0] = data[writeSize];							//Set IC address in read mode to the front of the array
    twiData.length = readSize;									//Set the amount of bytes it has to read from the IC

    TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send start bit
    twiData.watchdogOverflow = 0;
    startWatchdog((uint8_t*) & twiData.watchdogOverflow);		//Start watchdog for if it takes too long to wait for all the data to be read.
    while (twiTransceiverBusy());								//Wait for TWI is ready for next transmission
    stopWatchdog();												//Stop watchdog when done waiting
    if (twiData.watchdogOverflow == 1) {						//Return 4 when it took too long
#ifdef DEBUG
        printf("Took to long to read from IC.\nERROR: 0x%x", (TWCR & 0xF8));
#endif
        return 4;
    }
    for (uint8_t i = 0; i < readSize; i++) {
        data[i] = twiData.data[i];								//Put gathered data in original array
    }
    return 0;
}

/*
 * Read from IC and puts it in the unsigned 8-bit data[] array.
 * In between writing to the IC and reading from the IC a restart bit is send.
 * data[] should have the length to store the data you are trying to read +1.
 * writeSize is the amount of bytes you want to write to the IC before reading.
 * e.g.
 * data[0] = address (write mode)
 * data[1] = register you want to read from
 * data[2] = address (read mode)
 * For this example the writeSize is 2 and the readSize can't be greater than 3.
 * Some IC don't need a register you want to read from then you can use this example.
 * e.g.
 * data[0] = address (read mode)
 * data[1] = [empty will be filed with read data]
 * data[2] = [empty will be filed with read data]
 * data[3] = [empty will be filed with read data]
 * For this example the writeSize is 0 and the readSize can't be greater than 4.
 * readSize is the amount of bytes the IC is going to send back and needs to be put in the array.
 * The readSize should also count the address of the IC as a byte it's going to read. This could be used to verify it has used the correct address.
 * When first writing and afterward reading a stop bit is send in between.
 * If your IC is expecting a stop bit between selecting a register and reading its data use the "twiRead()" function.
 * Returns 0 if it read data successfully.
 * Returns 1 if the array send with this function is bigger than MAX_BUFFER_LENGHT.
 * Returns 2 if the TWI lane isn't clear within a reasonable time.
 * Returns 4 if it took too long to read from IC.
 */
uint8_t twiReadRS(uint8_t *data, uint8_t writeSize, uint8_t readSize) {
    if ((max(writeSize, readSize) + 1) > MAX_BUFFER_LENGTH) {	//Data too big for register
#ifdef DEBUG
        printf("twiWrite data too big\n");
#endif
        return 1;
    }

    twiData.watchdogOverflow = 0;
    startWatchdog((uint8_t*) & twiData.watchdogOverflow);		//Start watchdog for if it takes to long to wait for an empty TWI lane
    while (twiTransceiverBusy());								//Wait for TWI is ready for next transmission
    stopWatchdog();												//Stop watchdog when done waiting
    if (twiData.watchdogOverflow == 1) {						//Return 2 when it took too long
#ifdef DEBUG
        printf("Took to long to wait for empty TWI lane.\nERROR: 0x%x", (TWCR & 0xF8));
#endif
        return 2;
    }

    if ((data[0] & 1) == 0) {									//first byte is a write byte
        twiData.length = writeSize;
        for (int i = 0; i < writeSize + 1; i++) {
            twiData.data[i] = data[i];							//Copy data in buffer
        }
		
        twiData.data[(writeSize + 1)] = readSize;				//Add the readSize to the buffer.
        twiData.rs = 1;											//Enable read resend start
        TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send start bit
    }

    twiData.watchdogOverflow = 0;
    startWatchdog((uint8_t*) & twiData.watchdogOverflow);		//Start watchdog for if it takes to long to wait for all the data to be read.
    while (twiTransceiverBusy());								//Wait for TWI is ready for next transmission
    stopWatchdog();												//Stop watchdog when done waiting
    if (twiData.watchdogOverflow == 1) {						//Return 4 when it took too long
#ifdef DEBUG
        printf("Took to long to read from IC.\nERROR: 0x%x", (TWCR & 0xF8));
#endif
        return 4;
    }
	
#ifdef DEBUG
    printf("Data in array:\n");
#endif
    for (uint8_t i = 0; i < readSize; i++) {
#ifdef DEBUG
        printf("0x%x\t", twiData.data[i]);
#endif
        data[i] = twiData.data[i];								//Put gathered data in original array
    }
#ifdef DEBUG
    printf("\n");
#endif
    return 0;
}

/*
 * Interrupt vector for the TWI lane of the Arduino
 * This interrupt gets triggert when the TWI is done sending or receiving one byte or
 * when the TWI is done sending a start, stop or resend start bit.
 * When writing data it sends all the data in the struct "twiData.data[]".
 * When reading it will put all the data in the struct "twiData.data[]".
 * TWSR register is the register where all the TWI error codese can be read.
 * This will go though all the possible functions and send or receive what is needed next.
 */
ISR(TWI_vect) {
    switch (TWSR & 0xF8) {
        case TWI_START:										//start signal
        case TWI_REP_START:									//resend start signal
            twiData.dataIndex = 0;							//reset index
        case TWI_MTX_ADR_ACK:								//ACK on sending address
        case TWI_MTX_DATA_ACK:								//ACK on sending Data
            twiData.retries = 0;							//Set retries for sending a byte back to 0
            if (twiData.dataIndex < twiData.length) {		//Check if there needs to be send more data
#ifdef DEBUG
                printf("R\t0x%x\n", TWSR);
#else
                asmNop(ASM_NOP_TIMES);
#endif
                TWDR = twiData.data[twiData.dataIndex++];	//put data in TWDR register to be send over the TWI lane
#ifdef DEBUG
                printf("DS\t0x%x\n", twiData.data[twiData.dataIndex - 1]);
#endif

                TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Enable TWI
            } else if (twiData.rs == 1) {					//All write bit's have been send and a resend start bit should be send
#ifdef DEBUG
                printf("Rr\t0x%x\n", TWSR);
#endif
                twiData.data[0] = twiData.data[twiData.length];//Put the address in read mode on location 0
#ifdef DEBUG
                printf("Dr\t0x%x\n", twiData.data[0]);
#endif
                twiData.length = twiData.data[(twiData.length + 1)];//Make the lenght of the array the same as read that was send in twiReadRS();
                twiData.rs = 0;								//Disable read resend start
                TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send restart-bit
            } else {										//Send stop after last byte
#ifdef DEBUG
                printf("RE\t0x%x\n", TWSR);
#else
                asmNop(ASM_NOP_TIMES);
#endif
                TWCR = (1 << TWEN) | (0 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC); //Send stop bit
            }

            break;
        case TWI_MRX_DATA_ACK:								//Data byte has been received and ACK transmitted
            twiData.data[twiData.dataIndex++] = TWDR;		//Put received data in twiData.data struct
        case TWI_MRX_ADR_ACK:								//Address read has been transmitted and ACK was received

#ifdef DEBUG
            printf("DR\t0x%x\n", twiData.data[(twiData.dataIndex - 1)]);
#else
            asmNop(ASM_NOP_TIMES);
#endif
            if (twiData.dataIndex < (twiData.length - 1)) { //Check if more bytes need to be received
#ifdef DEBUG
				printf("Ri\t0x%x\n", TWSR);
#endif
                TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send ACK and wait for next bit


            } else {										//Last data byte received
#ifdef DEBUG
                printf("Re\t0x%x\n", TWSR);
#endif
                TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send NACK bit
            }
            break;
        case TWI_MRX_DATA_NACK:								//Data byte has been received and NACK transmitted
#ifdef DEBUG
            printf("DE\t0x%x\n", TWDR);
            printf("RE\t0x%x\n", TWSR);
#else
            asmNop(ASM_NOP_TIMES);
#endif
            twiData.data[twiData.dataIndex] = TWDR;			//Put received data in twiData.data struct
            TWCR = (1 << TWEN) | (0 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC); //Send stop bit
            break;
        case TWI_SRX_ADR_DATA_ACK:							//Ignored
#ifdef DEBUG
            printf("R\t0x%x\n", TWSR);
#else
            asmNop(ASM_NOP_TIMES);
#endif
            break;
        case TWI_MRX_ADR_NACK:
        case TWI_MTX_ADR_NACK:
        case TWI_MTX_DATA_NACK:
            if (twiData.retries < 2) {						//If an byte was not received break communication after 2 retries.
                TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC); //Send stop bit
                twiData.error = 1;
                break;
            }
        case TWI_BUS_ERROR:
        default:
            TWCR = (1 << TWEN) | (0 << TWIE) | (0 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //reset TWI interface
#ifdef DEBUG
            printf("RQ\t0x%x\n", TWSR);
#else
            asmNop(ASM_NOP_TIMES);
#endif
            break;
    }
}