/**
 * Name:
 * Course: CPE2610
 * Section:
 * Assignment: Lab11
 * File: adc.h
 * Dependencies:
 * Usage: Drivers for ADC
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

#define RCC_APB2ENR 0x40023844
#define ADC1EN 8
#define ADC1_BASE 0x40012000


typedef struct ADC_t {
	uint32_t sr;
	uint32_t cr1;
	uint32_t cr2;
	uint32_t smpr[2];
	uint32_t jofr[4];
	uint32_t htr;
	uint32_t ltr;
	uint32_t sqr[3];
	uint32_t jsqr;
	uint32_t jdr[4];
	uint32_t dr;
	uint32_t ccr;
} ADC_t;

//SR
#define EOC 1

//CR1
#define EOCIE 5
#define SCAN 8

//CR2
#define CONT 1
#define ADON 0
#define SWSTART 30

#endif /* ADC_H_ */
