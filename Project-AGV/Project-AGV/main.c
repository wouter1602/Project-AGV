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
	initMotor();
	//_delay_ms(100);
	initTwi();
	//_delay_ms(100);
	initMagneto();
	printf("Done setup\n");

	float avg[2] = {0};
	int32_t testData = 0;
	magnetoCallibrate(60);
	initPhotodiode();
    while (1) {
		//printf("%d\t%d\n", getMagnetoDataX(), getMagnetoDataY());
		twiWrite(MAGNETO_ADDR, CTRL0, (1 << BOOT));
		PORTC ^= (1 << PORTC7);
		printf("0: 0x%x\t1: 0x%x\t2: 0x%x\t3: 0x%x\t4: 0x%x\t5: 0x%x\n", getPhotodiodeData(0), getPhotodiodeData(1), getPhotodiodeData(2), getPhotodiodeData(3), getPhotodiodeData(4), getPhotodiodeData(5));
		_delay_ms(100);
    }
}