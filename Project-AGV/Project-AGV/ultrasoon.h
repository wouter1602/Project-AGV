/*
 * ultrasoon.h
 *
 * Created: 27-5-2020 22:06:13
 *  Author: Wouter
 */ 


#ifndef ULTRSOON_H_
#define ULTRSOON_H_

#include <avr/io.h>
#include <avr/delay.h>
#include <stdio.h>

#include "pins.h"

void initUltrasoon(void);
uint8_t getUltrasoonData(uint8_t sensor);

#endif /* ULTRSOON_H_ */