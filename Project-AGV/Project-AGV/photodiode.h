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

#define ADC_MEASURE_TIMES 10
#define MAX_SENSORS 6
#define ADC_MEASURE_OFFSET 10

//Default calibration data
#define WHITE_CAL0	0x3Ff
#define WHITE_CAL1	0x3FF
#define WHITE_CAL2	0x002
#define WHITE_CAL3	0x3FF
#define WHITE_CAL4	0x3FF
#define	WHITE_CAL5	0x002

#define BLUE_CAL0	0x35
#define BLUE_CAL1	0x37
#define BLUE_CAL2	0x02
#define BLUE_CAL3	0x37
#define BLUE_CAL4	0x31
#define BLUE_CAL5	0x02 

void initPhotodiode(void);
uint16_t getPhotodiodeData(uint8_t sensor, uint8_t lightMode);
void getAvgPhotodiodeData(uint16_t *data);
void calibrateBlue(void);
void calibrateWhite(void);
uint8_t sensorStatus(uint8_t sensor);

#endif /* PHOTODIODE_H_ */