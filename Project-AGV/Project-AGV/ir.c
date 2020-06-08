/*
 * ir.c
 *
 * Created: 27-5-2020 22:08:54
 *  Author: Wouter
 */ 

#include "ir.h"

inline uint8_t getIrData(void) {
	return ((PORTB >> IR) & 1);
}