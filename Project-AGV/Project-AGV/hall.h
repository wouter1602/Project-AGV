/*
 * hall.h
 *
 * Created: 28-5-2020 09:19:46
 *  Author: Wouter
 */ 


#ifndef HALL_H_
#define HALL_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "pins.h"

void initHall(void);
uint8_t getHallData(uint8_t sensor);

#endif /* HALL_H_ */