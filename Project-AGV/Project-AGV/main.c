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

#include "pins.h"
#include "motor.h"

int main(void) {
	sei();
	
	usbDeviceAttach();
	initPins();
	initMotor();
	
	DDRC |= (1 << DDC7);
    /* Replace with your application code */
    while (1) {
		/*
		setMotorR(0x7F);
		PORTC |= (1 << PORTC7);
		_delay_ms(500);
		setMotorR(0);
		_delay_ms(500);
		setMotorR(-0x7F);
		PORTC &= ~(1 << PORTC7);
		_delay_ms(500);
		setMotorR(0);
		_delay_ms(500);
		*/

		drive(0x7F);
		PORTC |= (1 << PORTC7);
		_delay_ms(500);
		drive(0);
		_delay_ms(500);
		drive(-0x7F);
		PORTC &= ~(1 << PORTC7);
		_delay_ms(500);
		drive(0);
		_delay_ms(500);

		/*
		setMotorL(0x7F);
		PORTC |= (1 << PORTC7);
		_delay_ms(500);
		setMotorL(0);
		_delay_ms(500);
		setMotorL(-0x7F);
		PORTC &= ~(1 << PORTC7);
		_delay_ms(500);
		setMotorL(0);
		_delay_ms(500);
		*/
    }
}

