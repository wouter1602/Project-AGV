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
	
	printf("Done setup\n");
	
	uint16_t firstMeasurment[6] = {0};
	uint16_t secondMeasurment[6] = {0};
	int16_t differance[6] = {0};
	
	getAvgPhotodiodeData(firstMeasurment);
	printf("0: 0x%x\t1: 0x%x\t2: 0x%x\t3: 0x%x\t4: 0x%x\t5: 0x%x\n", firstMeasurment[0], firstMeasurment[1], firstMeasurment[2], firstMeasurment[3], firstMeasurment[4], firstMeasurment[5]);
	while(buttonPressed());
	_delay_ms(5);
	getAvgPhotodiodeData(secondMeasurment);
	printf("0: 0x%x\t1: 0x%x\t2: 0x%x\t3: 0x%x\t4: 0x%x\t5: 0x%x\n", secondMeasurment[0],  secondMeasurment[1], secondMeasurment[2], secondMeasurment[3], secondMeasurment[4], secondMeasurment[5]);	
	
	for (uint8_t i = 0; i < 5; i++) {
		differance[i] = firstMeasurment[i] - secondMeasurment[i];
	}
	printf("0: %d\t1: %d\t2: %d\t3: %d\t4: %d\t5: %d\n", differance[0], differance[1], differance[2], differance[3], differance[4], differance[5]);
	//printf("0: %d\t1: %d\t2: %d\t3: %d\t4: %d\t5: %d\n", (int16_t) ((int16_t) firstMeasurment[0] - (int16_t)secondMeasurment[0]), (int16_t) ((int16_t) firstMeasurment[1] - (int16_t)secondMeasurment[1]), (int16_t) ((int16_t) firstMeasurment[2] - (int16_t)secondMeasurment[2]), (int16_t) ((int16_t) firstMeasurment[3] - (int16_t)secondMeasurment[3]), (int16_t) ((int16_t) firstMeasurment[4] - (int16_t)secondMeasurment[4]));
	

	initPhotodiode();
    while (1) {
		PORTC ^= (1 << PORTC7);
		//printf("0: 0x%x\t1: 0x%x\t2: 0x%x\t3: 0x%x\t4: 0x%x\t5: 0x%x\n", getPhotodiodeData(0), getPhotodiodeData(1), getPhotodiodeData(2), getPhotodiodeData(3), getPhotodiodeData(4), getPhotodiodeData(5));
		_delay_ms(100);
    }
}