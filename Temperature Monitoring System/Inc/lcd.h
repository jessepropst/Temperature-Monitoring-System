/* Name: Jesse Propst
 * Course: CPE 2610 - Embedded Systems
 * Section: 141
 * Assignment: Lab 4 - LCD Display
 * File: lcd.h
 * Dependencies: 
 * 		gpio.h - gpio structs
 * Usage: The header for the LCD functions file
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

//Find values for these constant in class with Dr. L.
#define DB_OFFSET 0x4
#define DB_MASK 0xFF<<4
#define BUSYFLAG 11
#define CNTL_OFFSET 8
#define RS 8
#define RW 9
#define E 10
//BSRR Constants
#define RS_DATA (1<<RS)
#define RW_RD (1<<RW)
#define E_SET (1<<E)
#define RS_CMD (1<<(RS+16))
#define RW_WR (1<<(RW+16))
#define E_CLR (1<<(E+16))

//GPIO Constants
#define GPIOA_CLEAR_MODER 0xFFFF00
#define GPIOA_SET_MODER 0x555500
#define GPIOC_CLEAR_MODER 0x3F0000
#define GPIOC_SET_MODER 0x150000
#define LCD_INIT_RS_RW_CLEAR 0x300
#define LCD_INIT_GPIOA_CLEAR 0xFF0
#define LCD_INIT_SET 0b00111000
#define LCD_CLEAR 0b00000001
#define LCD_HOME 0b00000010
#define LCD_ENTRY_MODE 0b00000110
#define RCC_AHB1_ENR 0x40023830

void LcdSetup();

void LcdInit();

void LcdClear();

void LcdHome();

void LcdSetDisplay(bool onOff, bool cursor, bool blink);

void LcdSetPosition(uint8_t row, uint8_t col);

void LcdWriteChar(char c);

void LcdWriteStr(const char * const str);

void LcdGetPosition(uint8_t* const row, uint8_t* const col);

#endif /* LCD_H_ */
