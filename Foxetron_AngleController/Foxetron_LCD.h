/***********************************************************************************************
* [Foxetron_LCD_chars.h]: HD44780 CHARACTER CODES & CUSTOM CHARACTER DATA
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_LCD_CHARS_H
#define _FOXETRON_LCD_CHARS_H



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



#pragma region FLASH DATA

// CUSTOM LCD CHARACTERS

// Miscellaneous icons
extern PROGMEM const char LCD_CHAR_FOX[8];
extern PROGMEM const char LCD_CHAR_GEM[8];
extern PROGMEM const char LCD_CHAR_GEM_SMALL[8];

// Angle symbols
extern PROGMEM const char LCD_CHAR_ANGLE[8];
extern PROGMEM const char LCD_CHAR_ANGLE_2[8];
extern PROGMEM const char LCD_CHAR_CLOCKWISE[8];
extern PROGMEM const char LCD_CHAR_COUNTER_CCW[8];

// Miscellaneous symbols
extern PROGMEM const char LCD_CHAR_CHECKMARK[8];
extern PROGMEM const char LCD_CHAR_BOX_EMPTY[8];
extern PROGMEM const char LCD_CHAR_BOX_CHECKMARK[8];
extern PROGMEM const char LCD_CHAR_WARNING[8];
extern PROGMEM const char LCD_CHAR_NOT_ALLOWED[8];
extern PROGMEM const char LCD_CHAR_LIGHTNING[8];
extern PROGMEM const char LCD_CHAR_NOTES[8];
extern PROGMEM const char LCD_CHAR_CLOCK_A[8];
extern PROGMEM const char LCD_CHAR_CLOCK_B[8];
extern PROGMEM const char LCD_CHAR_HOURGLASS_1[8];
extern PROGMEM const char LCD_CHAR_HOURGLASS_2[8];
extern PROGMEM const char LCD_CHAR_HOURGLASS_3[8];
extern PROGMEM const char LCD_CHAR_HOURGLASS_4[8];

// Gem cut icons
extern PROGMEM const char LCD_CHAR_CUT_BRILLIANT[8];
extern PROGMEM const char LCD_CHAR_CUT_STEP[8];

// Gem shape (outline) icons
extern PROGMEM const char LCD_CHAR_SHAPE_ROUND[8];
extern PROGMEM const char LCD_CHAR_SHAPE_OVAL[8];
extern PROGMEM const char LCD_CHAR_SHAPE_CUSHION[8];
extern PROGMEM const char LCD_CHAR_SHAPE_SQUARE[8];
extern PROGMEM const char LCD_CHAR_SHAPE_BAGUETTE[8];
extern PROGMEM const char LCD_CHAR_SHAPE_EMERALD[8];
extern PROGMEM const char LCD_CHAR_SHAPE_TRILLIANT[8];
extern PROGMEM const char LCD_CHAR_SHAPE_TRIANGLE[8];
extern PROGMEM const char LCD_CHAR_SHAPE_SPECIAL[8];

// Small arrow icons
extern PROGMEM const char LCD_CHAR_ARROW_UP_SMALL[8];
extern PROGMEM const char LCD_CHAR_ARROW_DOWN_SMALL[8];
extern PROGMEM const char LCD_CHAR_ARROW_LEFT_SMALL[8];
extern PROGMEM const char LCD_CHAR_ARROW_RIGHT_SMALL[8];

// Medium arrow icons
extern PROGMEM const char LCD_CHAR_ARROW_UP[8];
extern PROGMEM const char LCD_CHAR_ARROW_DOWN[8];

// Large arrow icons
extern PROGMEM const char LCD_CHAR_ARROW_LEFT_LARGE[8];
extern PROGMEM const char LCD_CHAR_ARROW_RIGHT_LARGE[8];

// Scrollbar icons
extern PROGMEM const char LCD_CHAR_SCROLLBAR_TOP[8];
extern PROGMEM const char LCD_CHAR_SCROLLBAR_BOTTOM[8];
extern PROGMEM const char LCD_CHAR_SCROLLBAR_1[8];
extern PROGMEM const char LCD_CHAR_SCROLLBAR_2[8];
extern PROGMEM const char LCD_CHAR_SCROLLBAR_3[8];
extern PROGMEM const char LCD_CHAR_SCROLLBAR_4[8];

// Bar graph icons
extern const char * const &  LCD_CHAR_BARGRAPH_EMPTY;
extern PROGMEM const char LCD_CHAR_BARGRAPH_FULL[8];
extern PROGMEM const char LCD_CHAR_BARGRAPH_SEMI_FULL_1[8];
extern PROGMEM const char LCD_CHAR_BARGRAPH_SEMI_FULL_2[8];
extern PROGMEM const char LCD_CHAR_BARGRAPH_1[8];
extern PROGMEM const char LCD_CHAR_BARGRAPH_2[8];
extern PROGMEM const char LCD_CHAR_BARGRAPH_3[8];
extern PROGMEM const char LCD_CHAR_BARGRAPH_4[8];

#pragma endregion FLASH DATA



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
