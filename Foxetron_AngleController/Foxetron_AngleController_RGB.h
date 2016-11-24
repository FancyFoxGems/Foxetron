/***********************************************************************************************
* [Foxetron_AngleController_RGB.h]: RGB LED FUNCTIONS & VaRGB ARDUINO LIBRARY INTEGRATION
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_RGB_H
#define _FOXETRON_RGB_H



#pragma region INCLUDES

// ITTY BITTY
#include "IttyBitty_util.h"

// 3RD-PARTY LIBS
#include "VaRGB.h"
#include "VaRGBCurves.h"

#pragma endregion


#pragma region DEFINES

// OUTPUT PINS

#define PIN_PWM_RGB_LED_RED		9	// Pin 9 / PB1
#define PIN_PWM_RGB_LED_GREEN	10	// Pin 10 / PB2
#define PIN_PWM_RGB_LED_BLUE	11	// Pin 11 / PB3

#pragma endregion


#pragma region GLOBAL VARIABLE DECLARATIONS

using namespace vargb;
using namespace vargb::Curve;

VOID RGB_OnSetColor(ColorSettings *);
VOID RGB_OnScheduleComplete(Schedule *);

EXTERN VaRGB * RGB;

EXTERN Schedule * _RgbSchedule;

EXTERN Flasher * _RgbCurveFlasher;
EXTERN Sine * _RgbCurveSine;
EXTERN Linear * _RgbCurves[];

#pragma endregion


#pragma region PROGRAM FUNCTION DECLARATIONS

VOID RGB_Initialize();
VOID RGB_Free();

VOID RGB_Step();

#pragma endregion


#endif
