/*
* magneto.c
*
* Created: 25-5-2020 20:31:15
*  Author: Wouter
*/

#include "magneto.h"

/*
 * Static struct that stores vector value's for calibration.
 */
static struct Vector {
	int16_t maxX;
	int16_t maxY;
	int16_t maxZ;
	int16_t minX;
	int16_t minY;
	int16_t minZ;
}magnetoVectors;

/*
 * Gets magneto data for the X-axes
 * returns signed 16-bit value from the sensor.
 */
static int16_t getMagnetoDataX(void) {
	int16_t data = 0;
	uint8_t twiData[3] ={MAGNETO_ADDR, (OUT_X_L_M | (1 << 7)), (MAGNETO_ADDR | 1)};

	
	twiReadRS(twiData, 2, 3);					//Read X-axes magneto sensor
	data = (twiData[1] << 8) | twiData[2];		//Combine two 8-bit values to one 16-bit value
	return data;
}

/*
 * Gets magneto data for the Y-axes
 * returns signed 16-bit value from the sensor.
 */
static int16_t getMagnetoDataY(void) {
	int16_t data = 0;
	uint8_t twiData[3] = {MAGNETO_ADDR, (OUT_Y_L_M | (1 << 7)), (MAGNETO_ADDR | 1)};
	
	twiReadRS(twiData, 2, 3);				//Read Y-axes magneto sensor
	data = (twiData[2] << 8) | twiData[1];	//Combine two 8-bit values to one 16-bit value
	return data;
}

/*
 * Gets temperature data from the Magneto sensor.
 * returns signed 16-bit value from the sensor.
 */
static int16_t getMagnetoTempData(void) {
	int16_t data = 0;
	uint8_t twiData[3] = {MAGNETO_ADDR, (TEMP_OUT_L | (1 << 7)), (MAGNETO_ADDR | 1)};
	
	twiReadRS(twiData, 2 , 3);				//Read temperature magneto sensor
	data = (twiData[2] << 8) | twiData[1];	//Combine two 8-bit values to one 16-bit value
	
	return data;
} //Currently not used

/*
 * Gets acceleration data form X-axes from the Magneto sensor.
 * returns signed 16-bit value from the sensor.
 */
static int16_t getAcceleroDataX(void) {
	int16_t data = 0;
	uint8_t twiData[3] = {MAGNETO_ADDR, (OUT_X_L_A | (1 << 7)), (MAGNETO_ADDR | 1)};
	
	twiReadRS(twiData, 2 , 3);				//Read accelerometer X-axes data magneto sensor
	data = (twiData[2] << 8) | twiData[1];	//Combine two 8-bit values to one 16-bit value
	
	return data;
} //Currently not used

/*
 * Gets acceleration data form Y-axes from the Magneto sensor.
 * returns signed 16-bit value from the sensor.
 */
static int16_t getAcceleroDataY(void) {
	int16_t data = 0;
	uint8_t twiData[3] = {MAGNETO_ADDR, (OUT_Y_L_A | (1 << 7)), (MAGNETO_ADDR | 1)};
	
	twiReadRS(twiData, 2 , 3);				//Read accelerometer Y-axes data magneto sensor
	data = (twiData[2] << 8) | twiData[1];	//Combine two 8-bit values to one 16-bit value
	
	return data;
} //Currently not used

/*
 * Gets acceleration data form Z-axes from the Magneto sensor.
 * returns signed 16-bit value from the sensor.
 */
static int16_t getAcceleroDataZ(void) {
	int16_t data = 0;
	uint8_t twiData[3] = {MAGNETO_ADDR, (OUT_Z_L_A | (1 << 7)), (MAGNETO_ADDR | 1)};
	
	twiReadRS(twiData, 2 , 3);				//Read accelerometer Z-axes data magneto sensor
	data = (twiData[2] << 8) | twiData[1];	//Combine two 8-bit values to one 16-bit value
	
	return data;
}//Currently not used

/*
 * Sets up the Magneto sensor through TWI (I²C).
 * Start measurement on 50Hz whith high resolution.
 * setup default value's in the calibration array
 */
void initMagneto(void) {		
	const uint8_t data2[3] = {MAGNETO_ADDR, CTRL2, 0};		//Enable accelerometer's
	const uint8_t data3[3] = {MAGNETO_ADDR, CTRL1, 0x57};	//Enable magneto sensor on 50Hz,
	const uint8_t data4[3] = {MAGNETO_ADDR, CTRL5, 0xEC};	//Enable Tmp, High resolution magneto, 25 Hz measurements.
	const uint8_t data5[3] = {MAGNETO_ADDR, CTRL6, 0x20};	//+/- 4 gaus magneto measurement
	const uint8_t data6[3] = {MAGNETO_ADDR, CTRL7, 0};		
	
	twiWrite(data2, 3);										//Enable accelerometer's.
	twiWrite(data3, 3);										//Enable Temp, magneto sensor (High accuracy) and 25 Hz measurement
	twiWrite(data4, 3);										//Set magnetic gaus to +/- 4
	twiWrite(data5, 3);										//Enable magnetic to continuous-conversion mode
	twiWrite(data6, 3);
	
	//Put default value's in array. (measured from previous tests)
	magnetoVectors.maxX = MAX_X;
	magnetoVectors.maxY = MAX_Y;
	magnetoVectors.minX = Min_X;
	magnetoVectors.minY = MIN_Y;
}

/*
 * Returns a 32-bit float with an average value of the magneto sensor X-axes data.
 * The amount of times measured is defined by the "MAGNETO_AVG" define.
 */
float getAvgMagnetoDataX(void) {
	float avgX = 0.0;					//Set's the float to 0.0
	for (int i = 0; i < MAGNETO_AVG; i++) {
		avgX += getMagnetoDataX();		//Calculate the sum of X-axes magneto data
	}
	
	avgX /= MAGNETO_AVG;				//Divide sum by the times measured.
	
	return avgX;
}

/*
 * Return a 32-bit float with an average value of the magneto sensor Y-axes data.
 * The amount of times measured is defined by the "MAGNETO_AVG" define.
 */
float getAvgMagnetoDataY(void) {
	float avgY = 0.0;				//Set's the float to 0.0
	for (int i = 0; i < MAGNETO_AVG; i++) {
		avgY += getMagnetoDataX();	//Calculate the sum of Y-axes magneto data
	}
	
	avgY /= MAGNETO_AVG;				//Divide sum by the times measured
	
	return avgY;
}

/*
 * Returns an 32-bit float of the absolute angle in degrees.
 * The angle is based on the earths magnetic field.
 * the array that should be send should be 2 long.
 * the first should be the magneto X-axes data the second should be the magneto Y-axes data.
 */
float magnetoHeading(float *data, uint16_t size) {
	float x_scaled = 2.0*(float) (data[0] - magnetoVectors.minX) / ( magnetoVectors.maxX - magnetoVectors.minX) - 1.0;		//Scale the X-axes with the calibrated minimum and maximum.
	float y_scaled = 2.0*(float) (data[1] - magnetoVectors.minY) / ( magnetoVectors.maxY - magnetoVectors.minY) - 1.0;		//Scale the Y-axes with the calibrated minimum and maximum.
	
	float angle = atan2(y_scaled, x_scaled) * 180 / M_PI;		//Calculate angle.
	if (angle < 0) {
		angle += 360;					//If the angle is below 0 360 gets added to keep it within the 360 degree rotation range.
	}
	return angle;
}

/*
 * Calibrate the Magneto sensor Minimum and Maximum for the X and Y-axes.
 * sampleSize is the amount of times this function should gather data for an average.
 * The data gets stored in an local struct for other functions to use.
 */
void magnetoCallibrate(uint8_t sampleSize) {
	int16_t x, y;
	int16_t minX, minY, maxX, maxY;
	
	setMotorL(CAL_SPEED);			//Let's the Zumo rotate so the angles of the magneto sensor change
	setMotorR(-CAL_SPEED);
	for (int i = 0; i < sampleSize; i++) {
		x = getMagnetoDataX();		//Gather X-axes data
		y = getMagnetoDataY();		//Gather Y-axes data
		
		minX = min(minX, x);		//Check if the data gathered is the lowest it has seen
		minY = min(minY, y);
		
		maxX = max(maxX, x);		//Check if the data gathered is the highest it has seen
		maxY = max(maxY, y);
	
		_delay_ms(50);				//Added delay so it waits till the Zumo has rotated before getting a new sample
	}
	
	setMotorL(0);					//Disable the motors
	setMotorR(0);
	
#ifdef DEBUG
	printf("Max X=\t%d\nMax Y=\t%d\nMIN X=\t%d\nMin Y=\t%d\n", maxX, maxY, minX, minY);
#endif
	magnetoVectors.maxX = maxX;		//Puts gathered data in calibration struct.
	magnetoVectors.maxY = maxY;
	magnetoVectors.minX = minX;
	magnetoVectors.minY = minY;
}