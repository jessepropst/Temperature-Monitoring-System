/*
 * Author: Jesse Propst
 * CPE 2610: Embedded Systems
 * Section: 141
 * Assignment: Lab 12
 * File Name: temp_sensor.h
 * Created on: April 24, 2024
 *
 */

#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

#include <stdint.h>

void tempInit();

uint16_t getTempData();

uint16_t getTempReal(uint16_t data);

#endif
