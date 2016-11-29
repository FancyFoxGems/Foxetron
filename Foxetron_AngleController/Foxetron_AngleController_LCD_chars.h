/***********************************************************************************************
* [Foxetron_AngleController_LCD_chars.h]: HD44780 CUSTOM CHARACTER DATA (IN FLASH PROGRAM SPACE)
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_LCD_CHARS_H
#define _FOXETRON_LCD_CHARS_H


#pragma region INCLUDES

// ITTY BITTY
#include "IttyBitty_aliases.h"

// AVR LIBS
#include <avr/pgmspace.h>

#pragma endregion


#pragma region FLASH DATA

// CUSTOM LCD CHARACTERS

// Miscellaneous icons
PROGMEM CCHAR LCD_CHAR_FOX[8]					= { 0x11, 0x1B, 0x1F, 0x11, 0x1B, 0x11, 0xE, 0x4 };
PROGMEM CCHAR LCD_CHAR_GEM[8]					= { 0x0, 0x0, 0xE, 0x1B, 0x15, 0xA, 0x4, 0x0 };
PROGMEM CCHAR LCD_CHAR_GEM_SMALL[8]				= { 0x0, 0x0, 0x0, 0xE, 0x11, 0xA, 0x4, 0x0 };

// Gem cut style icons
PROGMEM CCHAR LCD_CHAR_CUT_BRILLIANT[8]			= { 0x0, 0xE, 0x15, 0x1B, 0x15, 0xA, 0x4, 0x0 };
PROGMEM CCHAR LCD_CHAR_CUT_STEP[8]				= { 0x0, 0xE, 0x11, 0x1F, 0x11, 0x1F, 0xA, 0x4 };

#pragma endregion


#endif
