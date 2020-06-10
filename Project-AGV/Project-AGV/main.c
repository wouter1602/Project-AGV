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
	sei();
	
	usbDeviceAttach();
	streamInit();
	initPins();
	//initMotor();
	//initMagneto();
	
	initPhotodiode();
	
	DDRC |= (1 << DDC7);
    while (1) {
		PORTC ^= (1 << PORTC7);
		printf("0: 0x%x\t1: 0x%x\t2: 0x%x\t3: 0x%x\t4: 0x%x\t5: 0x%x\n", getPhotodiodeData(0), getPhotodiodeData(1), getPhotodiodeData(2), getPhotodiodeData(3), getPhotodiodeData(4), getPhotodiodeData(5));
		_delay_ms(100);
    }
}