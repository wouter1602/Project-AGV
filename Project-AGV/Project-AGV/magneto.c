/*
* magneto.c
*
* Created: 25-5-2020 20:31:15
*  Author: Wouter
*/

#include "magneto.h"

static struct Vector {
	int16_t maxX;
	int16_t maxY;
	int16_t maxZ;
	int16_t minX;
	int16_t minY;
	int16_t minZ;
}magnetoVectors;

static int16_t getMagnetoDataX(void) {
	int16_t data = 0;
	uint8_t twiData[3] ={MAGNETO_ADDR, (OUT_X_L_M | (1 << 7)), (MAGNETO_ADDR | 1)};

	
	twiReadRS(twiData, 2, 3);
	data = (twiData[1] << 8) | twiData[2];
	return data;
}

static int16_t getMagnetoDataY(void) {
	int16_t data = 0;
	uint8_t twiData[3] = {MAGNETO_ADDR, (OUT_Y_L_M | (1 << 7)), (MAGNETO_ADDR | 1)};
	
	twiReadRS(twiData, 2, 3);
	data = (twiData[2] << 8) | twiData[1];
	return data;
}

static int16_t getMagnetoTempData(void) {
	int16_t data = 0;
	uint8_t twiData[3] = {MAGNETO_ADDR, (TEMP_OUT_L | (1 << 7)), (MAGNETO_ADDR | 1)};
	
	twiReadRS(twiData, 2 , 3);
	data = (twiData[2] << 8) | twiData[1];
	
	return data;
}

static int16_t getAcceleroDataX(void) {
	int16_t data = 0;
	uint8_t twiData[3] = {MAGNETO_ADDR, (OUT_X_L_A | (1 << 7)), (MAGNETO_ADDR | 1)};
	
	twiReadRS(twiData, 2 , 3);
	data = (twiData[2] << 8) | twiData[1];
	
	return data;
}

static int16_t getAcceleroDataY(void) {
	int16_t data = 0;
	uint8_t twiData[3] = {MAGNETO_ADDR, (OUT_Y_L_A | (1 << 7)), (MAGNETO_ADDR | 1)};
	
	twiReadRS(twiData, 2 , 3);
	data = (twiData[2] << 8) | twiData[1];
	
	return data;
}

static int16_t getAcceleroDataZ(void) {
	int16_t data = 0;
	uint8_t twiData[3] = {MAGNETO_ADDR, (OUT_Z_L_A | (1 << 7)), (MAGNETO_ADDR | 1)};
	
	twiReadRS(twiData, 2 , 3);
	data = (twiData[2] << 8) | twiData[1];
	
	return data;
}

void initMagneto(void) {
	/*
	const uint8_t data1[3] = {MAGNETO_ADDR, CTRL0, (1 << BOOT)};
	const uint8_t data2[3] = {MAGNETO_ADDR, CTRL2, 0};
	const uint8_t data3[3] = {MAGNETO_ADDR, CTRL1, ((1 << AODR0) | (1 << AODR2) | (1 << AZEN) | (1 << AXEN) | (1 << AYEN))}; //Enable magneto sensor on 50hz, 
	const uint8_t data4[3] = {MAGNETO_ADDR, CTRL5, ((1 << TEMP_EN) | (1 << M_RES0) | (1 << M_RES1) | (1 << M_ODR1) | (1 << M_ODR0) & ~(1 << M_ODR2))}; //Enable Tmp, Hig resolution magneto, 25 Hz measurments.
	const uint8_t data5[3] = {MAGNETO_ADDR, CTRL6, (1 << MFS0)}; //+/- 4 gaus megneto measurment
	const uint8_t data6[3] = {MAGNETO_ADDR, CTRL7, ((0 << MD0) | (0 << MD1))};*/
		
	//const uint8_t data1[3] = {MAGNETO_ADDR, CTRL0, 0x80};
	const uint8_t data2[3] = {MAGNETO_ADDR, CTRL2, 0};
	const uint8_t data3[3] = {MAGNETO_ADDR, CTRL1, 0x57};
	const uint8_t data4[3] = {MAGNETO_ADDR, CTRL5, 0xEC};
	const uint8_t data5[3] = {MAGNETO_ADDR, CTRL6, 0x20};
	const uint8_t data6[3] = {MAGNETO_ADDR, CTRL7, 0};
	
	
	#ifdef DEBUG
//Printout data to be transferd to array
	#endif
	
	//twiWrite(data1, 3);		//reboot magento sensor
	_delay_ms(5); //wait for reboot
	twiWrite(data2, 3); //Enable acceleromator.
	twiWrite(data3, 3); //Enable Temp, magneto sensor (High accuracy) and 25 Hz measurment
	//_delay_us(5);
	twiWrite(data4, 3); //Set magnetic Gaus to +/- 4
	twiWrite(data5, 3); //Enalbe magnetic to continous-conversion mode
	twiWrite(data6, 3);
	
	magnetoVectors.maxX = MAX_X;
	magnetoVectors.maxY = MAX_Y;
	magnetoVectors.minX = Min_X;
	magnetoVectors.minY = MIN_Y;
}

float getAvgMagnetoDataX(void) {
	float avgX = 0.0;
	for (int i = 0; i < MAGNETO_AVG; i++) {
		avgX += getMagnetoDataX();
	}
	
	avgX /= MAGNETO_AVG;
	
	return avgX;
}

float getAvgMagnetoDataY(void) {
	float avgY = 0.0;
	for (int i = 0; i < MAGNETO_AVG; i++) {
		avgY += getMagnetoDataX();
	}
	
	avgY /= MAGNETO_AVG;
	
	return avgY;
}

float magnetoHeading(float *data, uint16_t size) {
	float x_scaled = 2.0*(float) (data[0] - magnetoVectors.minX) / ( magnetoVectors.maxX - magnetoVectors.minX) - 1.0;
	float y_scaled = 2.0*(float) (data[1] - magnetoVectors.minY) / ( magnetoVectors.maxY - magnetoVectors.minY) - 1.0;
	
	float angle = atan2(y_scaled, x_scaled) * 180 / M_PI;
	if (angle < 0) {
		angle += 360;
	}
	return angle;
}

void magnetoCallibrate(uint8_t sampleSize) {
	int16_t x, y;
	int16_t minX, minY, maxX, maxY;
	
	setMotorL(CAL_SPEED);
	setMotorR(-CAL_SPEED);
	for (int i = 0; i < sampleSize; i++) {
		x = getMagnetoDataX();
		y = getMagnetoDataY();
		
		minX = min(minX, x);
		minY = min(minY, y);
		
		maxX = max(maxX, x);
		maxY = max(maxY, y);
	
		_delay_ms(50);	
	}
	
	setMotorL(0);
	setMotorR(0);
	
#ifdef DEBUG
	printf("Max X=\t%d\nMax Y=\t%d\nMIN X=\t%d\nMin Y=\t%d\n", maxX, maxY, minX, minY);
#endif
	magnetoVectors.maxX = maxX;
	magnetoVectors.maxY = maxY;
	magnetoVectors.minX = minX;
	magnetoVectors.minY = minY;
}