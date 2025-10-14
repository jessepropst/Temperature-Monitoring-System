/* Name: Jesse Propst
 * Course: CPE 2610 - Embedded Systems
 * Section: 141
 * Assignment: Lab 6 - Timer
 * File: piezo.c
 * Dependencies: gpio.h,tim.h,systick.h
 *
 * Usage: The source for the piezo functions file
 */

#include "piezo.h"
#include "gpio.h"
#include "tim.h"
#include <stdint.h>
#include "systick.h"
#include "led_bar.h"

//create instances of each type
static volatile GPIO_t* const gpiob = (volatile GPIO_t*) GPIOB_BASE;
static volatile TIMx_t* const tim3 = (volatile TIMx_t*) TIM3_BASE;
static volatile TIMx_t* const tim5 = (volatile TIMx_t*) TIM5_BASE;
static volatile uint32_t* nvic_iser1 = (volatile uint32_t*) 0xE000E104;

//Piezo is at PB4
/**
 * Initializes the piezo buzzer to an output
 */
void PiezoInit(){
	//Enable Clock
	volatile uint32_t* rcc = (uint32_t*) RCC_AHB1_ENR;
	*rcc |= (1<<RCC_GPIOB_EN);

	//Set PB04 to alternate mode
	uint32_t temp = gpiob->moder;
	temp &= ~(0b11<<8);
	temp |= (0b10<<8);
	gpiob->moder = temp;

	//Set AFRL 4 to 0010 (PB4)
	temp = gpiob->afrl;
	temp &= ~(0b1111<<(16));
	temp |= (0b0010<<16);
	gpiob->afrl = temp;

	//enable the clock for tim3
	rcc = (uint32_t*) RCC_APB1_ENR;
	*rcc |= (1<<1);

	//set the direction to up
	tim3->CR1 &= ~(0b1<<4);

	//enable the UDIS bit
	tim3->CR1 |= 1<<1;

	//No prescalar
	tim3->PSC = 0;

	//set clock type (internal = 000)
	tim3->SMCR &= ~(0b111);

	//set ccr1 to 0
	tim3->CCR1 = 0;

	//set output compare toggle mode
	tim3->CCMR1 |= 0b011 << 4;

	//enable capture compare
	tim3->CCER |= 0b1<<0;

	//setup for tim5
	//enable the clock for tim5
	rcc = (uint32_t*) RCC_APB1_ENR;
	*rcc |= (1<<3);

	//turn off the timer for the setup
	tim5-> CR1 &= ~(0b1<<0);

	//set the direction to down
	tim5->CR1 |= (0b1<<4);

	//set the prescalar
	tim5->PSC = 7;
	tim5->EGR = 1<<0;
	tim5->SR &= ~(1<<0);

	//enable the nvic for tim5 (Position 50, iser1, bit 18)
	*nvic_iser1 |= 1<<18;

	//Enable the interrupt
	tim5->DIER |= 1<<0;
}
/**
 * Turns the piezo buzzer on
 */
void PiezoOn(){
	tim3->CR1 |= (1<<0);
}

/**
 * Turns the piezo buzzer off
 */
void PiezoOff(){
	tim3->CR1 &= ~(1<<0);
}

/**
 * Changes the frequency at which the piezo is played at
 */
void ChangeFrequency(uint32_t frequency){
	//calculate half the period
	uint32_t halfperiod = 16000000 /(frequency * 2);

	//load the timer (0)
	uint32_t temp = tim3->ARR;
	temp &= ~(0xFFFF);
	temp |= halfperiod;
	tim3->ARR = temp;

	//clear the current value
	tim3->CNT &= (~0xFFFF);
}

/**
 * Plays the described frequency on the piezo buzzer for the given duration
 * Frequency: The frequency (in hertz) that the piezo buzzer will be at when played
 * Duration: The length of time the piezo buzzer will be played for
 */
void PiezoBuzz(uint32_t frequency, uint32_t duration){
	//calculate the period for the input frequency
	//get half of that (half on, half off)
	uint32_t halfperiod = 16000000 /(frequency * 2);

	//disable tim5
	tim5->CR1 &= ~(1<<0);

	//Load the register
	tim5->ARR = (2000* duration);

	//Change the count value to the ARR value
	tim5->CNT = tim5->ARR;

	//load the timer (0)
	uint32_t temp2 = tim3->ARR;
	temp2 &= ~(0xFFFF);
	temp2 |= halfperiod;
	tim3->ARR = temp2;

	//clear the current value
	tim3->CNT &= (~0xFFFF);

	//enable timer5
	tim5->CR1 |= 0b1;

	//enable timer3
	tim3->CR1 |= 0x11;
}

/**
 * Handles the duration of the piezo buzz
 * Shuts off the flag and disables both the timers
 */
void TIM5_IRQHandler(){
	//Turn off the flag
	tim5->SR &= ~(1<<0);

	//Reset the count value (stop the timer and restart)
	tim3->CR1 &= ~(0x1);

	//disable the enable signal
	tim5->CR1 &= ~(0b1);
}
