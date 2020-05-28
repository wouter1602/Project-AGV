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
#include "ultrasoon.h"

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
		printf("Test\n");

		turnL(0x7F);
		_delay_ms(500);
		turnL(-0x7F);
		_delay_ms(500);

		turnR(0x7F);
		_delay_ms(500);
		turnR(-0x7F);
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