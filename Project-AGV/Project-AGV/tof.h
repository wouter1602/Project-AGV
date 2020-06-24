/*
 * tof.h
 *
 * Created: 27-5-2020 22:03:41
 *  Author: Jochem
 */ 


#ifndef TOF_H_
#define TOF_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "pins.h"
#include "twi.h"
#include "Thirdparty/VL53L0x_api/vl53l0x_api.h"
#include "Thirdparty/VL53L0x_api/vl53l0x_api_calibration.h"

void initToF(void);
uint16_t getTofData(uint8_t sensor);

#endif /* TOF_H_ */