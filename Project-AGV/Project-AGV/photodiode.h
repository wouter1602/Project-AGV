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
#include "watchdog.h"

#define ADC_MEASURETIMES 10
#define MAX_SENSORS 6

void initPhotodiode(void);
uint16_t getPhotodiodeData(uint8_t sensor);
void getAvgPhotodiodeData(uint16_t *data);

#endif /* PHOTODIODE_H_ */