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

#include "magneto.h"

#ifndef HEADING_DEVEATION
#define HEADING_DEVEATION 5
#endif

void initNavigation(void);
void setHeading(void);
void navigate(void);


#endif /* NAVIGATION_H_ */