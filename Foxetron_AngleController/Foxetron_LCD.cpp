/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_LCD.h"



#pragma region GLOBAL VARIABLES

// LCD display
extern BigCrystal_I2C LCD(LCD_I2C_ADDRESS, LCD_CHAR_COLS, LCD_CHAR_ROWS);	// Pin A4/A5 (I2C)

#pragma endregion GLOBAL VARIABLES



#pragma region FUNCTION DEFINITIONS

// PROGRAM FUNCTIONS

void initializeLCD()
{
	LCD.init();
	LCD.backlight();
	LCD.home();

	// Load large font
	uint8_t customChar[8];
	for (uint8_t i = 0; i < 8; i++)
	{
		for (uint8_t j = 0; j < 8; j++)
			customChar[j] = pgm_read_byte_near(BF_fontShapes + (i * 8) + j);
		LCD.createChar(i, customChar);
	}
}


// UTILITY FUNCTIONS

const char * const LCD_invertChar(const char * lcdChar, byte charWidth)
{
	char * newLcdChar = new char[sizeof(lcdChar)];

	for (byte pixY = 0; pixY < sizeof(lcdChar); pixY++)
		newLcdChar[pixY] = lcdChar[pixY] ^ (1 << charWidth - 1);

	return const_cast<const char *>(newLcdChar);
}

const char * LCD_invertChar_P(const char * lcdChar, byte charWidth)
{
	char lcdCharData[sizeof(lcdChar)];

	for (byte i = 0; i < sizeof(lcdChar); i++)
		lcdCharData[i] = pgm_read_byte_near(lcdChar++);

	return LCD_invertChar(const_cast<const char *>(lcdCharData), charWidth);
}

#pragma endregion FUNCTION DEFINITIONS

