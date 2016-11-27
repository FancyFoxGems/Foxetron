/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/

// GCC WARNING SUPPRESSIONS
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"

#include "IttyBitty_util.h"

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
#pragma region LCD FUNCTION CONSTANT DEFINITIONS

	CBYTE LCD_CELL_SEMI_MASKS[3][3]	= { { 0x9, 0x12, 0x4 }, { 0x15, 0xA, 0xFF }, { 0x16, 0xD, 0x1B } };

#pragma endregion


#pragma region UTILITY FUNCTION DEFINITIONS

	PBYTE LCD_CreateScrollBarChar(BYTE cellPercentage, CLCDSCROLLBARINDICATOR indicator)
	{
		PBYTE scrollBarChar = new byte[LCD_CHAR_HEIGHT];

		BYTE scrollerRow = cellPercentage == 100 ? LCD_CELL_LAST_PIXEL_ROW : cellPercentage * LCD_CHAR_HEIGHT / 100;

		for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
		{
			if (pixY == scrollerRow)
				scrollBarChar[pixY] = LCD_SCROLLER_PIXEL_ROW;
			else
				scrollBarChar[pixY] = 0;
		}

		return scrollBarChar;
	}

	PBYTE LCD_CreateSpaceChar(CLCDSPACETYLE spaceStyle)
	{
		PBYTE spaceChar = new byte[LCD_CHAR_HEIGHT];

		switch (spaceStyle)
		{
		case LcdSpaceStyle::DASH_SPACE:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
			{
				if (pixY == LCD_CELL_MIDDLE_PIXEL_ROW)
					spaceChar[pixY] = LCD_CELL_LINE_PIXEL_ROW;
				else
					spaceChar[pixY] = LCD_CELL_BLANK_PIXEL_ROW;
			}

			break;


		case LcdSpaceStyle::PLUS_SPACE:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
			{
				switch (pixY)
				{
				case 3:
				case 5:

					spaceChar[pixY] = LCD_CELL_CENTER_PIXEL_ROW;
					break;


				case 4:

					spaceChar[pixY] = LCD_CELL_SMALL_DASH_PIXEL_ROW;
					break;


				default:

					spaceChar[pixY] = LCD_CELL_BLANK_PIXEL_ROW;
				}
			}

			break;


		case LcdSpaceStyle::SQUARE_SPACE:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
			{
				switch (pixY)
				{
				case 2:
				case LCD_CHAR_HEIGHT - 2:

					spaceChar[pixY] = LCD_CELL_LINE_PIXEL_ROW;
					break;


				case 3:
				case 4:
				case 5:

					spaceChar[pixY] = LCD_CELL_FIRST_LAST_PIXEL_ROW;
					break;


				default:

					spaceChar[pixY] = LCD_CELL_BLANK_PIXEL_ROW;
				}
			}


		case LcdSpaceStyle::DIAMOND_SPACE:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				spaceChar[pixY] = pgm_read_byte_near(LCD_CHAR_SHAPE_DIAMOND + pixY);

			break;


		case LcdSpaceStyle::CIRCLE_SPACE:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				spaceChar[pixY] = pgm_read_byte_near(LCD_CHAR_SHAPE_CIRCLE + pixY);

			break;


		case LcdSpaceStyle::LINES_SPACE:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
			{
				switch (pixY)
				{
				case 0:
				case LCD_CHAR_HEIGHT - 1:

					spaceChar[pixY] = LCD_CELL_BLANK_PIXEL_ROW;
					break;


				default:

					spaceChar[pixY] = LCD_CELL_DOTS_PIXEL_ROW;
				}
			}

			break;


		case LcdSpaceStyle::DOTS_SPACE:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
			{
				if (pixY == LCD_CELL_MIDDLE_PIXEL_ROW)
					spaceChar[pixY] = LCD_CELL_DOTS_PIXEL_ROW;
				else
					spaceChar[pixY] = LCD_CELL_BLANK_PIXEL_ROW;
			}

			break;


		default:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				spaceChar[pixY] = LCD_CELL_BLANK_PIXEL_ROW;

			break;
		}

		return spaceChar;
	}

	PBYTE LCD_CreateGraphFullCellChar(CLCDGRAPHCELL cellStyle, PBYTE spaceChar)
	{
		PBYTE fullCellChar = new byte[LCD_CHAR_HEIGHT];

		switch (cellStyle)
		{
		case LcdGraphCell::BLOCK_CELL:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				fullCellChar[pixY] = LCD_CELL_LINE_PIXEL_ROW;

			break;


		case LcdGraphCell::LINE_CELL:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				fullCellChar[pixY] = spaceChar[pixY];

			fullCellChar[LCD_CELL_MIDDLE_PIXEL_ROW - 1] |= LCD_CELL_LINE_PIXEL_ROW;
			fullCellChar[LCD_CELL_MIDDLE_PIXEL_ROW] |= LCD_CELL_LINE_PIXEL_ROW;

			break;


		case LcdGraphCell::PLUS_CELL:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				fullCellChar[pixY] = spaceChar[pixY];

			fullCellChar[3] |= LCD_CELL_CENTER_PIXEL_ROW;
			fullCellChar[4] |= LCD_CELL_SMALL_DASH_PIXEL_ROW;
			fullCellChar[5] |= LCD_CELL_CENTER_PIXEL_ROW;

			break;


		case LcdGraphCell::SQUARE_CELL:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
			{
				fullCellChar[pixY] = spaceChar[pixY];

				if (pixY >= 3 AND pixY <=5)
					fullCellChar[pixY] |= LCD_CELL_SMALL_DASH_PIXEL_ROW;
			}

			break;


		case LcdGraphCell::DISC_CELL:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				fullCellChar[pixY] = spaceChar[pixY];

			for (BYTE pixY = 2; pixY < LCD_CHAR_HEIGHT - 1; pixY++)
				fullCellChar[pixY] |= pgm_read_byte_near(LCD_CHAR_SHAPE_ROUND + pixY);

			break;


		case LcdGraphCell::DIAMOND_CELL:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				fullCellChar[pixY] = spaceChar[pixY];

			for (BYTE pixY = 2; pixY < LCD_CHAR_HEIGHT - 1; pixY++)
				fullCellChar[pixY] |= pgm_read_byte_near(LCD_CHAR_SHAPE_DIAMOND + pixY);

			fullCellChar[3] |= LCD_CELL_DOTS_SPARSE_PIXEL_ROW;
			fullCellChar[5] |= LCD_CELL_DOTS_SPARSE_PIXEL_ROW;

			break;


		case LcdGraphCell::STRIPES_CELL:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				fullCellChar[pixY] = LCD_CELL_DOTS_PIXEL_ROW;

			break;


		default:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				fullCellChar[pixY] = LCD_CELL_LINE_PIXEL_ROW;
		}


		return fullCellChar;
	}

	PBYTE LCD_CreateGraphPartialCellChar(BYTE cellPercentage, PBYTE fullCellChar, PBYTE spaceChar)
	{
		PBYTE partialCellChar = new byte[LCD_CHAR_HEIGHT];

		for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
		{
			partialCellChar[pixY] = spaceChar[pixY] BOR MASK(fullCellChar[pixY],
				LCD_CELL_LINE_PIXEL_ROW - (B((100 - cellPercentage) * LCD_CHAR_WIDTH / 100) - 1));
		}

		return partialCellChar;
	}

	PBYTE LCD_CreateGraphSemiFillCellChar(BYTE cellPercentage, PBYTE fullCellChar)
	{
		PBYTE semiCellChar = new byte[LCD_CHAR_HEIGHT];

		for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
		{
			if (cellPercentage <= 33)
				semiCellChar[pixY] = MASK(fullCellChar[pixY], LCD_CELL_SEMI_MASKS[0][pixY % 3]);
			else if (cellPercentage <= 66)
				semiCellChar[pixY] = MASK(fullCellChar[pixY], LCD_CELL_SEMI_MASKS[1][pixY % 2]);
			else
				semiCellChar[pixY] = MASK(fullCellChar[pixY], LCD_CELL_SEMI_MASKS[2][pixY % 3]);
		}

		return semiCellChar;
	}

	PBYTE LCD_CreateSliderMarker(CLCDSLIDERMARKER markerStyle)
	{
		PBYTE markerChar = new byte[LCD_CHAR_HEIGHT];

		switch (markerStyle)
		{
		case LcdSliderMarker::CARET_MARKER:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
			{
				switch (pixY)
				{
				case 1:

					markerChar[pixY] = LCD_CELL_CENTER_PIXEL_ROW;
					break;


				case 2:

					markerChar[pixY] = LCD_CELL_DOTS_SPARSE_PIXEL_ROW;
					break;


				default:

					markerChar[pixY] = LCD_CELL_BLANK_PIXEL_ROW;
				}
			}

			break;


		case LcdSliderMarker::LINE_MARKER:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				markerChar[pixY] = LCD_CELL_CENTER_PIXEL_ROW;

			break;


		case LcdSliderMarker::PLUS_MARKER:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
			{
				switch (pixY)
				{
				case 3:
				case 5:

					markerChar[pixY] = LCD_CELL_CENTER_PIXEL_ROW;
					break;


				case 4:

					markerChar[pixY] = LCD_CELL_SMALL_DASH_PIXEL_ROW;
					break;


				default:

					markerChar[pixY] = LCD_CELL_BLANK_PIXEL_ROW;
				}
			}

			break;


		case LcdSliderMarker::SQUARE_MARKER:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
			{
				switch (pixY)
				{
				case 3:
				case 4:
				case 5:

					markerChar[pixY] = LCD_CELL_SMALL_DASH_PIXEL_ROW;
					break;


				default:

					markerChar[pixY] = 0;
				}
			}

			break;


		case LcdSliderMarker::DIAMOND_MARKER:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				markerChar[pixY] = pgm_read_byte_near(LCD_CHAR_SHAPE_ROUND + pixY);

			break;


		case LcdSliderMarker::DISC_MARKER:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				markerChar[pixY] = pgm_read_byte_near(LCD_CHAR_SHAPE_ROUND + pixY);

			break;


		case LcdSliderMarker::TRIANGLE_MARKER:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				markerChar[pixY] = pgm_read_byte_near(LCD_CHAR_SHAPE_TRIANGLE + pixY);

			break;


		case LcdSliderMarker::STRIPES_MARKER:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				markerChar[pixY] = LCD_CELL_DOTS_SPARSE_PIXEL_ROW;

			break;


		default:

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				markerChar[pixY] = LCD_CELL_LINE_PIXEL_ROW;
		}

		return markerChar;
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
			newLcdChar[pixY] = LCD_CELL_LINE_PIXEL_ROW - lcdChar[pixY];

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
		CLCDGRAPHCELL cellStyle = LcdGraphOptionsToLcdGraphCell(options);
		CLCDSPACETYLE spaceStyle = LcdGraphOptionsToLcdSpaceStyle(options);

		PBYTE spaceChar = LCD_CreateSpaceChar(spaceStyle);
		LCD->createChar(0x7, spaceChar);

		PBYTE fullCellChar = LCD_CreateGraphFullCellChar(cellStyle, spaceChar);
		LCD->createChar(0x6, fullCellChar);

		if (percentage % (100 / widthChars) > 0)
		{
			PBYTE partialCellChar = NULL;

			if (LcdGraphOptionsToLcdGraphPartialStyle(options) == LcdGraphPartialStyle::SEMI_FILL_PARTIAL)
			{
				partialCellChar = LCD_CreateGraphSemiFillCellChar(
					(percentage % (100 / widthChars)) * widthChars, fullCellChar);
			}
			else
			{
				partialCellChar = LCD_CreateGraphPartialCellChar(
					(percentage % (100 / widthChars)) * widthChars, fullCellChar, spaceChar);
			}

			LCD->createChar(0x5, partialCellChar);

			delete partialCellChar;
		}

		delete spaceChar;
		delete fullCellChar;

		BYTE cellPercentage = 0;

		for (BYTE col = 0; col < widthChars; col++)
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

	VOID LCD_DrawSlider(BYTE row, BYTE startCol, BYTE widthChars, BYTE percentage, CLCDSLIDEROPTIONS options)
	{
		CLCDSLIDERMARKER markerStyle = LcdSliderOptionsToLcdSliderMarker(options);
		CLCDSPACETYLE spaceStyle = LcdSliderOptionsToLcdSpaceStyle(options);

		if ((CBYTE)LcdSliderOptionsToLcdSliderEnds(options))
		{
			LCD->createChar(0x5, LCD_CHAR_SLIDER_END_LEFT);

			LCD->setCursor(startCol, row);
			LCD->write(0x5);

			LCD->createChar(0x4, LCD_CHAR_SLIDER_END_RIGHT);

			LCD->setCursor(startCol + widthChars - 1, row);
			LCD->write(0x4);

			++startCol;
			widthChars -= 2;
		}

		PBYTE spaceChar = LCD_CreateSpaceChar(spaceStyle);
		LCD->createChar(0x7, spaceChar);

		PBYTE markerChar = LCD_CreateSliderMarker(markerStyle);

		PBYTE partialMarkerChar = NULL;
		CHAR cellOffset = 0;

		for (BYTE col = 0; col < widthChars; col++)
		{
			if (percentage < 100 / widthChars AND col > 0
				OR percentage >= 100 / widthChars AND percentage < col * 100 / widthChars
				OR percentage >= (col + 1) * 100 / widthChars)
			{
				LCD->setCursor(col + startCol, row);
				LCD->write(0x7);

				continue;
			}

			if(!cellOffset)
			{
				cellOffset = ((CHAR)((percentage % (100 / widthChars)) * widthChars) - 50) * (CHAR)LCD_CHAR_WIDTH / 100;
			}
			else if (cellOffset < 0)
			{
				cellOffset += (CHAR)LCD_CHAR_WIDTH / 100;
			}
			else
			{
				LCD->setCursor(col + startCol, row);
				LCD->write(0x7);

				continue;
			}

			if (!cellOffset)
			{
				PrintLine();
				for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
					markerChar[pixY] |= spaceChar[pixY];

				LCD->createChar(0x6, markerChar);

				LCD->setCursor(col + startCol, row);
				LCD->write(0x6);

				continue;
			}

			partialMarkerChar = new byte[LCD_CHAR_HEIGHT];

			if (cellOffset < 0)
			{
				for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
					partialMarkerChar[pixY] = MASK(markerChar[pixY] SHL (255 - (CBYTE)cellOffset + 1), LCD_SLIDER_MARKER_COLS_MASK);
			}
			else
			{
				for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
					partialMarkerChar[pixY] = markerChar[pixY] SHR cellOffset;
			}

			for (BYTE pixY = 0; pixY < LCD_CHAR_HEIGHT; pixY++)
				partialMarkerChar[pixY] |= spaceChar[pixY];

			LCD->createChar(0x6, partialMarkerChar);

			LCD->setCursor(col + startCol, row);
			LCD->write(0x6);

			delete partialMarkerChar;
		}

		delete spaceChar;
		delete markerChar;
	}

#pragma endregion
};
