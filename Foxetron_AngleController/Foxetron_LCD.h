/***********************************************************************************************
* [Foxetron_LCD.h]: HD44780 LCD DISPLAY MODULE (W/ RGB BACKLIGHT) VIA I2C; LARGE FONT SUPPORT
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_LCD_H
#define _FOXETRON_LCD_H



#pragma region INCLUDES

// PROJECT INCLUDES
#include "Foxetron_pins.h"

// PROJECT LIBS
#include "libs/BigCrystal_I2C.custom.h"

// AVR LibC
//#include <avr/pgmspace.h>						// included by project 3rd-party libs

#pragma endregion INCLUDES



#pragma region DEFINES

// LCD CONSTANTS

#define LCD_I2C_ADDRESS			0x27
#define LCD_CHAR_COLS			16
#define LCD_CHAR_ROWS			2


// LCD SYMBOL CHARACTERS

// ARROWS
#define LCD_SYMBOL_SCROLL_RIGHT			0x10
#define LCD_SYMBOL_SCROLL_LEFT			0x11
#define LCD_SYMBOL_DOUBLE_ARROW_UP		0x15
#define LCD_SYMBOL_DOUBLE_ARROW_DOWN	0x16
#define LCD_SYMBOL_SPLIT_CIRCLE			0x17
#define LCD_SYMBOL_ARROW_RETURN			0x18
#define LCD_SYMBOL_ARROW_UP				0x19
#define LCD_SYMBOL_ARROW_DOWN			0x1A
#define LCD_SYMBOL_ARROW_RIGHT			0x1A
#define LCD_SYMBOL_ARROW_LEFT			0x1B
#define LCD_SYMBOL_SCROLL_UP			0x1E
#define LCD_SYMBOL_SCROLL_DOWN			0x1F

// MISCELLANEOUS
#define LCD_SYMBOL_HOUSE				0x7f
#define LCD_SYMBOL_NOTE					0x91
#define LCD_SYMBOL_BELL					0x98
#define LCD_SYMBOL_HEART				0x9d
#define LCD_SYMBOL_COPYRIGHT			0xa9
#define LCD_SYMBOL_RESERVED				0xae

#pragma endregion DEFINES



#pragma region GLOBAL VARIABLES

// LCD display
extern BigCrystal_I2C LCD;

#pragma endregion GLOBAL VARIABLES



#pragma region FUNCTION DECLARATIONS

void initializeLCD();

const char * const LCD_invertChar(const char *, byte = 5);
const char * LCD_invertChar_P(const char *, byte = 5);

#pragma endregion FUNCTION DECLARATIONS

#endif
