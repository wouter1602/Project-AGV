/*
 * tof.h
 *
 * Created: 27-5-2020 22:03:41
 *  Author: Wouter
 */ 


#ifndef TOF_H_
#define TOF_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "pins.h"
#include "twi.h"

#define TOF_ADDRS 0xFF //?

void initTof(void);
uint16_t getData(uint8_t sensor);



#endif /* TOF_H_ */