/*
 * navigation.c
 *
 * Created: 9-6-2020 18:55:15
 *  Author: Wouter
 */ 

#include "navigation.h"

/*
static struct NavigationParamiters
{
	float setHeading;
	float curHeading;
}navigation;*/

static void initNavigation(float* memory, uint8_t size) {
	for (uint8_t i = 0; i < size; i++){
		memory[i] = getMagnetoHeading();
	}
}

static float calculateDeflection(float curHeading, float* memory, uint8_t size) {
	float angle = 0;
	for(uint8_t i = 0; i < size; i++)
    {
        angle += memory[i];
    }
	angle /= size;
	return (angle -= curHeading);
}

static void driveStraight(float* memory, uint8_t size){
	float curHeading = getMagnetoHeading();
	initNavigation(memory, size);
	uint8_t driving = 0;
	float deflection = 0;
	while(driving == 0) {
		for (int8_t i = (size-1); i > 0; i--) {			//Moves all the data one to the right in the array
			memory[i] = memory[i-1];
		}
		memory[0] = getMagnetoHeading();
		deflection = calculateDeflection(curHeading, memory, size);
		
		if (deflection < -2) {
			PORTC |= (1 << PORTC7);
			turn(-CORRECTION);
			drive(MOTORSPEED);
		} else if (deflection > 2) {
			PORTC |= (1 << PORTC7);
			turn(CORRECTION);
			drive(MOTORSPEED);
		} else {
			PORTC &= (1 << PORTC7);
			drive(MOTORSPEED);
		}
		if (limitswitchPressed()) {
			driving = 1;
		}
	}
	drive(0);
}

void navigate(void) {
	float memory[ARRAY_SIZE] = {0};
	drive(MOTORSPEED);
	while(limitswitchPressed());
	drive(-MOTORSPEED);
	_delay_ms(300);
	drive(0);
	_delay_ms(1000);
	//driveStraight(memory, ARRAY_SIZE);
	turn(-90);
	drive(MOTORSPEED);
	while(limitswitchPressed());
	drive(-MOTORSPEED);
	_delay_ms(300);
	drive(0);
	_delay_ms(1000);
	//driveStraight(memory, ARRAY_SIZE);
	turn(-90);
	drive(MOTORSPEED);
	while(limitswitchPressed());
	drive(-MOTORSPEED);
	_delay_ms(30);
	drive(0);
	_delay_ms(1000);
	//driveStraight(memory, ARRAY_SIZE);
	turn(-90);
	drive(MOTORSPEED);
	while(limitswitchPressed());
	drive(-MOTORSPEED);
	_delay_ms(300);
	drive(0);
	_delay_ms(1000);
	//driveStraight(memory, ARRAY_SIZE);
	turn(-90);
}