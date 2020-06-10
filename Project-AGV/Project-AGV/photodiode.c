/*
* photodiode.c
*
* Created: 28-5-2020 09:21:53
*  Author: Wouter
*/

#include "photodiode.h"


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
void initPhotodiode(void) {
	DIDR0 |= (1 << ADC4D) | (1 << ADC5D) | (1 << ADC7D);	//disable digital input for ADC pins.
	DIDR2 |= (1 << ADC8D);
	ADMUX |= (1 << REFS0);									//Select AREF as external refrence
}

uint16_t getPhotodiodeData(uint8_t sensor) {	
	ADMUX &= ~(1 << MUX0) & ~(1 << MUX1) & ~(1 << MUX2) & ~(1 << MUX3) & ~(1 << MUX4);
	ADCSRB &= ~(1 << MUX5);									//Reset ADC mux to all 0
	
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
	
	ADCSRA |= (1 << ADSC);								//start measurment
	
	while(!(ADCSRA & (1 << ADIF)));						//waiting for measurment
	
	return ADC;
}