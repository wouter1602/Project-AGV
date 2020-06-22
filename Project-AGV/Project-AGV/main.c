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
	//initPhotodiode();
#ifdef DEBUG
	printf("Done setup\n");
#endif
	//Calibrate Zumo
	PORTC |= (1 << PORTC7);
	while(buttonPressed());		//wait for button pressed
	_delay_ms(5);
	PORTC &= ~(1 << PORTC7);
	magnetoCallibrate(60);		//Calibrate Zumo Magneto sensor
	
	
	//start Zumo	
	PORTC|= (1 << PORTC7);
	while(buttonPressed());		//wait for button pressed
	_delay_ms(5);
	PORTC &= ~(1 << PORTC7);
	
    while (1) {
		printf("Current heading:\t%d\n", (int32_t) round(getMagnetoHeading()));		//Prints out current heading for debugging
		_delay_ms(1000);
    }
}