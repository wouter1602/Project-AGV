/*
 * pins.h
 *
 * Created: 20-5-2020 21:15:47
 *  Author: Wouter
 */ 


#ifndef PINS_H_
#define PINS_H_

#include <avr/io.h>
#include <util/delay.h>

#define M1DIR 6 //PE6
#define M2DIR 4 //PB4
#define M1PWM 5 //PB5
#define M2PWM 6 //PB6

void initPins(void);

#endif /* PINS_H_ */