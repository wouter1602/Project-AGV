/*
 * motor.h
 *
 * Created: 20-5-2020 21:09:33
 *  Author: Wouter
 */ 



#ifndef MOTOR_H_
#define MOTOR_H_

#include <avr/io.h>
#include <avr/delay.h>
#include <util/delay.h>

#include "pins.h"

#define NINTEY_DEGREES 90		//Needed to turn 90 degreees

void initMotor(void);
void setmotorR(uint16_t speed);
void setMotorL(uint16_t speed);
void turnR(uint16_t degrees);
void turnL(uint16_t degrees);
void turn90R(void);
void turn90L(void);

#endif /* MOTOR_H_ */