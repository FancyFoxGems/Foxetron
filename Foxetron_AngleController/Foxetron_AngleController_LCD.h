/***********************************************************************************************
* [Foxetron_AngleController_LCD.h]: HD44780 LCD DISPLAY MODULE VIA I2C; LARGE FONT SUPPORT
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#pragma GCC diagnostic ignored "-Wparentheses"

#ifndef _FOXETRON_LCD_H
#define _FOXETRON_LCD_H


#pragma region INCLUDES

// ITTY BITTY
#include "IttyBitty_LCD_chars.h"
#include "IttyBitty_LCD_I2C.h"
#include "IttyBitty_LCD_extensions.h"

using namespace IttyBitty;

// PROJECT INCLUDES
#include "Foxetron_AngleController_pins.h"

// AVR LibC
//#include <avr/pgmspace.h>						// included by project 3rd-party libs

#pragma endregion


#pragma region DEFINES

// LCD OPTIONS/CONSTANTS

#define LCD_I2C_ADDRESS			0x27

#define LCD_COLS				20
#define LCD_ROWS				4

#pragma endregion


#pragma region CUSTOM LCD CHARACTER FLASH DATA

// Miscellaneous icons
PROGMEM CBYTE LCD_CHAR_FOX[8]					= { 0x11, 0x1B, 0x1F, 0x11, 0x1B, 0x11, 0xE, 0x4 };
PROGMEM CBYTE LCD_CHAR_GEM[8]					= { 0x0, 0x0, 0xE, 0x1B, 0x15, 0xA, 0x4, 0x0 };
PROGMEM CBYTE LCD_CHAR_GEM_SMALL[8]				= { 0x0, 0x0, 0x0, 0xE, 0x11, 0xA, 0x4, 0x0 };

// Gem cut style icons
PROGMEM CBYTE LCD_CHAR_CUT_BRILLIANT[8]			= { 0x0, 0xE, 0x15, 0x1B, 0x15, 0xA, 0x4, 0x0 };
PROGMEM CBYTE LCD_CHAR_CUT_STEP[8]				= { 0x0, 0xE, 0x11, 0x1F, 0x11, 0x1F, 0xA, 0x4 };

#pragma endregion


#pragma region LCD GLOBAL VARIABLE DECLARATION

// LCD display
EXTERN PLCDI2C<LCD_COLS, LCD_ROWS, LCD_I2C_ADDRESS> LCD;

#pragma endregion


#pragma region PROGRAM FUNCTION DECLARATIONS

VOID LCD_Initialize();
VOID LCD_Free();

#pragma endregion

#endif
