/*
 * hall.c
 *
 * Created: 28-5-2020 09:18:49
 *  Author: Wouter
 */

#include "hall.h"
void setupADC(void) {
/*
* setup ADC
* PF0 A5 (ADC0)
* MUX 0b000000 ADC0
*/
    DIDR0 |= (1 << ADC0D); //Disable digital inputs on ADC ports
    ADMUX |= (1 << REFS0); //Select AREF as external reference
}

void initHall(void){
    setupADC();
}
//Read ADC for Current sensing
static uint16_t readADCHall(void) {
    ADMUX &= ~(1 << MUX0) & ~(1 << MUX1) & ~(1 << MUX2) & ~(1 << MUX3) & ~(1 << MUX4); //Reset MUX to 0
    ADCSRB &= ~(1 << MUX5); //Reset MUX to 0
    ADCSRA |= (1 << ADSC); //start measurement
    while(!(ADCSRA & (1 << ADIF))); //wait for measurement
    ADCSRA |= (1 << ADIF); //reset measurement flag
    return ADC;
}

//This function checks if the AGV detects a magnet (the tree)
//The code for not detecting the same magnet (tree) needs to be implemented in the main code (the routine the AGV executes when it has detected a magnet (tree)
uint8_t getHallData(void){
    if(HALL_THRESHOLD < readADCHall()){
        return 1;
    }
    return 0;
}
