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
#include "tof.h"

int main(void) {
	DDRC |= (1 << DDC7);
	sei();
	
	usbDeviceAttach();
	streamInit();
	_delay_ms(2000);
	//initPins();
	//initMotor();
	//initMagneto();
    uint8_t testData[3] = {0};
	printf("\nlet's start\n");
	_delay_ms(100);
	initTwi();
	_delay_ms(100);
	initMagneto();
	//uint8_t TWI[] = {0x3A, 0x1F, 0x80};
	//uint8_t TWI[] = {0x04, 0x7B};
	//twiWrite(0xAA, 0x7B, 0xFF);
	//twiWrite(TWI, 3);
	//printf("Start Twi\n");
	//twiWrite(TWI, 2);
	printf("Done setup\n");
    uint16_t data = getMagnetoDataX(testData);
    _delay_ms(20);
    printf("Magneto1 0x%x\n", data);
    printf("Data received1:\n0x%x\t0x%x\t0x%x\n", testData[0], testData[1], testData[2]);
    data = getMagnetoDataX(testData);
    _delay_ms(20);
    printf("Magneto2 0x%x\n", data);
    printf("Data received2:\n0x%x\t0x%x\t0x%x\n", testData[0], testData[1], testData[2]);
    
    while (1) {
		data = getMagnetoDataX(testData);
        _delay_ms(20);
        printf("Magneto\t0x%x\n",data);
		PORTC ^= (1 << PORTC7);
		_delay_ms(500);
    }
}