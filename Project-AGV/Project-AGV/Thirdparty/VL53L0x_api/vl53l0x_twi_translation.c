/*
 * vl53l0x_twi_translation.c
 *
 * Created: 22-6-2020 22:30:43
 *  Author: Wouter
 */ 

#include "vl53l0x_twi_translation.h"

/*
 * Translates ST api I2C write to be used with the TWI functions.
 */
VL53L0X_Error VL53L0X_write_multi(uint8_t address, uint8_t index, uint8_t* pdata, uint32_t count){ 
	VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint8_t data[MAX_BUFFER_LENGTH] = {0};
	data[0] = address;
	data[1] = index;
	for (uint32_t i = 0; i < count; i++) {
		data[i+2] = pdata[i];
	}
	
	status = twiWrite(data, (count+2));
	
	if (status == 0)
	{
		return VL53L0X_ERROR_NONE;
	} else {
		return VL53L0X_ERROR_UNDEFINED;
	}
	
}

/*
 * Translates ST api I2C read to be used with the TWI functions.
 */
VL53L0X_Error VL53L0X_read_multi(uint8_t address, uint8_t index, uint8_t* pdata, uint32_t count) {
	VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint8_t data[MAX_BUFFER_LENGTH] = {0};
	
	data[0] = address;
	data[1] = index;
	data[2] = address | 1;
	status = twiReadRS(data, 2, count);
	for (uint32_t i = 0; i < count; i++) {
		pdata[i] = data[i+1];
	}
	if (status == 0)
	{
		return VL53L0X_ERROR_NONE;
	} else {
		return VL53L0X_ERROR_UNDEFINED;
	}
}

/*
 * Translates ST api I2C write to be used with the TWI functions.
 */
VL53L0X_Error VL53L0X_write_byte(uint8_t address, uint8_t index, uint8_t data) {
	VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint8_t array[3] = {address, index, data};
	
	status = twiWrite(array, 3);
	
	if (status == 0)
	{
		return VL53L0X_ERROR_NONE;
	} else {
		return VL53L0X_ERROR_UNDEFINED;
	}
}

/*
 * Translates ST api I2C write to be used with the TWI functions.
 */
VL53L0X_Error VL53L0X_write_word(uint8_t address, uint8_t index, uint16_t data) {
	VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint8_t array[4] = {address, index, (data >> 8), (data & 0xFF)};
		
	status = twiWrite(array, 4);
	
	if (status == 0)
	{
		return VL53L0X_ERROR_NONE;
	} else {
		return VL53L0X_ERROR_UNDEFINED;
	}
}

/*
 * Translates ST api I2C write to be used with the TWI functions.
 */
VL53L0X_Error VL53L0X_write_dword(uint8_t address, uint8_t index, uint32_t data) {
	VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint8_t array[6] = {address, index, ((data >> 42) & 0xFF), ((data >> 16) & 0xFF), ((data >> 8) & 0xFF), (data & 0xFF)};
		
	status = twiWrite(array, 6);
	
	if (status == 0) {
		return VL53L0X_ERROR_NONE;
	} else {
		return VL53L0X_ERROR_UNDEFINED;
	}
}

/*
 * Translates ST api I2C read to be used with the TWI functions.
 */
VL53L0X_Error VL53L0X_read_byte(uint8_t address, uint8_t index, uint8_t* data) {
	VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint8_t array[3] = {address, index, (address | 1)};
		
	status = twiReadRS(array, 2, 2);
	
	*data = array[1];
	if (status == 0)
	{
		return VL53L0X_ERROR_NONE;
	} else {
		return VL53L0X_ERROR_UNDEFINED;
	}
}

/*
 * Translates ST api I2C read to be used with the TWI functions.
 */
VL53L0X_Error VL53L0X_read_word(uint8_t address, uint8_t index, uint16_t* data) {
	VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint8_t array[3] = {address, index, (address | 1)};
	
	status = twiReadRS(array, 2 , 3);
	
	*data = (array[1] << 8) | array[0];
	if (status == 0) {
		return VL53L0X_ERROR_NONE;
	} else {
		return VL53L0X_ERROR_UNDEFINED;
	}
}

/*
 * Translates ST api I2C read to be used with the TWI functions.
 */
VL53L0X_Error VL53L0X_read_uint32_t(uint8_t address, uint8_t index, uint32_t* data) {
	VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint8_t array[5] = {address, index, (address | 1)};
		
	status = twiReadRS(array, 2, 5);
	
	*data = (array[1] << 42) | (array[2] << 16) | (array[3] << 8) | array[4]; 
	
	if (status == 0)
	{
		return VL53L0X_ERROR_NONE;
	} else {
		return VL53L0X_ERROR_UNDEFINED;
	}
}