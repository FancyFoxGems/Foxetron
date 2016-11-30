/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/

// GCC WARNING SUPPRESSIONS
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"


#include "Foxetron_AngleController_LCD.h"


#pragma region GLOBAL VARIABLE DEFINITIONS

// LCD display
PLCDI2C<LCD_COLS, LCD_ROWS> LCD = NULL;	// Pin A4/A5 (I2C)

#pragma endregion


#pragma region PROGRAM FUNCTION DEFINITIONS

VOID LCD_Initialize()
{
	LCD = new LCD_I2C<LCD_COLS, LCD_ROWS>(LCD_WRAP_LINES);

	LCD->Init();

	//LCD->LoadBigFont();
}

VOID LCD_Free()
{
	delete LCD;
	LCD = NULL;
}

#pragma endregion
