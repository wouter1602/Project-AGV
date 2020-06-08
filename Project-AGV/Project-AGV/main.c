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
	//initPins();
	//initMotor();
	//initMagneto();
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
    while (1) {
		//printf("%d\t%d\n", getMagnetoDataX(), getMagnetoDataY());
		
		//twiTest(0x04, 0x3B);
		
		/*while (errorNt != 20) {
			if (errorNt != 0) {
				printf("Error\t%d, \tTWCR = 0x%x\n", errorNt, (TWCR & 0xF8));
				errorNt = 0;
			}
		}*/
		//twiWatchdog();
		//printf("Error\t21, \tTWCR = 0x%x\n", (TWCR & 0xF8));
		PORTC ^= (1 << PORTC7);
		_delay_ms(500);
    }
}