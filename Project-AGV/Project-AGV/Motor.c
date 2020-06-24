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

/*
 * Enables the Motor diriver pwm timer.
 */
void initMotor(void) {
	//Setup Timer 0
	//in Normal mode
	
	OCR0A = 0x7F;
	OCR0B = 0x7F;
	
	TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B) | (1 << TOIE0); //Enalbe OVF, COMPA and COMPB fect
}

/*
 * Enables right motor.
 * Positive number is forward, negative is backwards.
 * value is the overflow of the pwm signal.
 */
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

/*
 * Enables left motor.
 * Positive number is forward, negative is backwards.
 * value is the overflow of the pwm signal.
 */
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

/*
 * Set's both motor's at the same speed.
 */
void drive(int16_t speed) {
	setMotorL(speed);
	setMotorR(speed);
}

/*
 * turn so many degrees right
 */
void turnR(float degrees) {
	setMotorL(ROTATION_SPEED);
	setMotorR(-ROTATION_SPEED);
	float curPoss = 0.0;
	while(1) {
		curPoss = getMagnetoHeading();
		if (!(curPoss > (degrees - HEADING_DEVEATION) && curPoss < (degrees + HEADING_DEVEATION))) {
			break;
		}
	}
}

/*
 * turn so many degrees left
 */
void turnL(float degrees) {
	setMotorL(-ROTATION_SPEED);
	setMotorR(ROTATION_SPEED);
	float curPoss = 0.0;
	while(1) {
		curPoss = getMagnetoHeading();
		if (!(curPoss > (degrees - HEADING_DEVEATION) && curPoss < (degrees + HEADING_DEVEATION))) {
			break;
		}
	}
}

/*
 * Turn an amound of degrees left or right.
 * Positive numbers is right negative numbers is left.
 * calculates the absolute location it should rotate to.
 */
void turn(float degrees) {
	float curPos = getMagnetoHeading();
	
	float absDegrees = curPos + degrees;
	
	absDegrees = fmodf(absDegrees, 360.0);
	if ((curPos - absDegrees) < 0.0) {
		//turn left
		turnL(absDegrees);
	} else if ((curPos - absDegrees) > 0.0) {
		//turn right.
		turnR(absDegrees);
	} else {
		//No need to turn
		return;
	}
}

/*
 * Stop right motor
 */
void stopMotorR(void) {
	setMotorL(0);
}

/*
 * Stop left motor
 */
void stopMotorL(void) {
	setMotorR(0);
}

/*
 * Stop both motors
 */
void stopAllMotors(void) {
	stopMotorL();
	stopMotorR();
}