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

// 3RD-PARTY LIBS
#include "VaRGB.h"
#include "VaRGBCurves.h"

#pragma endregion INCLUDES



#pragma region DEFINES

// OUTPUT PINS

#define PIN_PWM_RGB_LED_RED		9	// Pin 9 / PB1
#define PIN_PWM_RGB_LED_GREEN	10	// Pin 10 / PB2
#define PIN_PWM_RGB_LED_BLUE	11	// Pin 11 / PB3

#pragma endregion DEFINES



#pragma region GLOBAL VARIABLES

// RGB LED / VaRGB CONFIGURATION

using namespace vargb;

void RGB_callback_SetColor(ColorSettings *);
void RGB_callback_ScheduleComplete(Schedule *);

extern VaRGB RGB;

extern Schedule * _RgbSchedule;

extern Curve::Flasher * _RgbCurveFlasher;
extern Curve::Sine * _RgbCurveSine;
extern Curve::Linear * _RgbCurves[];

#pragma endregion GLOBAL VARIABLES



#pragma region FUNCTION DECLARATIONS

void initializeRGB();

#pragma region FUNCTION DECLARATIONS


#endif
