/* Name: Jesse Propst
 * Course: CPE 2610 - Embedded Systems
 * Section: 141
 * Assignment: Lab 4 - LCD Display
 * File: lcd.c
 * Dependencies: Delay.h, lcd.h, push_button.h
 * Usage: Controls the functions for the LCD
 */

//Includes
//Necessary standard libraries
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "delay.h"
#include "lcd.h"

//Custom header files

//File static pointers to necessary gpio ports
static volatile GPIO_t* const gpioa = (volatile GPIO_t*) GPIOA_BASE;
static volatile GPIO_t* const gpioc = (volatile GPIO_t*) GPIOC_BASE;

//Static helper functions
/**
 * lcdWriteCmd()
 * Description:
 * 		Sends command over to display.
 * 		Steps:
 * 		1. E=1,RS->CMD,RW->W
 * 		2. CMD-> DB
 * 		3. E=0,RS->CMD,RW->W
 * 		4. Wait appropriate delay or (EC)Poll BF
 * 	Inputs:
 * 		cmd(uint8_t) - cmd to execute on the display
 * 	Outputs: None
 */
static void LcdWriteCmd(uint8_t cmd){
	//set enable high, RS = CMD, RW = W
	gpioc->bsrr = E_SET | RS_CMD | RW_WR;

	//send the command to the odr
	uint16_t temp = gpioa->odr;
	temp &= ~(DB_MASK);
	temp |= (cmd << DB_OFFSET);
	gpioa->odr = temp;

	//set enable to low, Rs to cmd, rw to w
	gpioc->bsrr = E_CLR | RS_CMD | RW_WR;

	//delay 37 microseconds, works for almost every command
	BusyMicro(37);
}


/** EXTRA CREDIT
 * lcdWaitBusyFlag()
 * Description:
 * 		Blocks polling the BusyFlag until it reads '0'
 * 		Note: The mode of DB7 must be switched to input during polling
 * 				and back to output after polling is complete.
 */
//static void lcdWaitBusyFlag();


/**
 * LcdSetup()
 * Description:
 * 		Configure PA[11..4] for use as 8-bit output Databus(DB)
 * 		Configure PC8 as output for RS
 * 		Configure PC9 as output for RW
 * 		Configure PC10 as output for E
 * 		**For EC: Turn of Input Resistors in PA[11..4]
 * Inputs: None
 * Outputs: None
 */
void LcdSetup(){
	//enable rcc for GPIO_A and GPIO_C
    volatile uint32_t* rcc = (uint32_t*) RCC_AHB1_ENR;
    *rcc |= (1<<RCC_GPIOA_EN);
    *rcc |= (1<<RCC_GPIOC_EN);

    uint32_t temp = gpioa->moder;

	//Clear GPIO_A->moder 11-4
    temp &= ~(GPIOA_CLEAR_MODER);

    //Set GPIO_A_>moder pins 11-4 to 01 (output mode)
    temp |= (GPIOA_SET_MODER);

    gpioa->moder = temp;

    temp = gpioc->moder;

    //Clear GPIO_C Moder 8-10
    temp &= ~(GPIOC_CLEAR_MODER);

    //Set GPIO_C Moder 8-10 to 01 (output mode)
    temp |= (GPIOC_SET_MODER);

    gpioc->moder = temp;
}

/**
 * LcdInit()
 * Description:
 * 		Wait 40 MS
 * 		Set Function
 * 			8bit mode DL = 1
 * 			2Line mode N = 1
 * 			5x8 dots   F = 0
 * 		Set Function (again)
 * 		Display ON
 * 			DisplayOn D=1
 * 			CursorOn  C=1
 * 			BlinkOn   B=1
 * 		Display Clear (note longer delay)
 * 		Entry Mode Set
 * 			IncrementAddress I/D=1
 * 			ShiftOff S=0
 * Inputs: None
 * Outputs: None
 */
void LcdInit(){
	//Wait for 40 ms
	BusyMilli(40);

	//set function
	LcdWriteCmd(LCD_INIT_SET);

	LcdWriteCmd(LCD_INIT_SET);

	//Turn the display on
	LcdSetDisplay(1,1,1);

	//LCD Clear
	LcdClear();

	//Entry Mode Set
	LcdWriteCmd(LCD_ENTRY_MODE);
}

/**
 * LcdClear()
 * Description:
 * 		Clear Display
 * Inputs: None
 * Outputs: None
 */
void LcdClear(){
	//write clear command
	LcdWriteCmd(LCD_CLEAR);

	//delay for 1.52 ms, already have 37 us of delay
	BusyMilli(1);
	BusyMicro(483);
}

/**
 * LcdHome()
 * Description:
 * 		Move Cursor to the home position (0,0)
 * Inputs: None
 * Outputs: None
 */
void LcdHome(){
	//write clear command
	LcdWriteCmd(LCD_HOME);

	//delay for 1.52 ms, already have 37 us of delay
	BusyMilli(1);
	BusyMicro(483);
}

/**
 * LcdSetDisplay()
 * Description:
 * 		Set/change the display options
 * 	Inputs:
 * 		onOff(bool) - true (display on), false (display off)
 * 		cursor(bool) - true (cursor on), false (cursor off)
 * 		blink(bool) - true (blink on), flase (blink off)
 * 	Outputs: none
 */
void LcdSetDisplay(bool onOff, bool cursor, bool blink){
	uint8_t command =0b00001000;
	if(onOff){
		command |= (0b1<<2);
	}

	if(cursor){
		command |= (0b1<<1);
	}

	if(blink){
		command |= (0b1<<0);
	}

	LcdWriteCmd(command);
}

/**
 * LcdSetPosition()
 * Description:
 * 		Set the position of the cursor.
 * 		Top left (0,0)     Top right (0,15)
 * 		Bottom left (1,0)  Bottom right (1,15)
 * 	Inputs:
 * 		row(uint8_t) - row index, 0-top, 1-bottom
 * 		col(uint8_t) - col index, 0-left, 1-right
 */
void LcdSetPosition(uint8_t row, uint8_t col){
	//set the column for the cursor
	uint8_t position = 0;
	position = col;

	//|= (0x40) to get to second row
	if(row == 1){
		position |= 0x40;
	}

	//add a one for db7
	position |= (1<<7);
	LcdWriteCmd(position);
}

/**
 * LcdWriteChar()
 * Description:
 * 		Writes the character to screen according table in
 * 		documentation.
 * 		(See lcdWriteCmd for implementation details)
 * 	Input:
 * 		c(char) - char to write
 * 	Output: none
 */
void LcdWriteChar(char c){
	//set enable high, RS = DATA, RW = W
	gpioc->bsrr = E_SET | RS_DATA | RW_WR;

	//send the command to the odr
	uint16_t temp = gpioa->odr;
	temp &= ~(DB_MASK);
	temp |= (c <<(DB_OFFSET));
	gpioa->odr = temp;

	//set enable to low, Rs to cmd, rw to w
	gpioc->bsrr = E_CLR | RS_CMD | RW_WR;

	//delay 37 microseconds, works for almost every command
	BusyMicro(37);
}

/**
 * LcdWriteStr()
 * Description:
 * 		Writes string to the current console.
 * 		Note: no protection provided for overwriting end of screen
 * 	Input:
 * 		str(char*) - string to write
 * 	Output:
 * 		None
 */
void LcdWriteStr(const char * const str){
	uint16_t size = strlen(str);

	for(int x = 0; x < size; x++){
		LcdWriteChar(str[x]);
	}
}

/** ExtraCredit
 * LcdGetPosition()
 * Description:
 * 		Finds the current position of the cursor.
 * 	Inputs:
 * 		row(uint8_t*) - returned row location by reference
 * 		col(uint8_t*) - returned col location by reference
 */
void LcdGetPosition(uint8_t* const row, uint8_t* const col){

}
