/*
 * vl53l0x_twi_translation.h
 *
 * Created: 22-6-2020 22:31:06
 *  Author: Wouter
 */ 


#ifndef VL53L0X_TWI_TRANSLATION_H_
#define VL53L0X_TWI_TRANSLATION_H_

#include <avr/io.h>
#include "vl53l0x_def.h"
#include "../../twi.h"

VL53L0X_Error VL53L0X_write_multi(uint8_t address, uint8_t index, uint8_t* pdata, uint32_t count);
VL53L0X_Error VL53L0X_read_multi(uint8_t address, uint8_t index, uint8_t* pdata, uint32_t count);
VL53L0X_Error VL53L0X_write_byte(uint8_t address, uint8_t index, uint8_t data);
VL53L0X_Error VL53L0X_write_word(uint8_t address, uint8_t index, uint16_t data);
VL53L0X_Error VL53L0X_write_dword(uint8_t address, uint8_t index, uint32_t data);
VL53L0X_Error VL53L0X_read_byte(uint8_t address, uint8_t index, uint8_t* data);
VL53L0X_Error VL53L0X_read_word(uint8_t address, uint8_t index, uint16_t* data);
VL53L0X_Error VL53L0X_read_uint32_t(uint8_t address, uint8_t index, uint32_t* data);

#endif /* VL53L0X_TWI_TRANSLATION_H_ */