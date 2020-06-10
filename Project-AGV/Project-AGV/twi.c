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
    uint8_t rs : 1;
} twiData;

static inline uint8_t twiTransceiverBusy(void) {
    return ((TWCR & (1 << TWIE)) && (twiData.watchdogOverflow == 0)); // IF TWI Interrupt is enabled then the Transceiver is busy
}

void initTwi(void) {
    TWBR = 72;
    TWSR = 0;
    TWDR = 0xFF;
    TWCR = (1 << TWEN) | (0 << TWIE) | (0 << TWWC) | (0 << TWSTO) | (0 << TWSTA) | (0 << TWEA) | (0 << TWINT);
    twiData.length = 0;
    twiData.dataIndex = 0;
    twiData.error = 0;
    twiData.rs = 0;
    DDRC |= (1 << DDC7);
    PORTD |= (1 << PORTD0) | (1 << PORTD1);

    //Setup watchdog timer for overflow
    twiData.watchdogOverflow = 0;
    initWatchdog();
}

uint8_t twiWrite(const uint8_t *data, uint8_t size) {
    if (size > MAX_BUFFER_LENGTH) { //Data too big for register
#ifdef DEBUG
        printf("twiWrite data too big\n");
#endif
        return 1;
    }

    twiData.watchdogOverflow = 0;
    startWatchdog((uint8_t*) & twiData.watchdogOverflow);
    while (twiTransceiverBusy()); //Wait for TWI is ready for next transmission
    stopWatchdog();
    if (twiData.watchdogOverflow == 1) {
#ifdef DEBUG
        printf("Took to long to wait for empty TWI lane.\nERROR: 0x%x", (TWCR & 0xF8));
#endif
        return 2;
    }

    twiData.length = size;

    if ((data[0] & 1) == 1) { //register is read command
        return 3;
    }
    twiData.rs = 0;
    for (int i = 0; i < size; i++) { //Copy data in buffer
        twiData.data[i] = data[i];
    }

    TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send start bit
    return 0;
}

uint8_t twiRead(uint8_t *data, uint8_t writeSize, uint8_t readSize) {
    if (max(readSize, writeSize) > MAX_BUFFER_LENGTH) {
#ifdef DEBUG
        printf("twiRead data too big\n");
#endif
        return 1;
    }
    twiData.watchdogOverflow = 0;
    startWatchdog((uint8_t*) & twiData.watchdogOverflow);
    while (twiTransceiverBusy()); //wait for TWI is ready for next transmission
    stopWatchdog();
    if (twiData.watchdogOverflow == 1) {
#ifdef DEBUG
        printf("Took to long to wait for empty TWI lane.\nERROR: 0x%x", (TWCR & 0xF8));
#endif
        return 2;
    }
    twiData.rs = 0;
    if ((data[0] & 1) == 0) { //first byte is a write byte
        twiData.length = writeSize;
        for (int i = 0; i < writeSize; i++) {
            twiData.data[i] = data[i];
        }
        TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send start bit
        twiData.watchdogOverflow = 0;
        startWatchdog((uint8_t*) & twiData.watchdogOverflow);
        while (twiTransceiverBusy()); //wait for TWI is ready for next transmission
        stopWatchdog();
        if (twiData.watchdogOverflow == 1) {
#ifdef DEBUG
            printf("Took to long to write to IC.\nERROR: 0x%x", (TWCR & 0xF8));
#endif
            return 4;
        }
    }

    twiData.data[0] = data[writeSize];
    twiData.length = readSize;

    TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send start bit
    twiData.watchdogOverflow = 0;
    startWatchdog((uint8_t*) & twiData.watchdogOverflow);
    while (twiTransceiverBusy()); //wait for TWI is ready for next transmission
    stopWatchdog();
    if (twiData.watchdogOverflow == 1) {
#ifdef DEBUG
        printf("Took to long to read from IC.\nERROR: 0x%x", (TWCR & 0xF8));
#endif
        return 4;
    }
    for (uint8_t i = 0; i < readSize; i++) {
        data[i] = twiData.data[i];
    }
    return 0;
}

uint8_t twiReadRS(uint8_t *data, uint8_t writeSize, uint8_t readSize) {
    if ((max(writeSize, readSize) + 1) > MAX_BUFFER_LENGTH) { //Data too big for register
#ifdef DEBUG
        printf("twiWrite data too big\n");
#endif
        return 1;
    }

    twiData.watchdogOverflow = 0;
    startWatchdog((uint8_t*) & twiData.watchdogOverflow);
    while (twiTransceiverBusy()); //wait for TWI is ready for next transmission
    stopWatchdog();
    if (twiData.watchdogOverflow == 1) {
#ifdef DEBUG
        printf("Took to long to wait for empty TWI lane.\nERROR: 0x%x", (TWCR & 0xF8));
#endif
        return 2;
    }

    if ((data[0] & 1) == 0) { //first byte is a write byte
        twiData.length = writeSize;
        for (int i = 0; i < writeSize + 1; i++) {
            twiData.data[i] = data[i];
        }
        twiData.data[(writeSize + 1)] = readSize;
        twiData.rs = 1;
        TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send start bit
    }

    twiData.watchdogOverflow = 0;
    startWatchdog((uint8_t*) & twiData.watchdogOverflow);
    while (twiTransceiverBusy()); //wait for TWI is ready for next transmission
    //while ((TWSR != 0xF8) && twiData.watchdogOverflow == 0); //wait for TWI is ready for next transmission
    stopWatchdog();
    if (twiData.watchdogOverflow == 1) {
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
        data[i] = twiData.data[i];
    }
#ifdef DEBUG
    printf("\n");
#endif
    return 0;
}

ISR(TWI_vect) {
    switch (TWSR & 0xF8) {
            //start signal
        case TWI_START:
        case TWI_REP_START:
            twiData.dataIndex = 0; //reset index
        case TWI_MTX_ADR_ACK: //ACK on sending address
        case TWI_MTX_DATA_ACK: //ACK on sending Data
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
                printf("DS\t0x%x\n", twiData.data[twiData.dataIndex - 1]);
#endif
                TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Enable TWI
            } else if (twiData.rs == 1) {
#ifdef DEBUG
                printf("Rr\t0x%x\n", TWSR);
#endif
                twiData.data[0] = twiData.data[twiData.length];
#ifdef DEBUG
                printf("Dr\t0x%x\n", twiData.data[0]);
#endif
                twiData.length = twiData.data[(twiData.length + 1)];
                twiData.rs = 0;
                TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send restartbit
            } else { //Send stop after last byte
#ifdef DEBUG
                printf("RE\t0x%x\n", TWSR);
#else
                asmNop(ASM_NOP_TIMES);
#endif
                TWCR = (1 << TWEN) | (0 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC); //Send stop bit
            }

            break;
        case TWI_MRX_DATA_ACK:
            twiData.data[twiData.dataIndex++] = TWDR;
        case TWI_MRX_ADR_ACK: //Data byte has been received and ACK transmitted

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


            } else { //Last data byte received
#ifdef DEBUG
                printf("Re\t0x%x\n", TWSR);
#endif
                TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //Send NACK bit
            }
            break;
        case TWI_MRX_DATA_NACK: //Data byte has been received and NACK transmitted
#ifdef DEBUG
            printf("DE\t0x%x\n", TWDR);
            printf("RE\t0x%x\n", TWSR);
#else
            asmNop(ASM_NOP_TIMES);
#endif
            twiData.data[twiData.dataIndex] = TWDR;
            TWCR = (1 << TWEN) | (0 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC); //Send stop bit
            break;
        case TWI_SRX_ADR_DATA_ACK: //Ignored
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
                TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) | (0 << TWWC); //Send stop bit
                twiData.error = 1;
                break;
            }
        case TWI_BUS_ERROR:
        default:
            //twiError(5);

            TWCR = (1 << TWEN) | (0 << TWIE) | (0 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC); //reset TWI interface
#ifdef DEBUG
            printf("RQ\t0x%x\n", TWSR);
#else
            asmNop(ASM_NOP_TIMES);
#endif
            break;
    }
}