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
	deflection = AvarageAngle - navigation.setHeading;
}

void navigate(void) {
	while (1){
		j = j++;
		memory[j] = getMagnetoHeading;
		if(j > 4){
			j = j - 5;
		}
		if (deflection > 2){
			TurnL(0x7F);
			delay(1);
			drive(0x7F);
		}
		else if(deflection < 358 && deflection > 270){
			TurnR(0x7F);
			delay(1);
			drive(0x7F);
		}
		else{
			drive(0x7F);
		}
	}
}

void navigateSetup(void) {
	int memory[5];
	for (i = 0; i < 5; i++){
		memory[i] = getCurHeading;
	}
}