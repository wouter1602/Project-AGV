/*
* watchdog.c
*
* Created: 8-6-2020 14:21:15
*  Author: Wouter
*/

#include "watchdog.h"

static volatile uint8_t *addrs_ = NULL;

void initWatchdog(void) {
    WDTCSR |= (1 << WDCE); //Make sure the watchdog timer can be changed
	WDTCSR &= ~(1 << WDP0) & ~(1 << WDP1) & ~(1 << WDP2); //set prescaler to 16ms
	WDTCSR &= ~(1 << WDE) & ~(1 << WDIE); //Make sure the watchdog doesn't start
	MCUSR &= ~(1 << BORF) & ~(1 << JTRF) & ~(1 << WDRF) & ~(1 << EXTRF) & ~(1 << PORF);
}

uint8_t startWatchdog(uint8_t *addrs) {
    if(WDTCSR & (1 << WDIE)){   //already started
		#ifdef DEBUG
		printf("Watchdog already started.\n");
		#endif
		return 1;
    } else {
        #ifdef DEBUG
		printf("Watchdog timer started:\t0x%x\n", MCUSR);
		#endif
		addrs_ = addrs;
		MCUSR &= ~(1 << PORF) & ~(1 << JTRF) & ~(1 << WDRF) & ~(1 << EXTRF) & ~(1 << PORF); //reset interrupt status register
		WDTCSR |= (1 << WDIE);
        return 0;
    }
}

uint8_t stopWatchdog(void){
    if(!(WDTCSR & (1 << WDIE))){   //already stopped
        #ifdef DEBUG
		printf("Watchdog already stopped.\n");
		#endif
        return 1;
    } else {
        #ifdef DEBUG
        printf("Watchdog timer stopped:\t0x%x\n", MCUSR);
        #endif
		WDTCSR = 0x00;
		MCUSR &= ~(1 << PORF) & ~(1 << JTRF) & ~(1 << WDRF) & ~(1 << EXTRF) & ~(1 << PORF); //reset interrupt status register
		
        return 0;
    }
}

ISR(WDT_vect) {
	#ifdef DEBUG
	printf("***WATCHDOG TRIGGERT***\n");
	#endif
	if (addrs_ == NULL)
	{
		//Pointer not assigned
		#ifdef DEBUG
		printf("***WATCHDOG POINTER NOT ASSIGNED***\n");
		#endif
	} else {
		*addrs_ ^= 1; //changes the first bit of the data pointed to addrs_
		addrs_ = NULL;
	}
}