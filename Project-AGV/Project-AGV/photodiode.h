/*
 * photodiode.h
 *
 * Created: 28-5-2020 09:22:02
 *  Author: Wouter
 */ 


#ifndef PHOTODIODE_H_
#define PHOTODIODE_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "pins.h"

void initPhotodiode(void);
uint8_t getPhotodiodeData(uint8_t sensor);

#endif /* PHOTODIODE_H_ */