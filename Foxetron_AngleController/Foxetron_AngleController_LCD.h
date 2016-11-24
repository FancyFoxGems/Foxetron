/***********************************************************************************************
* [Foxetron_AngleController_LCD.h]: HD44780 LCD DISPLAY MODULE VIA I2C; LARGE FONT SUPPORT
*
* This file is part of the Foxetron program suite
* Copyright � 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#pragma GCC diagnostic ignored "-Wparentheses"

#ifndef _FOXETRON_LCD_H
#define _FOXETRON_LCD_H


#pragma region INCLUDES

// ITTY BITTY
#include "IttyBitty_util.h"

// PROJECT INCLUDES
#include "Foxetron_AngleController_LCD_chars.h"
#include "Foxetron_AngleController_pins.h"

// PROJECT LIBS
#include "libs/BigCrystal_I2C.custom.h"

// AVR LibC
//#include <avr/pgmspace.h>						// included by project 3rd-party libs

#pragma endregion


#pragma region DEFINES

// LCD OPTIONS/CONSTANTS

//#define LCD_I2C_ADDRESS			0x27
#define LCD_I2C_ADDRESS			(0x7C >> 1)

#define LCD_COLS				16
#define LCD_ROWS				2

#pragma endregion


#pragma region LCD GLOBAL VARIABLE DECLARATION

// LCD display
EXTERN BigCrystal_I2C * LCD;

#pragma endregion


#pragma region PROGRAM FUNCTION DECLARATIONS

VOID LCD_Initialize();
VOID LCD_Free();

#pragma endregion


namespace Foxetron
{
#pragma region LCD SCROLLBAR ENUMS

	enum LcdScrollBarIndicator : BYTE
	{
		LINE_INDICATOR = 0x00,
		BLOCK_INDICATOR = 0x10
	};

	TYPEDEF_ENUM_ALIASES(LcdScrollBarIndicator, LCDSCROLLBARINDICATOR);


	enum LcdScrollBarPosition : BYTE
	{
		RIGHT_POSITION = 0x0,
		LEFT_POSITION = 0x1
	};

	TYPEDEF_ENUM_ALIASES(LcdScrollBarPosition, LCDSCROLLBARPOSITION);


	ENUM LcdScrollBarOptions : BYTE
	{
		LINE = LINE_INDICATOR OR RIGHT_POSITION,
		LINE_LEFT = LINE_INDICATOR OR LEFT_POSITION,

		BLOCK = BLOCK_INDICATOR OR RIGHT_POSITION,
		BLOCK_LEFT = BLOCK_INDICATOR OR LEFT_POSITION
	};

	TYPEDEF_ENUM_ALIASES(LcdScrollBarOptions, LCDSCROLLBAROPTIONS);



	STATIC CLCDSCROLLBARINDICATOR LcdScrollBarOptionsToLcdScrollBarIndicator(CLCDSCROLLBAROPTIONS lcdScrollBarOptions)
	{
		return static_cast<CLCDSCROLLBARINDICATOR>(HIGH_NYBBLE((CBYTE)lcdScrollBarOptions));
	}

	STATIC CLCDSCROLLBARPOSITION LcdScrollBarOptionsToLcdScrollBarPosition(CLCDSCROLLBAROPTIONS lcdScrollBarOptions)
	{
		return static_cast<CLCDSCROLLBARPOSITION>(LOW_NYBBLE((CBYTE)lcdScrollBarOptions));
	}

#pragma endregion


#pragma region LCD GRAPH ENUMS

	enum LcdGraphCellStyle : BYTE
	{
		BAR_GRAPH		= 0x00,
		LINE_GRAPH		= 0x10,
		SQUARE_GRAPH	= 0x20,
		DISC_GRAPH		= 0x30
	};

	TYPEDEF_ENUM_ALIASES(LcdGraphCellStyle, LCDGRAPHCELLSTYLE);


	enum LcdGraphPartialStyle : BYTE
	{
		NORMAL_PARTIAL	= 0x00,
		SEMI_PARTIAL	= 0x80
	};

	TYPEDEF_ENUM_ALIASES(LcdGraphPartialStyle, LCDGRAPHPARTIALSTYLE);


	enum LcdGraphSpaces : BYTE
	{
		EMPTY_SPACE = 0x0,
		DASH_SPACE = 0x1,
		PLUS_SPACE = 0x2,
		DOTS_SPACE = 0x3,
		CIRCLE_SPACE = 0x4,
		SQUARE_SPACE = 0x5,
	};

	TYPEDEF_ENUM_ALIASES(LcdGraphSpaces, LCDGRAPHSPACES);


	ENUM LcdGraphOptions : BYTE
	{
		BAR					= BAR_GRAPH OR NORMAL_PARTIAL OR EMPTY_SPACE,
		BAR_DASH			= BAR_GRAPH OR NORMAL_PARTIAL OR DASH_SPACE,
		BAR_DOTS			= BAR_GRAPH OR NORMAL_PARTIAL OR DOTS_SPACE,
		BAR_DOT				= BAR_GRAPH OR NORMAL_PARTIAL OR PLUS_SPACE,
		BAR_CIRCLE			= BAR_GRAPH OR NORMAL_PARTIAL OR CIRCLE_SPACE,
		BAR_SQUARE			= BAR_GRAPH OR NORMAL_PARTIAL OR SQUARE_SPACE,

		BAR_SEMI			= BAR_GRAPH OR SEMI_PARTIAL OR EMPTY_SPACE,
		BAR_SEMI_DASH		= BAR_GRAPH OR SEMI_PARTIAL OR DASH_SPACE,
		BAR_SEMI_DOTS		= BAR_GRAPH OR SEMI_PARTIAL OR DOTS_SPACE,
		BAR_SEMI_DOT		= BAR_GRAPH OR SEMI_PARTIAL OR PLUS_SPACE,
		BAR_SEMI_CIRCLE		= BAR_GRAPH OR SEMI_PARTIAL OR CIRCLE_SPACE,
		BAR_SEMI_SQUARE		= BAR_GRAPH OR SEMI_PARTIAL OR SQUARE_SPACE,

		LINE				= LINE_GRAPH OR NORMAL_PARTIAL OR EMPTY_SPACE,
		LINE_DASH			= LINE_GRAPH OR NORMAL_PARTIAL OR DASH_SPACE,
		LINE_DOTS			= LINE_GRAPH OR NORMAL_PARTIAL OR DOTS_SPACE,
		LINE_DOT			= LINE_GRAPH OR NORMAL_PARTIAL OR PLUS_SPACE,
		LINE_CIRCLE			= LINE_GRAPH OR NORMAL_PARTIAL OR CIRCLE_SPACE,
		LINE_SQUARE			= LINE_GRAPH OR NORMAL_PARTIAL OR SQUARE_SPACE,

		LINE_SEMI			= LINE_GRAPH OR SEMI_PARTIAL OR EMPTY_SPACE,
		LINE_SEMI_DASH		= LINE_GRAPH OR SEMI_PARTIAL OR DASH_SPACE,
		LINE_SEMI_DOTS		= LINE_GRAPH OR SEMI_PARTIAL OR DOTS_SPACE,
		LINE_SEMI_DOT		= LINE_GRAPH OR SEMI_PARTIAL OR PLUS_SPACE,
		LINE_SEMI_CIRCLE	= LINE_GRAPH OR SEMI_PARTIAL OR CIRCLE_SPACE,
		LINE_SEMI_SQUARE	= LINE_GRAPH OR SEMI_PARTIAL OR SQUARE_SPACE,

		SQUARE				= SQUARE_GRAPH OR NORMAL_PARTIAL OR EMPTY_SPACE,
		SQUARE_DASH			= SQUARE_GRAPH OR NORMAL_PARTIAL OR DASH_SPACE,
		SQUARE_DOTS			= SQUARE_GRAPH OR NORMAL_PARTIAL OR DOTS_SPACE,
		SQUARE_DOT			= SQUARE_GRAPH OR NORMAL_PARTIAL OR PLUS_SPACE,
		SQUARE_CIRCLE		= SQUARE_GRAPH OR NORMAL_PARTIAL OR CIRCLE_SPACE,
		SQUARE_SQUARE		= SQUARE_GRAPH OR NORMAL_PARTIAL OR SQUARE_SPACE,

		SQUARE_SEMI			= SQUARE_GRAPH OR SEMI_PARTIAL OR EMPTY_SPACE,
		SQUARE_SEMI_DASH	= SQUARE_GRAPH OR SEMI_PARTIAL OR DASH_SPACE,
		SQUARE_SEMI_DOTS	= SQUARE_GRAPH OR SEMI_PARTIAL OR DOTS_SPACE,
		SQUARE_SEMI_DOT		= SQUARE_GRAPH OR SEMI_PARTIAL OR PLUS_SPACE,
		SQUARE_SEMI_CIRCLE	= SQUARE_GRAPH OR SEMI_PARTIAL OR CIRCLE_SPACE,
		SQUARE_SEMI_SQUARE	= SQUARE_GRAPH OR SEMI_PARTIAL OR SQUARE_SPACE,

		DISC				= DISC_GRAPH OR NORMAL_PARTIAL OR EMPTY_SPACE,
		DISC_DASH			= DISC_GRAPH OR NORMAL_PARTIAL OR DASH_SPACE,
		DISC_DOTS			= DISC_GRAPH OR NORMAL_PARTIAL OR DOTS_SPACE,
		DISC_DOT			= DISC_GRAPH OR NORMAL_PARTIAL OR PLUS_SPACE,
		DISC_CIRCLE			= DISC_GRAPH OR NORMAL_PARTIAL OR CIRCLE_SPACE,
		DISC_SQUARE			= DISC_GRAPH OR NORMAL_PARTIAL OR SQUARE_SPACE,

		DISC_SEMI			= DISC_GRAPH OR SEMI_PARTIAL OR EMPTY_SPACE,
		DISC_SEMI_DASH		= DISC_GRAPH OR SEMI_PARTIAL OR DASH_SPACE,
		DISC_SEMI_DOTS		= DISC_GRAPH OR SEMI_PARTIAL OR DOTS_SPACE,
		DISC_SEMI_DOT		= DISC_GRAPH OR SEMI_PARTIAL OR PLUS_SPACE,
		DISC_SEMI_CIRCLE	= DISC_GRAPH OR SEMI_PARTIAL OR CIRCLE_SPACE,
		DISC_SEMI_SQUARE	= DISC_GRAPH OR SEMI_PARTIAL OR SQUARE_SPACE
	};

	TYPEDEF_ENUM_ALIASES(LcdGraphOptions, LCDGRAPHOPTIONS);


	STATIC CLCDGRAPHCELLSTYLE LcdGraphOptionsToLcdGraphCellStyle(CLCDGRAPHOPTIONS lcdGraphOptions)
	{
		return static_cast<CLCDGRAPHCELLSTYLE>(HIGH_NYBBLE((CBYTE)lcdGraphOptions));
	}

	#define LCD_GRAPH_PARTIAL_STYLE_MASK	0x80

	STATIC CLCDGRAPHPARTIALSTYLE LcdGraphOptionsToLcdGraphPartialStyle(CLCDGRAPHOPTIONS lcdGraphOptions)
	{
		return static_cast<CLCDGRAPHPARTIALSTYLE>(MASK((CBYTE)lcdGraphOptions, LCD_GRAPH_PARTIAL_STYLE_MASK));
	}

	STATIC CLCDGRAPHSPACES LcdGraphOptionsToLcdGraphSpaces(CLCDGRAPHOPTIONS lcdGraphOptions)
	{
		return static_cast<CLCDGRAPHSPACES>(LOW_NYBBLE((CBYTE)lcdGraphOptions));
	}

#pragma endregion


#pragma region LCD FUNCTION DECLARATIONS

	PCCHAR LCD_CreateInvertedChar(PCCHAR);
	PCCHAR LCD_CreateInvertedChar_P(PCCHAR);

	VOID LCD_LoadInvertedChar(BYTE, PCCHAR);
	VOID LCD_LoadInvertedChar_P(BYTE, PCCHAR);

	VOID LCD_DrawScrollBar(BYTE, CLCDSCROLLBAROPTIONS = LcdScrollBarOptions::LINE);
	VOID LCD_DrawGraph(BYTE, BYTE, BYTE, BYTE, CLCDGRAPHOPTIONS = LcdGraphOptions::BAR);

#pragma endregion
};

#endif
