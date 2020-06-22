/*
* motor.c
*
* Created: 20-5-2020 21:08:56
*  Author: Wouter
*/
//Motordirver DRV8835

#include "motor.h"

volatile uint8_t motorR = 0;
volatile uint8_t motorL = 0;

ISR(TIMER0_OVF_vect) {
	PORTB &= ~(1 << M1PWM) & ~(1 << M2PWM);
}

ISR(TIMER0_COMPA_vect) {
	if (motorR) {
		PORTB |= (1 << M1PWM);
	}
}

ISR(TIMER0_COMPB_vect) {
	if (motorL) {
		PORTB |= (1 << M2PWM);
	}
}

//Include everything needed to start the motor
void initMotor(void) {
	//Setup Timer 0
	//in Normal mode
	
	OCR0A = 0x7F;
	OCR0B = 0x7F;
	
	TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B) | (1 << TOIE0); //Enalbe OVF, COMPA and COMPB fect
}

//set motor speed R
void setMotorR(int16_t speed) {
	TCCR0B &= ~(1 << CS00);
	PORTB &= ~(1 << M1PWM);
	OCR0A = abs(speed);
	if (speed < 0) {
		PORTE |= (1 << M1DIR);
		TCCR0B |= (1 << CS00);
		motorR = 1;
		} else if (speed > 0) {
		PORTE &= ~(1 << M1DIR);
		TCCR0B |= (1 << CS00);
		motorR = 1;
		} else {
		motorR = 0;
		if (motorR == 0 && motorL == 0)
		{
			TCCR0B &= ~(1 << CS00);
			PORTB &= ~(1 << M1PWM);
		}
	}
	
}

//set motor speed L
void setMotorL(int16_t speed) {
	TCCR0B &= ~(1 << CS00); //Disable signal
	PORTB &= ~(1 << M2PWM); //Turn of signal
	OCR0B = abs(speed);
	if (speed < 0) {
		PORTB |= (1 << M2DIR);
		TCCR0B |= (1 << CS00);
		motorL = 1;
		} else if (speed > 0) {
		PORTB &= ~(1 << M2DIR);
		TCCR0B |= (1 << CS00);
		motorL = 1;
		} else {
		motorL = 0;
		if (motorR == 0 && motorL == 0)
		{
			TCCR0B &= ~(1 << CS00);
			PORTB &= ~(1 << M1PWM);
		}
	}
}

void drive(int16_t speed) {
	setMotorL(speed);
	setMotorR(speed);
}

//turn so many degrees right
void turnR(int16_t degrees) {
	setMotorL(ROTATION_SPEED);
	setMotorR(-ROTATION_SPEED);
	while(!(getMagnetoHeading() > (degrees - HEADING_DEVEATION) && getMagnetoHeading() < (degrees + HEADING_DEVEATION)));
	
	setMotorL(0);
	setMotorR(0);
}

//turn so many degrees left
void turnL(int16_t degrees) {
	setMotorR(ROTATION_SPEED);
	setMotorL(-ROTATION_SPEED);
	while(!(getMagnetoHeading() > (degrees - HEADING_DEVEATION) && getMagnetoHeading() < (degrees + HEADING_DEVEATION)));
	
	setMotorL(0);
	setMotorR(0);
}

void turn(int16_t degrees) {
	int16_t curPos = getMagnetoHeading();
	
	int16_t absDegrees = curPos + degrees;
	
	absDegrees = absDegrees % 361;
	if ((curPos - absDegrees) < 0) {
		//turn left
		turnL(absDegrees);
	} else if ((curPos - absDegrees) == 0) {
		//No need to turn
		return;
	} else {
		//turn right.
		turnR(absDegrees);
	}
}

void stopMotorR(void) {
	setMotorL(0);
}

void stopMotorL(void) {
	setMotorR(0);
}

void stopAllMotors(void) {
	stopMotorL();
	stopMotorR();
}