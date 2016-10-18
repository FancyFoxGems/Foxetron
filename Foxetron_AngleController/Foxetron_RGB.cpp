/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_RGB.h"


#pragma region GLOBAL VARIABLES

// RGB LED / VaRGB CONFIGURATION

VaRGB RGB(RGB_callback_SetColor, RGB_callback_ScheduleComplete);

Schedule * _RgbSchedule = new Schedule();

Curve::Flasher * _RgbCurveFlasher = new Curve::Flasher(VaRGB_COLOR_MAXVALUE, VaRGB_COLOR_MAXVALUE, VaRGB_COLOR_MAXVALUE, 6, 20);

Curve::Sine * _RgbCurveSine = new Curve::Sine(500, VaRGB_COLOR_MAXVALUE, 500, 6, 2);

Curve::Linear * _RgbCurves[] =
{
	// start black
	new Curve::Linear(0, 0, 0, 0),
	// go to ~1/2 red, over one 5 seconds
	new Curve::Linear(500, 0, 0, 5),
	// go to red+blue, over 2s
	new Curve::Linear(1000, 0, 1000, 2),
	// back down to ~1/2 red over 2s
	new Curve::Linear(500, 0, 0, 2),
	// fade to black for 5s
	new Curve::Linear(0, 0, 0, 5),
};

#pragma endregion GLOBAL VARIABLES



#pragma region FUNCTION DEFINITIONS

// VaRGB CALLBACK FUNCTIONS

VOID RGB_callback_SetColor(ColorSettings * colors)
{
	analogWrite(PIN_PWM_RGB_LED_RED, colors->red);
	analogWrite(PIN_PWM_RGB_LED_GREEN, colors->green);
	analogWrite(PIN_PWM_RGB_LED_BLUE, colors->blue);
}

VOID RGB_callback_ScheduleComplete(Schedule * schedule)
{
	RGB.resetTicks();
	RGB.setSchedule(schedule);
}


// PROGRAM FUNCTIONS

VOID initializeRGB()
{
	_RgbSchedule->addTransition(_RgbCurveFlasher);
	_RgbSchedule->addTransition(_RgbCurveSine);

	for (uint8_t i = 0; i < 5; i++)
		_RgbSchedule->addTransition(_RgbCurves[i]);

	RGB.setSchedule(_RgbSchedule);
}

VOID freeRGB()
{
	delete _RgbSchedule;

	delete _RgbCurveFlasher;
	delete _RgbCurveSine;
	
	for (BYTE i = 0; i < SIZEOF(_RgbCurves) / SIZEOF(_RgbCurves[0]); i++)
	{
		if (_RgbCurves[i] != NULL)
			delete _RgbCurves[i];
	}

	delete[] _RgbCurves;
}

#pragma endregion FUNCTION DEFINITIONS
