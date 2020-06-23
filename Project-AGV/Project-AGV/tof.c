/*
 * tof.c
 *
 * Created: 27-5-2020 22:03:32
 *  Author: Wouter
 */ 

#include "tof.h"

VL53L0X_Dev_t Tof1;
VL53L0X_DEV pTof1 = &Tof1;

static void getDeviceInfo(uint8_t address, uint8_t *data) {
	
}

void changeTofAddress(uint8_t orignalAddress, uint8_t newAddres) {
	newAddres &= 0x7F;
	
	
}

void initToF() {
	Tof1.I2cDevAddr = 0x52;
	Tof1.comms_type = 1;
	Tof1.comms_speed_khz = 100;
	VL53L0X_Version_t version;
	version.build = 0;
	version.major = 0;
	version.minor = 0;
	version.revision = 0;
	VL53L0X_GetVersion(&version);
	uint8_t major = 0;
	uint8_t minor = 0;
	printf("Build: %d\nMajor: %d\nMinor: %d\nRevision: %d\n", version.build, version.major, version.minor, version.revision);
	VL53L0X_GetProductRevision(pTof1, &major, &minor);
	
	printf("\nMajor: %d\nMinor: %d\n", major, minor);
}

uint16_t getTofData(uint8_t sensor) {
	uint16_t data = 0;
    return data;
}