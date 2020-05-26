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

int main(void) {
	sei();
	
	usbDeviceAttach();
	streamInit();
	initPins();
	initMotor();
	initMagneto();
	
	DDRC |= (1 << DDC7);
	//twiWrite(0xAA, 0x7B, 0xFF);
    while (1) {
		//printf("%d\t%d\n", getMagnetoDataX(), getMagnetoDataY());
		twiWrite(MAGNETO_ADDR, CTRL0, (1 << BOOT));
		PORTC ^= (1 << PORTC7);
		_delay_ms(10);
    }
}