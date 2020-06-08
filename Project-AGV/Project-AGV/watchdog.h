/* 
 * File:   watchdog.h
 * Author: Wouter
 *
 * Created on 8 juni 2020, 14:21
 */

#ifndef WATCHDOG_H
#define	WATCHDOG_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

void initWatchdog(void);

uint8_t startWatchdog(uint8_t *addrs);
uint8_t stopWatchdog(void);

#endif	/* WATCHDOG_H */

