/*
 * Author: Jesse Propst
 * CPE 2610: Embedded Systems
 * Section: 141
 * Assignment: Lab 12
 * File Name: temp_sensor.h
 * Created on: April 24, 2024
 * Dependencies: adc.h, gpio.h
 *
 */

#include "temp_sensor.h"
#include "adc.h"
#include "gpio.h"
#include <stdint.h>

static volatile ADC_t* adc1 = (ADC_t*) ADC1_BASE;
static volatile GPIO_t* gpiob = (GPIO_t*) GPIOB_BASE;

void tempInit(){

	//Enable GPIOB
	volatile uint32_t* rcc = (uint32_t*) RCC_AHB1_ENR;
	*rcc |= (1<<RCC_GPIOB_EN);

	//Enable the ADC Clock
	rcc = (uint32_t*) RCC_APB2_ENR;
	*rcc |= (1<<8);

	//Set the temperature sensor to analog mode
	gpiob->moder |= (0b11<<0);

	//Setup other ADC Settings
	//Select the number of samples
	adc1->sqr[0] &= ~(0xF00000);

	//Disable ADC interrupts
	adc1->cr1 &= ~(1<<EOCIE);

	//Enable scan mode
	adc1->cr1 |= (1<<SCAN);

	//Enable the ADC on CR2
	adc1->cr2 |= 1<<ADON;

	//Setup SQR1 for the temp sensor (Channel 8)
	adc1->sqr[2] &= ~(0x1F);
	adc1->sqr[2] |= 8; //SQ1 = Channel 8
}

uint16_t getTempData(){
	//Enable the sample
	adc1->cr2 |= 1<<SWSTART;

	//Wait for the conversion to complete
	while(!(adc1->sr&(1<<EOC))){

	}

	//Return the sample
	return (adc1->dr & 0xFFFF);
}

uint16_t getTempReal(uint16_t data){
	//Get the correct voltage
	double millivolts = (data * 3300)/4095;

	//Convert to celsius (0.5 V offset, 10mv/C)
	double celsius = -50 + (millivolts/10);

	//Convert to fahrenheit
	double fahrenheit = ((celsius * 9)/5) + 32;

	return (uint16_t)fahrenheit;
}

