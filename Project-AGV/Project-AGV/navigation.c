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

void navigate(void) {
	
}