/* Name: Jesse Propst
 * Course: CPE 2610 - Embedded Systems
 * Section: 141
 * Assignment: Lab 6 - Timer
 * File: piezo.c
 * Dependencies: non
 *
 * Usage: The header for the piezo functions file
 */

#ifndef PIEZO_H_
#define PIEZO_H_

#include <stdint.h>

#define GPIOB_MODER 0x40020400

void PiezoInit();

void ChangeFrequency();

void PiezoOn();

void PiezoOff();

void PiezoBuzz(uint32_t frequency, uint32_t duration);

#endif /* PIEZO_H_ */
