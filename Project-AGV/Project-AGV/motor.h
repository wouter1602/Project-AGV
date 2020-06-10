/*
 * motor.h
 *
 * Created: 20-5-2020 21:09:33
 *  Author: Wouter
 */ 



#ifndef MOTOR_H_
#define MOTOR_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "pins.h"
#include "magneto.h"

#define NINTEY_DEGREES 90		//Needed to turn 90 degreees
#define ROTATION_SPEED 350
#define HEADING_DEVEATION 5

void initMotor(void);
void setMotorR(int16_t speed);
void setMotorL(int16_t speed);
void drive(int16_t speed);
void turnR(int16_t degrees);
void turnL(int16_t degrees);
void turn(int16_t degrees);
void turn90R(void);
void turn90L(void);
void stopMotorL(void);
void stopMotorR(void);
void stopAllMotors(void);

#endif /* MOTOR_H_ */