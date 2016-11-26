/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/

// GCC WARNING SUPPRESSIONS
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"

#include "Foxetron_AngleController_LCD.h"

using namespace Foxetron;

#include <avr/pgmspace.h>


#pragma region GLOBAL VARIABLE DEFINITIONS

// LCD display
BigCrystal_I2C * LCD = NULL;	// Pin A4/A5 (I2C)

#pragma endregion


#pragma region PROGRAM FUNCTION DEFINITIONS

VOID LCD_Initialize()
{
	LCD = new BigCrystal_I2C(LCD_I2C_ADDRESS, LCD_COLS, LCD_ROWS);

	LCD->init();
	LCD->backlight();
	LCD->home();

	//LCD_LoadBigFont();
}

VOID LCD_Free()
{
	delete LCD;
	LCD = NULL;
}

#pragma endregion


namespace Foxetron
{
#pragma region UTILITY FUNCTION DEFINITIONS

	PBYTE LCD_CreateScrollBarChar(BYTE cellPercentage, CLCDSCROLLBARINDICATOR indicator)
	{
		PBYTE scrollBarChar = new byte[LCD_CHAR_HEIGHT];

		BYTE scrollerRow = cellPercentage == 100 ? LCD_CHAR_HEIGHT - 1 : cellPercentage * LCD_CHAR_HEIGHT / 100;

		for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
		{
			if (pixY == scrollerRow)
				scrollBarChar[pixY] = LCD_SCROLLER_PIXEL_ROW;
			else
				scrollBarChar[pixY] = 0;
		}

		return scrollBarChar;
	}

	PBYTE LCD_CreateGraphSpaceChar(CLCDGRAPHSPACES spaceStyle)
	{
		PBYTE spaceChar = new byte[LCD_CHAR_HEIGHT];

		switch (spaceStyle)
		{
		case LcdGraphSpaces::DASH_SPACE:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				spaceChar[pixY] = 0;

			spaceChar[LCD_CHAR_HEIGHT / 2 + 1] = LCD_CHAR_LINE_PIXEL_ROW;

			break;


		case LcdGraphSpaces::DOTS_SPACE:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				spaceChar[pixY] = 0;

			spaceChar[LCD_CHAR_HEIGHT / 2 + 1] = LCD_CHAR_DOTS_PIXEL_ROW;

			break;


		case LcdGraphSpaces::PLUS_SPACE:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				spaceChar[pixY] = 0;

			spaceChar[2] = LCD_CHAR_CENTER_PIXEL_ROW;
			spaceChar[3] = LCD_CHAR_SMALL_DASH_PIXEL_ROW;
			spaceChar[4] = LCD_CHAR_CENTER_PIXEL_ROW;

			break;


		case LcdGraphSpaces::CIRCLE_SPACE:

			for (BYTE pixY = 1; pixY < LCD_CHAR_HEIGHT - 1; pixY++)
				spaceChar[pixY] = pgm_read_byte_near(LCD_CHAR_SHAPE_ROUND + pixY);

			break;


		case LcdGraphSpaces::SQUARE_SPACE:

			for (BYTE pixY = 1; pixY < LCD_CHAR_HEIGHT - 1; pixY++)
				spaceChar[pixY] = LCD_CHAR_FIRST_LAST_PIXEL_ROW;

			spaceChar[1] = LCD_CHAR_LINE_PIXEL_ROW;
			spaceChar[LCD_CHAR_HEIGHT - 2] = LCD_CHAR_LINE_PIXEL_ROW;

			break;


		default:

			break;
		}

		return spaceChar;
	}

	PBYTE LCD_CreateGraphFullCellChar(CLCDGRAPHCELLSTYLE cellStyle, PBYTE spaceChar)
	{
		PBYTE fullCellChar = new byte[LCD_CHAR_HEIGHT];

		switch (cellStyle)
		{
		case LcdGraphCellStyle::BAR_GRAPH:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				fullCellChar[pixY] = LCD_CHAR_LINE_PIXEL_ROW;

			break;


		case LcdGraphCellStyle::LINE_GRAPH:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				fullCellChar[pixY] = spaceChar[pixY];

			fullCellChar[LCD_CHAR_HEIGHT / 2 + 1] |= LCD_CHAR_LINE_PIXEL_ROW;

			break;


		case LcdGraphCellStyle::SQUARE_GRAPH:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
			{
				fullCellChar[pixY] = spaceChar[pixY];

				if (pixY >= 3 && pixY <=5)
					fullCellChar[pixY] |= LCD_CHAR_SMALL_DASH_PIXEL_ROW;
			}

			break;


		case LcdGraphCellStyle::DISC_GRAPH:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				fullCellChar[pixY] = spaceChar[pixY];

			for (BYTE pixY = 1; pixY < LCD_CHAR_HEIGHT - 1; pixY++)
				fullCellChar[pixY] |= pgm_read_byte_near(LCD_CHAR_SHAPE_ROUND + pixY);

			fullCellChar[2] |= LCD_CHAR_CENTER_PIXEL_ROW;
			fullCellChar[3] |= LCD_CHAR_SMALL_DASH_PIXEL_ROW;
			fullCellChar[4] |= LCD_CHAR_CENTER_PIXEL_ROW;

			break;
		}


		return fullCellChar;
	}

	PBYTE LCD_CreateGraphSemiCellChar(PBYTE fullCellChar)
	{
		PBYTE semiCellChar = new byte[LCD_CHAR_HEIGHT];

		for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
			semiCellChar[pixY] = fullCellChar[pixY];

		for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
		{
			if (pixY % 2)
				semiCellChar[pixY] &= LCD_CHAR_SEMI_MASK_ODD;
			else
				semiCellChar[pixY] &= LCD_CHAR_SEMI_MASK_EVEN;
		}

		return semiCellChar;
	}

	PBYTE LCD_CreateGraphPartialCellChar(BYTE cellPercentage, PBYTE fullCellChar)
	{
		PBYTE partialCellChar = new byte[LCD_CHAR_HEIGHT];

		for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
		{
			partialCellChar[pixY] = fullCellChar[pixY] BAND
				(LCD_CHAR_LINE_PIXEL_ROW - (B(cellPercentage == 100 ?
					LCD_CHAR_WIDTH - 1 : cellPercentage * LCD_CHAR_WIDTH / 100) - 1));
		}

		return partialCellChar;
	}

#pragma endregion


#pragma region LCD FUNCTION DEFINITIONS

	VOID LCD_LoadBigFont()
	{
		BYTE customChar[LCD_CHAR_HEIGHT];

		for (BYTE i = 0; i < LCD_CHAR_HEIGHT; i++)
		{
			for (BYTE j = 0; j < LCD_CHAR_HEIGHT; j++)
				customChar[j] = pgm_read_byte_near(BF_fontShapes + (i * LCD_CHAR_HEIGHT) + j);

			LCD->createChar(i, customChar);
		}
	}

	PBYTE LCD_CreateInvertedChar(PBYTE lcdChar)
	{
		PBYTE newLcdChar = new byte[LCD_CHAR_HEIGHT];

		for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
			newLcdChar[pixY] = LCD_CHAR_PIXEL_ROW_MASK - lcdChar[pixY];

		return newLcdChar;
	}

	PBYTE LCD_CreateInvertedChar_P(PCCHAR lcdChar)
	{
		PCHAR lcdCharData = new char[LCD_CHAR_HEIGHT];

		for (BYTE i = 0; i < LCD_CHAR_HEIGHT; i++)
			lcdCharData[i] = pgm_read_byte_near(lcdChar++);

		return LCD_CreateInvertedChar((PBYTE)lcdCharData);
	}

	VOID LCD_LoadInvertedChar(BYTE customCharIdx, PBYTE lcdChar)
	{
		PBYTE invertedLcdChar = LCD_CreateInvertedChar(lcdChar);

		LCD->createChar(customCharIdx, invertedLcdChar);

		delete invertedLcdChar;
	}

	VOID LCD_LoadInvertedChar_P(BYTE customCharIdx, PCCHAR lcdChar)
	{
		PBYTE invertedLcdChar = LCD_CreateInvertedChar_P(lcdChar);

		LCD->createChar(customCharIdx, invertedLcdChar);

		delete invertedLcdChar;
	}

	VOID LCD_DrawScrollBar(BYTE percentage, CLCDSCROLLBAROPTIONS options)
	{
		BYTE col = LcdScrollBarOptionsToLcdScrollBarPosition(options)
			== LcdScrollBarPosition::LEFT_POSITION ? 0 : LCD_COLS - 1;
		BYTE currRow = percentage == 100 ? LCD_ROWS - 1 : percentage * LCD_ROWS / 100;

		PBYTE scrollBarChar = LCD_CreateScrollBarChar(percentage == 100 ? 100 :
				(percentage % (100 / LCD_ROWS)) * LCD_ROWS,
			LcdScrollBarOptionsToLcdScrollBarIndicator(options));
		LCD->createChar(0x7, scrollBarChar);

		delete scrollBarChar;

		for (BYTE row = 0; row < LCD_ROWS; row++)
		{
			LCD->setCursor(col, row);

			if (row == currRow)
				LCD->write(0x7);
			else
				LCD->write(LCD_SYMBOL_BLANK);
		}
	}

	VOID LCD_DrawGraph(BYTE row, BYTE startCol, BYTE widthChars, BYTE percentage, CLCDGRAPHOPTIONS options)
	{
		CLCDGRAPHCELLSTYLE cellStyle = LcdGraphOptionsToLcdGraphCellStyle(options);
		CLCDGRAPHSPACES spaceStyle = LcdGraphOptionsToLcdGraphSpaces(options);

		PBYTE spaceChar = LCD_CreateGraphSpaceChar(spaceStyle);
		LCD->createChar(0x7, spaceChar);

		PBYTE fullCellChar = LCD_CreateGraphFullCellChar(cellStyle, spaceChar);
		LCD->createChar(0x6, fullCellChar);

		if (percentage % (100 / widthChars) > 0)
		{
			PBYTE partialCellChar = NULL;

			if (LcdGraphOptionsToLcdGraphPartialStyle(options) == LcdGraphPartialStyle::SEMI_PARTIAL)
			{
				partialCellChar = LCD_CreateGraphSemiCellChar(fullCellChar);
			}
			else
			{
				partialCellChar = LCD_CreateGraphPartialCellChar(
					(percentage % (100 / widthChars)) * widthChars, fullCellChar);
			}

			LCD->createChar(0x5, partialCellChar);

			delete partialCellChar;
		}

		delete spaceChar;
		delete fullCellChar;

		BYTE cellPercentage = 0;

		for (BYTE col = 0; row < widthChars; col++)
		{
			LCD->setCursor(col + startCol, row);

			if (percentage <= col * 100 / widthChars)
				LCD->write(0x7);
			else if (percentage >= (col + 1) * 100 / widthChars)
				LCD->write(0x6);
			else
				LCD->write(0x5);
		}
	}

#pragma endregion
};
