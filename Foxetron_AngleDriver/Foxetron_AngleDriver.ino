/*****************************************************************************************************
* Name:		Foxetron Angle Driver (Firmware Code; Part of the Foxetron Program Suite)
* Created:	10/2/2016
* Author:	Tom Biuso <tjbtech@yahoo.com>
*
* This file is part of the Foxetron program suite.
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
*
* The Foxetron suite is free software: you can redistribute it and/or modify it under the terms of the
* GNU General Public License as published by the Free Software Foundation, either version
* 3 of the License, or (at your option) any later version.
*
* The Foxetron suite is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* See the included GNU General Public License text for more details.
*****************************************************************************************************/


#pragma GCC diagnostic ignored "-Wunknown-pragmas"


#pragma region INCLUDES

// PROJECT INCLUDES
//#include "Foxetron_pins.h"


// PROJECT MODULES

// PROJECT LIBS

// 3RD-PARTY LIBS
#include "HalfStepper.h"					// well...1st-party in this case, really.

// ARDUINO LIBS

// ARDUINO CORE
//#include <Arduino.h>							// included by project/3rd-party libs

// AVR LibC
//#include <avr/pgmspace.h>						// included by project 3rd-party libs

#pragma endregion INCLUDES



#pragma region DEFINES

// PROGRAM OPTIONS

#define DEBUG_INPUTS			0

#define SERIAL_BAUD_RATE		115200

#if defined(DEBUG_INPUTS) && DEBUG_INPUTS != 1
	#undef DEBUG_INPUTS
#endif

#pragma endregion DEFINES




#pragma region PROGRAM CONSTANTS

#pragma endregion PROGRAM CONSTANTS



#pragma region PROGRAM VARIABLES

// INPUTS

// Angle encoder
volatile bool _AngleEncoderA	= false;	// Pin 2 / PD2 (INT0)
volatile bool _AngleEncoderB	= false;	// Pin 3 / PD3 (INT1)

volatile bool _AngleUp			= false;
volatile uint32_t _AngleReading	= 0;
word _AngleDelta				= 0;
word _AngleVelocity				= 0;

// Mast control inputs
volatile bool _ActionButton		= false;	// Pin 15/A1 / PC1 (PCINT9)
volatile bool _OneShotButton	= false;	// Pin 16/A2 / PC2 (PCINT10)
volatile bool _LatchButton		= false;	// Pin 17/A3 / PC3 (PCINT11)


// OUTPUTS

// LEDs
volatile bool _StatusLed		= LOW;
volatile bool _ActionLed		= HIGH;

#pragma endregion PROGRAM VARIABLES



#pragma region PROGRAM OUTLINE: ENTRY POINT & LOOP

void setup()
{
	Serial.begin(SERIAL_BAUD_RATE);

	initializePins();
	initializeInterrupts();
}

void loop()
{
#ifdef DEBUG_INPUTS
	_DEBUG_printInputValues();
#endif
}

#pragma endregion PROGRAM OUTLINE: ENTRY POINT & LOOP



#pragma region INTERRUPT VECTORS and SUPPORTING MACROS & INLINED FUNCTIONS

// INT0/INT1: ANGLE ENCODER

#define _ISR_ANGLE_ENCODER_READ_CHANNEL(channel, other_channel, increment_comparison)					\
	_AngleEncoder ## channel = !_AngleEncoder ## channel;												\
	_AngleUp = (_AngleEncoder ## channel increment_comparison _AngleEncoder ## other_channel);			\
	_ISR_AngleEncoder_updateAngleReading();

static inline void _ISR_AngleEncoder_updateAngleReading() __attribute__((always_inline));

static inline void _ISR_AngleEncoder_updateAngleReading()
{
	if (_AngleUp)
		++_AngleReading;
	else
		--_AngleReading;

#ifdef DEBUG_INPUTS
	Serial.print(F("ANGLE: "));
	Serial.println(_AngleReading);
#endif
}

// CHANNEL A
ISR(INT0_vect)
{
	_ISR_ANGLE_ENCODER_READ_CHANNEL(A, B, ==);
}

// CHANNEL B
ISR(INT1_vect)
{
	_ISR_ANGLE_ENCODER_READ_CHANNEL(B, A, != );
}


// TIMER EVENTS

// TIMER 2 OVERFLOW
ISR(TIMER2_OVF_vect, ISR_NOBLOCK)
{

}


// SERIAL EVENTS

// USART RECEIVE
/*ISR(USART_RX_vect, ISR_NOBLOCK)
{

}*/

#pragma endregion INTERRUPT VECTORS and SUPPORTING MACROS & INLINED FUNCTIONS



#pragma region PROGRAM FUNCTIONS

void initializeInterrupts()
{
	// External interrupts: Angle encoder
	EIMSK |= 0b00000011;
	EICRA &= 0b11110101;
	EICRA |= 0b00000101;
}

#pragma endregion PROGRAM FUNCTIONS



#pragma region DEBUG UTILITY FUNCTIONS

void _DEBUG_printInputValues()
{
	static char valStr[2];

	_AngleEncoderA	= digitalRead(PIN_ANGLE_ENCODER_A);
	_AngleEncoderB	= digitalRead(PIN_ANGLE_ENCODER_B);

	_ActionButton = !digitalRead(PIN_BUTTON_ACTION);
	_OneShotButton	= !digitalRead(PIN_BUTTON_ONESHOT);
	_LatchButton = !digitalRead(PIN_BUTTON_LATCH);


	// REAR INPUTS

	Serial.print(itoa(_AngleEncoderA, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_AngleEncoderB, valStr, 2));
	Serial.println();


	// FRONT INPUTS

	Serial.print(itoa(_LatchButton, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_OneShotButton, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_ActionButton, valStr, 2));
	Serial.println();

	Serial.println();

	_ActionLed = !_ActionLed;
	digitalWrite(PIN_OUT_ACTION_LED, _ActionLed);

	_StatusLed = !_StatusLed;
	digitalWrite(PIN_OUT_STATUS_LED, _StatusLed);

	delay(500);
}

#pragma endregion DEBUG UTILITY FUNCTIONS
