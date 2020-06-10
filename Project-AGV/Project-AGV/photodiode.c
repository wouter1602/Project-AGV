/*
* photodiode.c
*
* Created: 28-5-2020 09:21:53
*  Author: Wouter
*/

#include "photodiode.h"

static uint8_t adcWatchdog = 0;

//ADC pin's
//Light PD2
//Left photodiode = 0
//Right photodiode = 5
//photo 0 = (D4) PD4 ADC8
//photo 1 = (A3) PF4 ADC4
//photo 2 = (D11) PB7; No adc port on Leonardo
//photo 3 = (A0) PF7 ADC7
//photo 4 = (A2) PF5 ADC5
//photo 5 = (D5) PC6; No adc port on Leonardo


static uint16_t blueData[6];
static uint16_t whiteData[6];

void initPhotodiode(void) {
	initWatchdog();
	DIDR0 |= (1 << ADC4D) | (1 << ADC5D) | (1 << ADC7D);	//disable digital input for ADC pins.
	DIDR2 |= (1 << ADC8D);
	ADMUX |= (1 << REFS0);									//Select AREF as external refrence
	
	//Set default calibration value's
	blueData[0] = BLUE_CAL0;
	blueData[1] = BLUE_CAL1;
	blueData[2] = BLUE_CAL2;
	blueData[3] = BLUE_CAL3;
	blueData[4] = BLUE_CAL4;
	blueData[5] = BLUE_CAL5;
	
	whiteData[0] = WHITE_CAL0;
	whiteData[1] = WHITE_CAL1;
	whiteData[2] = WHITE_CAL2;
	whiteData[3] = WHITE_CAL3;
	whiteData[4] = WHITE_CAL4;
	whiteData[5] = WHITE_CAL5;
}

uint16_t getPhotodiodeData(uint8_t sensor, uint8_t lightMode) {
	ADMUX &= ~(1 << MUX0) & ~(1 << MUX1) & ~(1 << MUX2) & ~(1 << MUX3) & ~(1 << MUX4);
	ADCSRB &= ~(1 << MUX5);									//Reset ADC mux to all 0
	ADCSRA |= (1 << ADEN);									//Enable ADC
	
	switch (sensor) {
		case 0:
		ADCSRB |= (1 << MUX5);							//Connect ADC8 in single ended mode
		break;
		case 1:
		ADMUX |= (1 << MUX2);							//Connect ADC4 in single ended mode
		break;
		case 2:
		return 2;										//Photodiode 2 can't be read no adc available
		break;
		case 3:
		ADMUX |= (1 << MUX0) | (1 << MUX1) | (1 << MUX2);//Connect ADC7 in single ended mode
		break;
		case 4:
		ADMUX |= (1 << MUX0) | (1 << MUX2);				//Connect ADC5 in single ended mode
		break;
		case 5:
		return 2;										//Photodiode 5 can't be read no adc available
		break;
		default:
		return 2;										//No sensor withing range given.
		break;
	}
	if (lightMode == 1) {
		PORTD |= (1 << PHOTOLIGHT);
	} else {
		PORTD &= (1 << PHOTOLIGHT);
	}
	
	ADCSRA |= (1 << ADSC);									//start measurment
	adcWatchdog = 0;
	startWatchdog((uint8_t*) &adcWatchdog);
	while((!(ADCSRA & (1 << ADIF))) && (adcWatchdog == 0));						//waiting for measurment
	stopWatchdog();
	if (adcWatchdog == 1) {
		#ifdef DEBUG
		printf("Took too long to measure Photodiode %d\n", sensor);
		#endif
		ADCSRA &= ~(1 << ADEN);								//disable ADC
		return 0;
	}
	ADCSRA &= ~(1 << ADEN);									//Disable ADC
	PORTD &= ~(1 << PHOTOLIGHT);
	return (ADC * 64);										//puts 10 bit value to max 16 register
}

static uint16_t getAvgPhotodiodeDataSingle(uint8_t sensor) {
	uint32_t avg = 0;
	for (uint8_t i = 0; i < ADC_MEASURE_TIMES; i++)
	{
		avg += getPhotodiodeData(sensor, 1);
	}
	avg /= ADC_MEASURE_TIMES;
	return (uint16_t) avg;
}

void getAvgPhotodiodeData(uint16_t *data) {
	for (uint8_t i = 0; i < MAX_SENSORS; i++)
	{
		data[i] = getAvgPhotodiodeDataSingle(i);
	}
}

void calibrateBlue(void) {
	getAvgPhotodiodeData(blueData);
#ifdef DEBUG
	printf("Blue data:\n0: 0x%x\t1: 0x%x\t2: 0x%x\t3: 0x%x\t4: 0x%x\t5: 0x%x\n", blueData[0], blueData[1], blueData[2], blueData[3], blueData[4], blueData[5]);	
#endif
}

void calibrateWhite(void) {
	getAvgPhotodiodeData(whiteData);
#ifdef DEBUG
	printf("White data:\n0: 0x%x\t1: 0x%x\t2: 0x%x\t3: 0x%x\t4: 0x%x\t5: 0x%x\n", whiteData[0], whiteData[1], whiteData[2], whiteData[3], whiteData[4], whiteData[5]);
#endif
}

uint8_t sensorStatus(uint8_t sensor) {
	uint16_t data = getAvgPhotodiodeDataSingle(sensor);
	
	if ((data - blueData[sensor]) < ADC_MEASURE_OFFSET) {
		return 1;
	} else {
		return 0;
	}
}