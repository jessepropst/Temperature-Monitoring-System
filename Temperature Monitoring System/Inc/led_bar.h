/**
 * Name: Jesse Propst
 * Course: CPE2610
 * Section: 141
 * Assignment: Lab 2
 * File: led_bar.h
 * Dependencies: None
 */

#ifndef LED_BAR_H_
#define LED_BAR_H_

#include <stdint.h>
#include <stdbool.h>

#define RCC_AHB1_ENR 0x40023830
#define RCC_GPIOB_EN 1

#define GPIOB_MODER 0x40020400
#define GPIOB_MODE_OUT 0b01

#define GPIOB_ODR 0x40020414
#define GPIOB_BSRR 0x40020418

//Bit masks to clear and set the GPIOB to output mode
#define GPIOB_CLEAR 0xFF3FFC00
#define GPIOB_SET 0x55155400

//Masks used to read the top and bottom of the LEDs
#define READ_MASK1 0x7E0
#define READ_MASK2 0xF000

//Clears the extra bits in the roll left function
#define ROLL_LEFT_PATTERN 0x3FF

/**
 * LedBarInit()
 * Description:
 * 		Configures PB5-PB10,PB12-PB15 as outputs
 * Inputs: None
 * Outputs: None
 */
void LedBarInit();

/**
 * LedBarOn()
 * Description:
 * 		Turns on the LED at the index
 * Inputs:
 * 		index - 0-9 the index of light to be activated
 * 			Note: out of range args should have no effect.
 * Outputs: none
 *
 */
void LedBarOn(uint8_t index);

/**
 * LedBarOff()
 * Description:
 * 		Turns off the LED at the index
 * Inputs:
 * 		index - 0-9 the index of light to be activated
 * 			Note: out of range args should have no effect.
 * Outputs: none
 *
 */
void LedBarOff(uint8_t index);

/**
 * LedBarToggle()
 * Description:
 * 		Reverses the state the LED at the index
 * Inputs:
 * 		index - 0-9 the index of light to be activated
 * 			Note: out of range args should have no effect.
 * Outputs: none
 *
 */
void LedBarToggle(uint8_t index);

/**
 * LedBarWrite()
 * Description:
 * 		Write a pattern to the Led Bar
 * 	Inputs:
 * 		pattern - the 10 least significant bit are
 * 						written to the lights
 * 	Outputs: None
 */
void LedBarWrite(uint16_t pattern);

/**
 * LedBarRead()
 * Description:
 * 		Read the current from Led Bar
 * 	Inputs: None
 * 	Return: pattern - the 10 least significant bits are
 * 						current light patterns
 */
uint16_t LedBarRead();

/**
 * LedBarRollRight()
 * Description:
 * 		Shift the pattern one bit the right.
 * 		LSBit becomes MSBit.
 * 	Inputs: None
 * 	Outputs: None
 */
void LedBarRollRight();

/**
 * LedBarRollRight()
 * Description:
 * 		Shift the pattern one bit the left.
 * 		MSBit becomes LSBit.
 * 	Inputs: None
 * 	Outputs: None
 */
void LedBarRollLeft();




#endif /* LED_BAR_H_ */
