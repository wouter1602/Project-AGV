/*
 * navigation.h
 *
 * Created: 9-6-2020 18:55:26
 *  Author: Wouter
 */ 


#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include <avr./io.h>
#include <util/delay.h>
#include <math.h>

#include "magneto.h"

#define ARRAY_SIZE 5
#define MOTORSPEED 0x7F
#define CORRECTION 1

void setHeading(void);
void navigate(void);


#endif /* NAVIGATION_H_ */