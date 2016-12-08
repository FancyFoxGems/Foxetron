/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"


#include "Foxetron_AngleController_RGB.h"


#pragma region GLOBAL VARIABLE DEFINITIONS

// RGB LED / VaRGB CONFIGURATION

VaRGB * RGB = NULL;

Schedule * _RgbSchedule = new Schedule();

Flasher * _RgbCurveFlasher = new Flasher(VaRGB_COLOR_MAXVALUE, VaRGB_COLOR_MAXVALUE, VaRGB_COLOR_MAXVALUE, 6, 20);

Sine * _RgbCurveSine = new Sine(500, VaRGB_COLOR_MAXVALUE, 500, 6, 2);

Linear * _RgbCurves[] =
{
	// start black
	new Linear(0, 0, 0, 0),
	// go to ~1/2 red, over one 5 seconds
	new Linear(500, 0, 0, 5),
	// go to red+blue, over 2s
	new Linear(1000, 0, 1000, 2),
	// back down to ~1/2 red over 2s
	new Linear(500, 0, 0, 2),
	// fade to black for 5s
	new Linear(0, 0, 0, 5),
};

#pragma endregion


#pragma region [VaRGB] CALLBACK FUNCTIONS

VOID RGB_OnSetColor(ColorSettings * colors)
{
	analogWrite(PIN_PWM_RGB_LED_RED, colors->red);
	analogWrite(PIN_PWM_RGB_LED_GREEN, colors->green);
	analogWrite(PIN_PWM_RGB_LED_BLUE, colors->blue);
}

VOID RGB_OnScheduleComplete(Schedule * schedule)
{
	RGB->resetTicks();
	RGB->setSchedule(schedule);
}

#pragma endregion


#pragma region PROGRAM FUNCTION DEFINITIONS

VOID RGB_Initialize()
{
	_RgbSchedule->addTransition(_RgbCurveFlasher);
	_RgbSchedule->addTransition(_RgbCurveSine);

	for (uint8_t i = 0; i < 5; i++)
		_RgbSchedule->addTransition(_RgbCurves[i]);

	RGB = new VaRGB(RGB_OnSetColor, RGB_OnScheduleComplete);

	RGB->setSchedule(_RgbSchedule);
}

VOID RGB_Free()
{
	delete _RgbSchedule;
	_RgbSchedule = NULL;

	delete _RgbCurveFlasher;
	_RgbCurveFlasher = NULL;

	delete _RgbCurveSine;
	_RgbCurveSine = NULL;

	for (BYTE i = 0; i < COUNTOF(_RgbCurves); i++)
	{
		if (_RgbCurves[i])
			delete _RgbCurves[i];
	}

	delete RGB;
	RGB = NULL;
}

VOID RGB_Step()
{
	RGB->tickAndDelay();
}

#pragma endregion
