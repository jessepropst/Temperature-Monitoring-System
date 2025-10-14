/*
 * Author: Jesse Propst
 * CPE 2610: Embedded Systems
 * Section: 141
 * Assignment: Lab 1
 * File Name: delay.h
 *  Created on: Jan 23, 2024
 * Dependencies: Systick.h
 * Resources: Systick
 */

#include "delay.h"
#include <stdint.h>
#include <stdio.h>
#include "systick.h"

#define SECONDS_LOOP_COUNT 1340000
#define MILLI_LOOP_COUNT 1470
#define MICRO_LOOP_COUNT 2

static volatile SysTick_t* const SysTick = (volatile SysTick_t*) SYSTICKBASE;

//Function to delay between blinks
//Calculates the delay in seconds, milliseconds, and microseconds, then calls the busy functions
void Delay(int delay) {
    //divide into seconds, milliseconds, and microseconds
	//use modulus function to calculate
	uint32_t time = delay;

	//Variables for each length of time
	uint32_t seconds = 0;
	uint32_t milli = 0;
	uint32_t micro = 0;

	//Calculates the number of seconds, milliseconds, and microseconds without
	//any of them overlapping
	seconds = (int)(time/1000);
	time -=(seconds * 1000);
	milli = (int)(time);
	time -=(milli);
	micro = time * 1000;

	//Calls the Busy loops for seconds, milliseconds, and microseconds
	DelaySysSec(seconds);
	DelaySysMilliSec(milli);
	DelaySysMicroSec(micro);

}

//Runs an empty busy loop that lasts about 1 second
//Tested to about 2.006 seconds when 2000ms were entered
void BusySec(uint32_t N){
	for(int x = 0; x < N; x++){
		for(int x = 0; x < SECONDS_LOOP_COUNT; x++){

		}
	}
}

//Runs an empty busy loop that lasts about 1 millisecond
//Tested to about 502 ms when 500 ms is entered
void BusyMilli(uint32_t N){
	for(int x = 0; x < N; x++){
		for(int x = 0; x < MILLI_LOOP_COUNT; x++){

		}
	}
}

//Runs an empty busy loop that lasts about 1 microsecond
//Likely not super accurate due to hardware limitations and small units of time
void BusyMicro(uint32_t N){
	for(int x = 0; x < N; x++){
		for(int x = 0; x < MICRO_LOOP_COUNT; x++){

		}
	}
}

//STK_CTRL:
//Bit 0: Enable, 0 - off, 1 - on
//Bit 1: Tickint: Leave alone
//Bit 2: Clock Source: 0 - AHB/8 (2 MHz), 1- AHB (16 MHz)


/**
 * Runs a delay in microseconds times the N input value
 */
void DelaySysMicroSec(uint32_t N){
	//1. Program reload value.
	//2. Clear current value.
	//3. Program Control and Status register


	//Check if the number is valid so the top 8 registers dont get clobbered
	if((N * 16) < 0xFFFFFF){
		//disable the enable signal
		SysTick->STK_CTRL = 0;

		//Setup the load register, Each clock cycle is 62.5ns, *16 to get to 1us
		SysTick->STK_LOAD= (16 * N);

		//Clear the current value register
		SysTick->STK_VAL = 0;

		//Setup the ctrl register, AHB, Enable
		SysTick->STK_CTRL = (0x5 << 0);

		//wait until the count flag is 1
		while((SysTick->STK_CTRL & 0x00010000) == 0){

		}

		//disable the enable signal
		SysTick->STK_CTRL = 0;
	}
}

/**
 * Runs a delay in milliseconds times the N input value
 */
void DelaySysMilliSec(uint32_t N){
	//1. Program reload value.
	//2. Clear current value.
	//3. Program Control and Status register

	//Check if the number is valid so the top 8 registers dont get clobbered
	if((N * 2000) < 0xFFFFFF){
		//disable the enable signal
		SysTick->STK_CTRL = 0;

		//Setup the load register
		SysTick->STK_LOAD =(2000 * N);

		///Clear the current value register
		SysTick->STK_VAL = 0;

		//Setup the ctrl register, AHB/8, Enable
		SysTick->STK_CTRL = (0x1 << 0);

		//wait until the count flag is 1
		while((SysTick->STK_CTRL & 0x00010000) == 0){

		}

		//disable the enable signal
		SysTick->STK_CTRL = 0;
	}

}

/**
 * Runs a delay in seconds times the N input value
 */
void DelaySysSec(uint32_t N){
	//1. Program reload value.
	//2. Clear current value.
	//3. Program Control and Status register

	//Check if the number is valid so the top 8 registers dont get clobbered
		if((N * 2000000) < 0xFFFFFF){
			//disable the enable signal
			SysTick->STK_CTRL = 0;

			//Setup the load register
			SysTick->STK_LOAD =(2000000 * N);

			///Clear the current value register
			SysTick->STK_VAL = 0;

			//Setup the ctrl register, AHB/8, Enable
			SysTick->STK_CTRL = (0x1 << 0);


			//wait until the count flag is 1
			while((SysTick->STK_CTRL & 0x00010000) == 0){

			}

			//disable the enable signal
			SysTick->STK_CTRL = 0;
		}
}









