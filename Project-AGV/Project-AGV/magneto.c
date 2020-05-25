/*
 * magneto.c
 *
 * Created: 25-5-2020 20:31:15
 *  Author: Wouter
 */ 

#include "magneto.h"

void initMagneto(void) {
	initTwi();
	twiWrite(MAGNETO_ADDR_W, CTRL0, (1 << BOOT));
	_delay_ms(5); //wait for reboot
	twiWrite(MAGNETO_ADDR_W, CTRL5, ((1 << TEMP_EN) | (1 << M_RES0) | (1 << M_RES1) | (1 << M_ODR1) | (1 << M_ODR0))); //enable Temp, Magneto sensor (High accuracy) and 25 Hz measurement speed
	twiWrite(MAGNETO_ADDR_W, CTRL6, (1 << MFS0)); //Set magnetic gauss to +/- 4
	twiWrite(MAGNETO_ADDR_W, CTRL7, ((0 << MD0) | (0 << MD1))); //Enable magnetic to continuous-conversion mode
	
}

uint16_t getMagnetoDataX(void) {
	uint16_t data = 0;
	uint8_t dataL = twiRead(MAGNETO_ADDR_R, OUT_X_L_M);
	uint8_t dataH = twiRead(MAGNETO_ADDR_R, OUT_X_H_M);
	data = (dataH << 8) | dataL;
	return data;
}

uint16_t getMagnetoDataY(void) {
	uint16_t data = 0;
	uint8_t dataL = twiRead(MAGNETO_ADDR_R, OUT_Y_L_M);
	uint8_t dataH = twiRead(MAGNETO_ADDR_R, OUT_Y_H_M);
	data = (dataH << 8) | dataL;
	return data;
}