/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_LCD.h"


#pragma region GLOBAL VARIABLE DEFINITIONS

// LCD display
BigCrystal_I2C LCD(LCD_I2C_ADDRESS, LCD_CHAR_COLS, LCD_CHAR_ROWS);	// Pin A4/A5 (I2C)

#pragma endregion


#pragma region PROGRAM FUNCTIONS

VOID LCD_Initialize()
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

#pragma endregion


#pragma region UTILITY FUNCTIONS

CONST CHAR * CONST LCD_InvertChar(CONST CHAR * lcdChar, BYTE charWidth)
{
	PCHAR newLcdChar = new char[SIZEOF(lcdChar)];

	for (BYTE pixY = 0; pixY < SIZEOF(lcdChar); pixY++)
		newLcdChar[pixY] = lcdChar[pixY] ^ (1 << charWidth - 1);

	return const_cast<CONST CHAR *>(newLcdChar);
}

CONST CHAR * LCD_InvertChar_P(CONST CHAR * lcdChar, BYTE charWidth)
{
	PCHAR lcdCharData = new char[SIZEOF(lcdChar)];

	for (BYTE i = 0; i < SIZEOF(lcdChar); i++)
		lcdCharData[i] = pgm_read_byte_near(lcdChar++);

	return LCD_InvertChar(const_cast<CONST CHAR *>(lcdCharData), charWidth);
}

#pragma endregion

