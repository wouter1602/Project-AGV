/*
 * tof.c
 *
 * Created: 27-5-2020 22:03:32
 *  Author: Wouter
 */ 

#include "tof.h"

static VL53L0X_Dev_t Tof0;
static VL53L0X_DEV pTof0 = &Tof0;

void changeTofAddress(uint8_t orignalAddress, uint8_t newAddres) {
	newAddres &= 0x7F;
	
	
}

void initToF(void) {
	Tof0.I2cDevAddr = 0x52;
	Tof0.comms_type = 1;
	Tof0.comms_speed_khz = 100;
	VL53L0X_Version_t version;
	version.build = 0;
	version.major = 0;
	version.minor = 0;
	version.revision = 0;
	
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;
	
	VL53L0X_GetVersion(&version);
	uint8_t major = 0;
	uint8_t minor = 0;
	//printf("Build: %d\nMajor: %d\nMinor: %d\nRevision: %d\n", version.build, version.major, version.minor, version.revision);
	VL53L0X_GetProductRevision(pTof0, &major, &minor);
	
	VL53L0X_DeviceInfo_t InfoTof0;
	
	//printf("\nMajor: %d\nMinor: %d\n", major, minor);
	
	VL53L0X_DataInit(&Tof0);
	
	VL53L0X_GetDeviceInfo(pTof0, &InfoTof0);
	
	//printf("Device: %d\n", InfoTof0.Name);
	
	VL53L0X_StaticInit(pTof0);
	
	uint32_t spadCountTof0 = 0;
	uint8_t isApetureSpadsTof0 = 0;
	
	VL53L0X_perform_ref_spad_management(pTof0, &spadCountTof0, &isApetureSpadsTof0);
	
	uint8_t	vhvSettingsTof0 = 0;
	uint8_t phaseCalTof0 = 0;
	
	VL53L0X_PerformRefCalibration(pTof0, &vhvSettingsTof0, &phaseCalTof0);
	
	// Enable/Disable Sigma and Signal check
	if (Status == VL53L0X_ERROR_NONE) {
		Status = VL53L0X_SetLimitCheckEnable(pTof0, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
	}

	if (Status == VL53L0X_ERROR_NONE) {
		Status = VL53L0X_SetLimitCheckEnable(pTof0, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
	}

	if (Status == VL53L0X_ERROR_NONE) {
		Status = VL53L0X_SetLimitCheckEnable(pTof0, VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD, 1);
	}

	if (Status == VL53L0X_ERROR_NONE) {
		Status = VL53L0X_SetLimitCheckValue(pTof0, VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD, (FixPoint1616_t)(1.5 * 0.023 * 65536));
	}
}

uint16_t getTofData(uint8_t sensor) {
	uint16_t data = 0;
	VL53L0X_RangingMeasurementData_t RangingMeasuementData;
	
	//printf("measurement: %d\n", (uint16_t) RangingMeasuementData.RangeMilliMeter);
	VL53L0X_PerformSingleRangingMeasurement(pTof0, &RangingMeasuementData);
	//printf("Timestamp: %d\nMeasure time: %d\nRange: %d\nRange Max: %d\nSignal Rate: %d\nAmbient RAte: %d\nEffective spad: &d\nZone id: %d\nRange fraction: %d\nRange status: %d\n", RangingMeasuementData.TimeStamp, RangingMeasuementData.MeasurementTimeUsec, RangingMeasuementData.RangeMilliMeter, RangingMeasuementData.RangeDMaxMilliMeter, RangingMeasuementData.SignalRateRtnMegaCps, RangingMeasuementData.AmbientRateRtnMegaCps, RangingMeasuementData.EffectiveSpadRtnCount, RangingMeasuementData.ZoneId, RangingMeasuementData.RangeFractionalPart, RangingMeasuementData.RangeStatus);
	//printf("measurement: %d\n", (uint16_t) RangingMeasuementData.RangeMilliMeter);
	_delay_ms(10);
	//VL53L0X_GetRangingMeasurementData(pTof0, &RangingMeasuementData);
	
	//printf("Timestamp: %d\nMeasure time: %d\nRange: %d\nRange Max: %d\nSignal Rate: %d\nAmbient RAte: %d\nEffective spad: &d\nZone id: %d\nRange fraction: %d\nRange status: %d\n", RangingMeasuementData.TimeStamp, RangingMeasuementData.MeasurementTimeUsec, RangingMeasuementData.RangeMilliMeter, RangingMeasuementData.RangeDMaxMilliMeter, RangingMeasuementData.SignalRateRtnMegaCps, RangingMeasuementData.AmbientRateRtnMegaCps, RangingMeasuementData.EffectiveSpadRtnCount, RangingMeasuementData.ZoneId, RangingMeasuementData.RangeFractionalPart, RangingMeasuementData.RangeStatus);
	data = RangingMeasuementData.RangeMilliMeter;
    return data;
}