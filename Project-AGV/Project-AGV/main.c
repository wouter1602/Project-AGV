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
#include "ir.h"

int main(void) {
	
	sei();
	usbDeviceAttach();
	streamInit();
	initPins();
	initMotor();
	/*
	 * Test code for the IR sensor Pin D11 (Arduino)
	 */
	DDRC |= (1 << DDC7);
    while (1) {
		PORTC ^= (1 << PORTC7);
		printf("%d\n", getIrData());
		_delay_ms(500);
    }
}