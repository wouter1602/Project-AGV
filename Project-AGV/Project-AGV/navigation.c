/*
 * navigation.c
 *
 * Created: 9-6-2020 18:55:15
 *  Author: Wouter
 */ 

#include "navigation.h"

static struct NavigationParamiters
{
	float setHeading;
	float curHeading;
}navigation;

static inline uint8_t getCurHeading(void) {
	return getMagnetoHeading();
}

static void initNavigation(uint8_t* memory, uint8_t size) {
	for (uint8_t i = 0; i < size; i++){
		memory[i] = getCurHeading;
	}
}
void setHeading(void) {
	navigation.setHeading = getMagnetoHeading();
}

static uint8_t calculateDeflection(uint8_t* memory, uint8_t size) {
	int32_t angle = 0;
	for(uint8_t i = 0; i < size; i++)
    {
        angle += memory[i];
    }
	angle /= size;
	return (angle -= getCurHeading());
}

static void driveStraight(uint8_t* memory, uint8_t size){
	/*while (1){
		j = j++;
		memory[j] = getCurHeading;
		if(j > 4){
			j = j - 5;
		}
		if (deflection > 2 && deflection < 270){
			turn(-1);
			drive(0x7F);
		}
		else if(deflection < 358 && deflection > 270){
			turn(1);
			drive(0x7F);
		}
		else{
			drive(0x7F);
		}
	}*/
	uint8_t driving = 0;
	uint8_t deflection = 0;
	while(driving == 0) {
		for (int8_t i = (size-1); i > 0; i--) {			//Moves all the data one to the right in the array
			memory[i] = memory[i-1];
		}
		memory[0] = getCurHeading();
		deflection = calculateDeflection(memory, size);
		
		if (deflection > 2 && deflection < 270) {
			turn(-CORRECTION);
			drive(MOTORSPEED);
		} else if (deflection < 358 && deflection > 270) {
			turn(CORRECTION);
			drive(MOTORSPEED);
		} else {
			drive(MOTORSPEED);
		}
		if (buttonPressed()) {
			driving = 1;
		}
	}
	drive(0);
}

void navigate(void) {
	uint8_t memory[ARRAY_SIZE] = {0};
	initNavigation(memory, ARRAY_SIZE);
	
	driveStraight(memory, ARRAY_SIZE);
	turn(-90);
	driveStraight(memory, ARRAY_SIZE);
	turn(-90);
	driveStraight(memory, ARRAY_SIZE);
	turn(-90);
	driveStraight(memory, ARRAY_SIZE);
	turn(-90);
}