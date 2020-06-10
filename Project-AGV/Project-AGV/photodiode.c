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

static struct SensorData {
	uint16_t 0;
	uint16_t 1;
	uint16_t 2;
	uint16_t 3;
	uint16_t 4;
	uint16_t 5;
	};

void initPhotodiode(void) {
	initWatchdog();
	DIDR0 |= (1 << ADC4D) | (1 << ADC5D) | (1 << ADC7D);	//disable digital input for ADC pins.
	DIDR2 |= (1 << ADC8D);
	ADMUX |= (1 << REFS0);									//Select AREF as external refrence
}

uint16_t getPhotodiodeData(uint8_t sensor) {
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
	return ADC;
}

void getAvgPhotodiodeData(uint16_t *data) {
	uint32_t avg;
	for (uint8_t i = 0; i < MAX_SENSORS; i++)
	{
		avg = 0;
		for (uint8_t j = 0; j < ADC_MEASURETIMES; j++) {
			avg += getPhotodiodeData(i);
		}
		avg /= ADC_MEASURETIMES;
		data[i] = avg;
	}
}