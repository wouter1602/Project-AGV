/*
 * tof.c
 *
 * Created: 27-5-2020 22:03:32
 *  Author: Wouter
 */ 

#include "tof.h"

static void getDeviceInfo(uint8_t address, uint8_t *data) {
	
}

void changeTofAddress(uint8_t orignalAddress, uint8_t newAddres) {
	newAddres &= 0x7F;
	
	
}

void initToF() {
	
}

uint16_t getTofData(uint8_t sensor) {
	uint16_t data = 0;
    return data;
}