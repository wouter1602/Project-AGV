/*
* magneto.c
*
* Created: 25-5-2020 20:31:15
*  Author: Wouter
*/

#include "magneto.h"

void initMagneto(void) {
	const uint8_t data1[3] = {MAGNETO_ADDR, CTRL0, (1 << BOOT)};
	const uint8_t data2[3] = {MAGNETO_ADDR, CTRL5, ((1 << TEMP_EN) | (1 << M_RES0) | (1 << M_RES1) | (1 << M_ODR1) | (1 << M_ODR0))};
	const uint8_t data3[3] = {MAGNETO_ADDR, CTRL6, (1 << MFS0)};
	const uint8_t data4[3] = {MAGNETO_ADDR, CTRL7, ((0 << MD0) | (0 << MD1))};
	#ifdef DEBUG
	printf("Data in Array:\n");
	for (int j = 0; j < 3; j++) {
		printf("0x%x\t", data1[j]);
	}
	printf("\n");
	for (int j = 0; j < 3; j++) {
		printf("0x%x\t", data2[j]);
	}
	printf("\n");
	for (int j = 0; j < 3; j++) {
		printf("0x%x\t", data3[j]);
	}
	printf("\n");
	for (int j = 0; j < 3; j++) {
		printf("0x%x\t", data4[j]);
	}
	printf("\n");
	#endif
	
	twiWrite(data1, 3);		//reboot magento sensro
	_delay_ms(5); //wait for reboot
	twiWrite(data2, 3); //Enable Temp, magneto sensor (High accuracy) and 25 Hz measurment
	twiWrite(data3, 3); //Set magnetic Gaus to +/- 4
	_delay_us(5);
	//printf("Tmp: %x\n", TWSR);
	twiWrite(data4, 3); //Enable magnetic to continous-conversion mode	
	twiWrite(data1, 3);
}

uint16_t getMagnetoDataX(uint8_t *data_) {
	uint16_t data = 0;
	//uint8_t dataL = 0;//twiRead(MAGNETO_ADDR, OUT_X_L_M);
	//uint8_t dataH = 0;//twiRead(MAGNETO_ADDR, OUT_X_H_M);
	//data = (dataH << 8) | dataL;
	//return data;
    uint8_t twiData[4] = {MAGNETO_ADDR, OUT_X_L_M, (MAGNETO_ADDR | 1)};
#ifdef DEBUG
    printf("Data in array 2:\n0x%x\t0x%x\t0x%x\n", twiData[0], twiData[1], twiData[2]);
#endif
    
    twiReadRS(twiData, 2, 2);
    for (int i = 0; i < 3; i++) {
        data_[i] = twiData[i];
    }
    return data = twiData[1];
}

uint16_t getMagnetoDataY(void) {
	uint16_t data = 0;
	uint8_t dataL = 0;//twiRead(MAGNETO_ADDR, OUT_Y_L_M);
	uint8_t dataH = 0;//twiRead(MAGNETO_ADDR, OUT_Y_H_M);
	data = (dataH << 8) | dataL;
	return data;
}