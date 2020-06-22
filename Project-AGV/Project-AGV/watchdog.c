/*
* watchdog.c
*
* Created: 8-6-2020 14:21:15
*  Author: Wouter
*/

#include "watchdog.h"

static volatile uint8_t *addrs_ = NULL;

/*
 * Set watchdog with an overflow of 16ms.
 * When the watchdog overflows the ISR(WDT_vect) wil be triggert
 */
void initWatchdog(void) {
    WDTCSR |= (1 << WDCE); //Make sure the watchdog timer can be changed
	WDTCSR &= ~(1 << WDP0) & ~(1 << WDP1) & ~(1 << WDP2); //set prescaler to 16ms
	WDTCSR &= ~(1 << WDE) & ~(1 << WDIE); //Make sure the watchdog doesn't start
	MCUSR &= ~(1 << BORF) & ~(1 << JTRF) & ~(1 << WDRF) & ~(1 << EXTRF) & ~(1 << PORF);
}

/*
 * Start watchdog and sets pinter int as value for the watchdog to trigger.
 * If the watchdog is already started will return 1.
 * If the watchdog is started will return 0.
 */
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
		addrs_ = addrs;			//Set address of int in a local int
		MCUSR &= ~(1 << PORF) & ~(1 << JTRF) & ~(1 << WDRF) & ~(1 << EXTRF) & ~(1 << PORF); //reset interrupt status register
		WDTCSR |= (1 << WDIE);		//Start watchdog
        return 0;
    }
}

/*
 * Stops the watchdog from triggering.
 * This has to be done after the startWatchdog() fucntion.
 * It should be after the function that could hang because of communication error.
 * If the Watchdog is already stopped will return 1.
 * If the watchdog is stopped will return 0.
 */
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
		WDTCSR = 0x00;					//Stop watchdog
		MCUSR &= ~(1 << PORF) & ~(1 << JTRF) & ~(1 << WDRF) & ~(1 << EXTRF) & ~(1 << PORF); //reset interrupt status register
		
        return 0;
    }
}

/*
 * This function is triggert when the watchdog overflows.
 * When it overflows it will change the value that addrs_ in pointing to from a 0 to 1 or from 1 to 0.
 * This could give feedback to a while loop that is waiting for data to be returned as alternative if that communication takes to long.
 * After changing the value that addrs_ is pointing to addrs_ is change to NULL.
 * This is done so it doesn't accidentally change the value again when it shouldn't have.
 */
ISR(WDT_vect) {
#ifdef DEBUG
	printf("***WATCHDOG TRIGGERT***\n");
#endif
	if (addrs_ == NULL)				//If addrs_ doesn't point to a valid address where it can change a value
	{
		//Pointer not assigned
#ifdef DEBUG
		printf("***WATCHDOG POINTER NOT ASSIGNED***\n");
#endif
	} else {
		*addrs_ ^= 1; //changes the first bit of the data pointed to addrs_
#ifdef DEBUG
		printf("***WATCHDOG VARIABLE CHANGED TO: 0x%x***\n", *addrs_);
#endif
		addrs_ = NULL;			//Reset addrs_ pointer to zero so it doesn't accidentally retrigger the same value
	}
}