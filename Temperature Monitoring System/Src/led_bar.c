/*
 * Author: Jesse Propst
 * CPE 2610: Embedded Systems
 * Section: 141
 * Assignment: Lab 2
 * File Name: led_bar.c
 * Created on: Jan 31, 2024
 *
 */

#include "led_bar.h"
#include <stdint.h>
#include <stdio.h>


//Initialize PB5-PB10 and PB12-PB15 to be outputs
void LedBarInit(){
	///Enable Clock
    volatile uint32_t* rcc = (uint32_t*) RCC_AHB1_ENR;
    *rcc |= (1<<RCC_GPIOB_EN);

    //Make temp variable so the user doesn't see the changes
    volatile uint32_t* moder = (uint32_t*) GPIOB_MODER;
    uint32_t temp = *moder;

	//set pins as output
    //First clear the 2 bits, then set them to 01, then copy to moder
    temp &= ~(GPIOB_CLEAR); // Clear each of the 2 bit patterns, leave others
    temp |= (GPIOB_SET); //Set each of the 2 bits for PB5-10,PB12-15, leave the others
    *moder = temp;
}
/**
 * Turns on the specified LED on the light bar
 * index: The index between 0 and 9 on the led bar to turn on
 */
void LedBarOn(uint8_t index){
	//Offset by 5 since we start at PB5
	if(index >= 0  && index < 6){
		volatile uint32_t* odr = (uint32_t*) GPIOB_ODR;
				*odr |= (1<<(5 + index));
	}

	//Offset by 6 since we start at PB12
	if((index >= 6) && (index <= 9)){
		volatile uint32_t* odr = (uint32_t*) GPIOB_ODR;
		*odr |= (1<<(6 + index));
	}
	//does nothing if index is not between 0 and 9
}
/**
 * Turns off the specified LED on the light
 * index: The index between 0 and 9 for the led bar to turn off
 */
void LedBarOff(uint8_t index){
	//Offset by 5 since we start at PB5
	if(index >= 0  && index < 6){
		volatile uint32_t* odr = (uint32_t*) GPIOB_ODR;
				*odr &= ~(1<<5 + index);
	}

	//Offset by 6 since we start at PB 12
	if((index >= 6) && (index <= 9)){
		volatile uint32_t* odr = (uint32_t*) GPIOB_ODR;
		*odr &= ~(1<< 6 + index);
	}
	//does nothing if index is not between 0 and 9
}

/**
 * Toggles(switches from on->off or off->on) a specific LED based on the specified index
 * index: The index between 0 and 9 for the led bar to toggle
 */
void LedBarToggle(uint8_t index){
	if((index >= 0) && (index < 6)){
		volatile uint32_t* odr = (uint32_t*) GPIOB_ODR;
		*odr ^= (1<< (5 + index));
	}

	if((index >= 6) && (index <= 9)){
		volatile uint32_t* odr = (uint32_t*) GPIOB_ODR;
		*odr ^= (1<< (6 + index));
	}
	//does nothing if not between 0 and 9
}

/**
 * Write a 16 bit pattern to the LEDs by using the 10 least significant bits
 * Pattern: The pattern to display on the LEDs
 */
void LedBarWrite(uint16_t pattern){
	uint16_t mask;
	uint16_t bit;
	uint16_t val;

	for(int x = 0; x < 10; x++){
		//reset the mask everytime the loop runs
		mask = 0;
		bit = 0;
		val  = 0;
		//Mask is 1 shifted by x
		mask = 1 << x;
		//Extracts a single bit my anding pattern with
		bit = pattern & mask;
		val = bit >> x;
		if(val == 1){
			LedBarOn(x);
		} else{
			LedBarOff(x);
		}

	}
}


uint16_t LedBarRead(){
	//get the odr to find the registers
	volatile uint32_t* odr = (uint32_t*) GPIOB_ODR;

	//Setup masks to extract bits 5-10 and 12-15
	uint16_t mask1 = READ_MASK1;
	uint16_t mask2 = READ_MASK2;

	//Get the bottom bits
	uint16_t bottom = mask1 & *odr;
	bottom = (bottom >> 5);

	//Get the top bits
	uint16_t top = mask2 & *odr;
	top = (top >> 6);

	//join the top and bottom bits and return the number
	uint16_t Leds = bottom + top;
	return Leds;
}
/**
 * Shifts the entire LED bar right by one, takes the rightmost bit and puts it in the leftmost spot
 */
void LedBarRollRight(){
	//Read the bits
	uint16_t read = LedBarRead();

	//get the bit that would get cut off
	uint16_t end = read & (1);

	//shift right by 1 and add the last bit
	uint16_t pattern = (read >> 1);
	pattern = pattern | (end<<9);
	LedBarWrite(pattern);
}

/**
 * Shifts the entire LED bar left by one, takes the leftmost bit and puts it in the rightmost spot
 */
void LedBarRollLeft(){
	//read the bits
	uint16_t read = LedBarRead();

	//get the bit that would get cut off
	uint16_t end = read & (1<<9);
	end = (end >> 9);

	//shift left by 1, clear the first 6 bits, and add the cut off bit
	uint16_t pattern = (read << 1);
	pattern = pattern & ROLL_LEFT_PATTERN;
	pattern = pattern | (end);
	LedBarWrite(pattern);
}
