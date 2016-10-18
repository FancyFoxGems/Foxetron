/***********************************************************************************************
* [Foxetron_pins.h]: PIN DEFINITIONS FOR THE FOXETRON ANGLE DRIVER
*
* This file is part of the Foxetron program suite
* Copyright � 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_PINS_H
#define _FOXETRON_PINS_H



#pragma region INCLUDES

// PROJECT INCLUDES
#include <Arduino.h>

#pragma endregion INCLUDES



#pragma region DEFINES

// INPUT PINS

#define PIN_ANGLE_ENCODER_A		2	// Pin 2 / PD2 (INT0)
#define PIN_ANGLE_ENCODER_B		3	// Pin 3 / PD3 (INT1)


#define PIN_BUTTON_ACTION		15	// Pin 15/A1 / PC1 (PCINT9)
#define PIN_BUTTON_ONESHOT		16	// Pin 16/A2 / PC2 (PCINT10)
#define PIN_BUTTON_LATCH		17	// Pin 17/A3 / PC3 (PCINT11)


// OUTPUT PINS

#define PIN_OUT_STATUS_LED		13	// Pin 13 / PB5

#define PIN_OUT_ACTION_LED		14	// Pin 14/A0 / PC0 (PCINT8)


#pragma endregion DEFINES



#pragma region FUNCTIONS

inline void initializePins()
{

	// REAR PINS

	// [Pins 0 (RX) & 1 (TX): USART serial communication --[angle feedback, commands, etc.]--> AVR uC stepper motor angle controller]

	// Angle encoder
	pinMode(PIN_ANGLE_ENCODER_A, INPUT);			// Channel A
	pinMode(PIN_ANGLE_ENCODER_B, INPUT);			// Channel B


	// FRONT PINS

	pinMode(PIN_OUT_STATUS_LED, OUTPUT);

	pinMode(PIN_OUT_ACTION_LED, OUTPUT);

	// Mast control inputs
	pinMode(PIN_BUTTON_ACTION, INPUT_PULLUP);
	pinMode(PIN_BUTTON_ONESHOT, INPUT_PULLUP);
	pinMode(PIN_BUTTON_LATCH, INPUT_PULLUP);
}

#pragma endregion FUNCTIONS


#endif