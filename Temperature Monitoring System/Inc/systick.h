/* Name: Jesse Propst
 * Course: CPE 2610 - Embedded Systems
 * Section: 141
 * Assignment: Lab 5 - Keypad
 * File: systick.h
 * Dependencies:
 * Usage:
 */

#ifndef SYSTICK_H
#define SYSTICK_H


#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

#define SYSTICKBASE 0xE000E010

typedef struct SysTick_t {
	uint32_t STK_CTRL; //Enables SysTick features
	uint32_t STK_LOAD; //Specifies the value to load into STK_VAL
	uint32_t STK_VAL; //Gets the current timer count
	uint32_t STK_CALIB; //Indicates the systick calibration properites
} SysTick_t;

#endif
