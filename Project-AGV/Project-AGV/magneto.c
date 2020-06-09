/*
* magneto.c
*
* Created: 25-5-2020 20:31:15
*  Author: Wouter
*/

#include "magneto.h"

static struct Vector3 {
	int32_t x;
	int32_t y;
	int32_t z;
};
typedef struct Vector3 Vector3;

static struct MagnetoVector {
	Vector3 max;
	Vector3 min;
};
typedef struct MagnetoVector MagnetoVector;

MagnetoVector magnetoVectors;

static int16_t getMagnetoDataX(void) {
	int16_t data = 0;
	uint8_t twiData[3] ={MAGNETO_ADDR, (OUT_X_L_M | (1 << 7)), (MAGNETO_ADDR | 1)};

	
	twiReadRS(twiData, 2, 3);
	data = (twiData[2] << 8) | twiData[1];
	return data;
}

static int16_t getMagnetoDataY(void) {
	int16_t data = 0;
	uint8_t twiData[3] = {MAGNETO_ADDR, (OUT_Y_L_M | (1 << 7)), (MAGNETO_ADDR | 1)};
	
	twiReadRS(twiData, 2, 3);
	data = (twiData[2] << 8) | twiData[1];
	return data;
}

static int16_t getMagnetoDataZ(void) {
	int16_t data = 0;
	uint8_t twiData[3] = {MAGNETO_ADDR, (OUT_Z_L_M | (1 << 7)), (MAGNETO_ADDR | 1)};
	
	twiReadRS(twiData, 2, 3);
	data = (twiData[2] << 8) | twiData[1];
	return data;
}

static float getMagnetoDataXAvg(void) {
	float data = 0;
	for (int i = 0; i < CAL_TIMES; i++)	{
		data += getMagnetoDataX();
	}
	
	data /= CAL_TIMES;
	return round(data);
}

static float getMagnetoDataYAvg(void) {
	float data = 0;
	for (int i = 0; i < CAL_TIMES; i++)	{
		data += getMagnetoDataY();
	}
	
	data /= CAL_TIMES;
	return round(data);
}

static float getMagnetoDataZAvg(void) {
	int32_t data = 0;
	for (int i = 0; i < CAL_TIMES; i++)	{
		data += getMagnetoDataZ();
	}
	
	data /= CAL_TIMES;
	return round(data);
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
	twiWrite(data4, 3); //Set magnetic Gauss to +/- 4
	twiWrite(data5, 3); //Enalbe magnetic to continous-conversion mode
	twiWrite(data6, 3);
	
	magnetoVectors.max.x = MAX_X;
	magnetoVectors.max.y = MAX_Y;
	magnetoVectors.min.x = Min_X;
	magnetoVectors.min.y = MIN_Y;
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
		avgY += getMagnetoDataY();
	}
	
	avgY /= MAGNETO_AVG;
	
	return avgY;
}

float getAvgMagnetoDataZ(void) {
	float avgZ = 0.0;
	for (int i = 0; i < MAGNETO_AVG; i++) {
		avgZ += getMagnetoDataZ();
	}
	
	avgZ /= MAGNETO_AVG;
	
	return avgZ;
}

float magnetoHeading(float *data, uint16_t size) {
	float x_scaled = 2.0*(float) (data[0] - magnetoVectors.min.x) / ( magnetoVectors.max.x - magnetoVectors.min.x) - 1.0;
	float y_scaled = 2.0*(float) (data[1] - magnetoVectors.min.y) / ( magnetoVectors.max.y - magnetoVectors.min.y) - 1.0;
	
	float angle = atan2(y_scaled, x_scaled) * 180 / M_PI;
	if (angle < 0) {
		angle += 360;
	}
	return angle;
}

void magnetoCallibrate(uint8_t sampleSize) {
	int32_t x, y;
	MagnetoVector vector = {0};
	vector.max.x = -9999;
	vector.max.y = -9999;
	
	setMotorL(CAL_SPEED);
	setMotorR(-CAL_SPEED);
	for (int i = 0; i < sampleSize; i++) {
		x = (int32_t) round(getMagnetoDataXAvg());
		y = (int32_t) round(getMagnetoDataYAvg());
		
		vector.min.x = min(vector.min.x, x);
		vector.min.y = min(vector.min.y, y);
		
		vector.max.x = max(vector.max.x, x);
		vector.max.y = max(vector.max.y, y);
		
		_delay_ms(50);
	}
	
	setMotorL(0);
	setMotorR(0);
	
#ifdef DEBUG
	printf("Max X=\t%d\nMax Y=\t%d\nMIN X=\t%d\nMin Y=\t%d\n",(int16_t) (vector.max.x),(int16_t) (vector.max.y),(int16_t) (vector.min.x),(int16_t) (vector.min.y));
#endif
	magnetoVectors.max.x = vector.max.x;
	magnetoVectors.max.y = vector.max.y;
	magnetoVectors.min.x = vector.min.x;
	magnetoVectors.min.y = vector.min.y;
}

float getMagnetoHeading(void) {
	float data[2] = {0.0};
	data[0] = getAvgMagnetoDataX();
	data[1] = getAvgMagnetoDataY();
	return magnetoHeading(data, 2);
}