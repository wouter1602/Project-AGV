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

void initNavigation(void) {
	
}

static void getCurHeading(void) {
	navigation.curHeading = getMagnetoHeading();
}

void setHeading(void) {
	navigation.setHeading = getMagnetoHeading();
}

void calculateDeflection(void) {
	int sum = 0;
	for(i = 0; i < 5; i++)
    {
        sum = sum + memory[i];
    }
	AvarageAngle = sum/5;
	deflection = AvarageAngle - getCurHeading;
}

void navigate(void) {
	driveStraight();
	Turn(-90);
	driveStraight();
	turn(-90)
	driveStraight();
	turn(-90)
	driveStraight();
	turn(-90)
}

void driveStraight(void){
	while (1){
		j = j++;
		memory[j] = getCurHeading;
		if(j > 4){
			j = j - 5;
		}
		if (deflection > 2 && deflection < 270){
			Turn(-1);
			drive(0x7F);
		}
		else if(deflection < 358 && deflection > 270){
			Turn(1);
			drive(0x7F);
		}
		else{
			drive(0x7F);
		}
	}
	drive(0)
}

void navigateSetup(void) {
	int memory[5];
	for (i = 0; i < 5; i++){
		memory[i] = getCurHeading;
	}
}