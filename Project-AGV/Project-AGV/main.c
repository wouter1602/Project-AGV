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
	DDRC |= (1 << DDC7);
	sei();
	
	usbDeviceAttach();
	streamInit();
	_delay_ms(2000);
	
	printf("\nlet's start\n");
	initMotor();
	//_delay_ms(100);
	initTwi();
	//_delay_ms(100);
	initMagneto();
	printf("Done setup\n");

	float avg[2] = {0};
	int32_t testData = 0;
	magnetoCallibrate(60);
    while (1) {
		avg[0] = getAvgMagnetoDataX();
		avg[1] = getAvgMagnetoDataY();
		
		testData = 100 * magnetoHeading(avg, 2);
		
        _delay_ms(20);
		printf("Angel: %d\n", testData);
		PORTC ^= (1 << PORTC7);
		_delay_ms(500);
    }
}