/***********************************************************************************************
* [Foxetron_RGB.h]: RGB LED FUNCTIONS & VaRGB ARDUINO LIBRARY INTEGRATION
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

VOID RGB_callback_SetColor(ColorSettings *);
VOID RGB_callback_ScheduleComplete(Schedule *);

EXTERN VaRGB RGB;

EXTERN Schedule * _RgbSchedule;

EXTERN Curve::Flasher * _RgbCurveFlasher;
EXTERN Curve::Sine * _RgbCurveSine;
EXTERN Curve::Linear * _RgbCurves[];

#pragma endregion


#pragma region FUNCTION DECLARATIONS

VOID initializeRGB();
VOID freeRGB();

#pragma region FUNCTION DECLARATIONS


#endif
