/*
 * Author: Jesse Propst
 * CPE 2610: Embedded Systems
 * Section: 141
 * Assignment: Lab 12
 * File Name: main.c
 * Created on: April 23, 2024
 * Dependencies: piezo.h, led_ar.h, lcd.h, temp_sensor.h, tim.h
 */


/* Description:
 * The project is to have an timer-interrupt driven temperature monitoring system
 * The system will sample the temperature every 2 seconds and then update the other peripherals accordingly
 * The LCD will display the current temperature on fahrenheit as well as a title message
 * The LEDs will display how hot the temperature is on a scale from 0 to 10 with 0 being the coldest,
 * (less than 53 degrees, and 10 being the hottest (over 80) degrees,
 * The entire system is driven by a timer interrupt which tells the temperature sensor to sample, then
 * tells the system to update the other peripherals
 */

#include <stdio.h>
#include "piezo.h"
#include "led_bar.h"
#include "lcd.h"
#include <stdbool.h>
#include "temp_sensor.h"
#include "tim.h"

//Static types and registers
static volatile TIMx_t* const tim2 = (volatile TIMx_t*) TIM2_BASE;
static volatile uint32_t* nvic_iser0 = (volatile uint32_t*) 0xE000E100;

//Static variables for flgas
static bool sample = false;
static bool update = false;

//Static important value to be used in multiple functions
static uint16_t temp = 0;

// main
int main(void){

	//Initialize all subsystems (Piezo, Led Bar, LCD, and Temperature Sensor)
	PiezoInit();
	LedBarInit();
	LcdSetup();
	LcdInit();
	tempInit();

	//Title Screen
	LcdSetPosition(0,0);
	LcdWriteStr("Temp Monitor");

	//Setup the timer
	//Enable timer 2
	volatile uint32_t* rcc = (uint32_t*) RCC_APB1_ENR;
	*rcc |= (1<<0);

	//turn off the timer for the setup
	tim2-> CR1 &= ~(0b1<<0);

	//set the direction to down
	tim2->CR1 |= (0b1<<4);

	//set the prescalar
	tim2->PSC = 15999; //Run at 1 kHz
	tim2->EGR = 1<<0;
	tim2->SR &= ~(1<<0);

	//enable the nvic for tim2 (Position 28)
	*nvic_iser0 |= 1<<28;

	//Enable the interrupt
	tim2->DIER |= 1<<0;

	//Load the register
	tim2->ARR = (500); //2 seconds

	//Change the count value to the ARR value
	tim2->CNT = tim2->ARR;

	//enable timer2
	tim2->CR1 |= 0b1;

	//Infinite running loop
	while(1){
		//Sample the temperture sensor
		if(sample){

			//Get the sample
			uint16_t data = getTempData();
			temp = getTempReal(data);

			//Turn off the sample flag
			sample = false;

			//Tell the screen to update
			update = true;

		}

		//Tell the peripherals to update
		if(update){
			//Update the screen
			char buf[32];
			sprintf(buf,"Temperature:%d ",temp);
			LcdSetPosition(1,0);
			LcdWriteStr(buf);

			//Update the LED Bar
			if(temp < 53){
				LedBarWrite(0); //0
			} else if (temp < 56){
				LedBarWrite(1);//1
			} else if (temp < 59){
				LedBarWrite(3); //2
			} else if (temp < 62){
				LedBarWrite(7);//3
			} else if(temp < 65){
				LedBarWrite(15); //4
			} else if(temp < 68){
				LedBarWrite(31); //5
			} else if(temp < 71){
				LedBarWrite(63); //6
			} else if(temp < 74){
				LedBarWrite(127);//7
			} else if (temp < 77){
				LedBarWrite(255);//8
			} else if(temp < 80) {
				LedBarWrite(511);//9
			} else {
				LedBarWrite(1023);//10
			}

			//Possibly play noises on the piezo depending on temperature
			if(temp > 77){
				for(int x = 0; x < 1; x++){
					PiezoBuzz(500,250);
				}
			}

			//Turn off the update flag
			update = false;
		}

	}

	return 0;
}

void TIM2_IRQHandler(){

	//Turn off the flag
	tim2->SR &= ~(1<<0);

	//Tell the adc to take a sample
	sample = true;

}

