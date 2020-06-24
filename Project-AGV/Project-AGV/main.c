/*
 * Project-AGV.c
 *
 * Created: 4-5-2020 16:36:46
 * Author : Wouter
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <USBCore.h>
#include <USBART.h>

#include "pins.h"
#include "motor.h"
#include "magneto.h"
#include "twi.h"
#include "hall.h"
#include "photodiode.h"
#include "tof.h"

int main(void) {
	DDRC |= (1 << DDC7);
	sei();
	usbDeviceAttach();		//setup bootloader
	streamInit();			//enable USART over USB
	_delay_ms(2000);		//Wait 2 sec for bootloader to finish

#ifdef DEBUG
	printf("\nlet's start\n");
#endif
	initPins();
	initMotor();
	initTwi();
	initMagneto();
	initToF();
#ifdef DEBUG
	printf("Done setup\n");
#endif
	getTofData(0);

    while (1) {
		printf("Measurment: %d", getTofData(0));
		PORTC ^= (1 << PORTC7);
		_delay_ms(1000);
    }
}