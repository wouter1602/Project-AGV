/*
 * ir.h
 *
 * Created: 27-5-2020 22:09:03
 *  Author: Wouter
 */ 


#ifndef IR_H_
#define IR_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "pins.h"

void initIr(void);
uint16_t getIrData(uint8_t sensor);



#endif /* IR_H_ */