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

	float avg[2] = {0};
	int32_t testData = 0;
	//Calibrate Zumo
	PORTC |= (1 << PORTC7);
	while(buttonPressed());		//wait for button pressed
	_delay_ms(5);
	PORTC &= ~(1 << PORTC7);
	magnetoCallibrate(60);
	
	
	//start Zumo	
	PORTC|= (1 << PORTC7);
	while(buttonPressed());		//wait for button pressed
	_delay_ms(5);
	PORTC &= ~(1 << PORTC7);
	setHeading();
	
	turnR(90);
	
    while (1) {
		turn(180);
		_delay_ms(100);
		turn(-180);
		_delay_ms(100);
		//navigate();
		
		//float x = getAvgMagnetoDataX();
		//float y = getAvgMagnetoDataY();
		//float heading = getMagnetoHeading();
		//printf("{%d/100.0,%d/100.0}, ", (int16_t) (x * 100), (int16_t) (y * 100));
		//printf("Heading: %d\n", (int16_t)heading);
		//printf("x: %d\ty: %d\tz: %d\n", (int16_t) getAvgMagnetoDataX(), (int16_t) getAvgMagnetoDataY(), (int16_t) getAvgMagnetoDataZ());
    }
}