/*
 * pins.c
 *
 * Created: 20-5-2020 21:15:56
 *  Author: Wouter
 */ 

#include "pins.h"

//Set data directory and pull-ups
static void initIo(void) {
	DDRB |= (1 << M2DIR) | (1 << M1PWM) | (1 << M2PWM);
	DDRE |= (1 << M1DIR);
	
	DDRD |= (1 << PHOTOLIGHT);
	
	PORTD &= ~(1 << PHOTOLIGHT);
}

//setup all the timers
static void initTimers(void) {
	
}

//setup all the intterupts
static void initInterrupt(void) {
	//Setup Limitswitches
}

//combine all the setups
void initPins(void){
	initIo();
	initTimers();
	initInterrupt();
}