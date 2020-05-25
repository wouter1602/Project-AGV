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

int main(void) {
	sei();
	
	usbDeviceAttach();
	initPins();
	initMotor();
	streamInit();
	
	DDRC |= (1 << DDC7);
    /* Replace with your application code */
    while (1) {
		
    }
}

