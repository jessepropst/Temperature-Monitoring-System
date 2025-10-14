/*
 * Author: Jesse Propst
 * CPE 2610: Embedded Systems
 * Section: 141
 * Assignment: Lab 1
 * File Name: delay.h
 * Created on: Jan 23, 2024
 *
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>
#include <stdio.h>

void Delay(int delay);

void BusySec(uint32_t N);

void BusyMilli(uint32_t N);

void BusyMicro(uint32_t N);

void DelaySysMicroSec(uint32_t N);

void DelaySysMilliSec(uint32_t N);

void DelaySysSec(uint32_t N);

#endif /* DELAY_H_ */
