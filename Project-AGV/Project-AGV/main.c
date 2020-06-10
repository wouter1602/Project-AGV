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

int main(void) {
	DDRC |= (1 << DDC7);
	sei();
	
	usbDeviceAttach();
	streamInit();
	_delay_ms(2000);
	
	printf("\nlet's start\n");
	initPins();
	initMotor();
	//initTwi();
	//initMagneto();
	initPhotodiode();
	
	printf("Done setup\n");
	
	PORTC |= (1 << PORTC7);
	while(buttonPressed());
	PORTC &= ~(1 << PORTC7);
	_delay_ms(100);
	calibrateWhite();
	
	PORTC |= (1 << PORTC7);
	while (buttonPressed());
	PORTC &= ~(1 << PORTC7);
	_delay_ms(100);
	calibrateBlue();
	
	PORTC |= (1 << PORTC7);
	while (buttonPressed());
	PORTC &= ~(1 << PORTC7);
	_delay_ms(100);
	
    while (1) {
		PORTC ^= (1 << PORTC7);
		printf("0: %d\t1: %d\t2: %d\t3: %d\t4: %d\t5: %d\n", sensorStatus(0), sensorStatus(1), sensorStatus(2), sensorStatus(3), sensorStatus(4), sensorStatus(5));
		_delay_ms(100);
    }
}