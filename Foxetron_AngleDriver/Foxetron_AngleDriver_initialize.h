/***********************************************************************************************
* [Foxetron_AngleDriver_pins.h]: PIN DEFINITIONS FOR THE FOXETRON ANGLE DRIVER
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_PINS_H
#define _FOXETRON_PINS_H


#pragma region INCLUDES

// PROJECT INCLUDES
#include "Foxetron_common.h"

#pragma endregion


#pragma region DEFINES

// INPUT PINS

#define PIN_ANGLE_ENCODER_A		2	// Pin 2 / PD2 (INT0)
#define PIN_ANGLE_ENCODER_B		3	// Pin 3 / PD3 (INT1)


#define PIN_BUTTON_ACTION		15	// Pin 15/A1 / PC1 (PCINT9)
#define PIN_BUTTON_ONESHOT		16	// Pin 16/A2 / PC2 (PCINT10)
#define PIN_BUTTON_LATCH		17	// Pin 17/A3 / PC3 (PCINT11)


// OUTPUT PINS

#define PIN_OUT_MOTOR_1A		4	// Pin 4 / PD4
#define PIN_OUT_MOTOR_1B		5	// Pin 5 / PD5
#define PIN_OUT_MOTOR_2A		6	// Pin 6 / PD6
#define PIN_OUT_MOTOR_2B		7	// Pin 7 / PD7

#define PIN_OUT_STATUS_LED		13	// Pin 13 / PB5

#define PIN_OUT_ACTION_LED		14	// Pin 14/A0 / PC0 (PCINT8)

#pragma endregion


#pragma region PROGRAM FUNCTIONS

INLINE VOID InitializePins()
{
	// REAR PINS

	// [Pins 0 (RX) & 1 (TX): USART serial communication --[angle feedback, commands, etc.]--> AVR uC stepper motor angle controller]

	// Angle encoder
	SetPinMode(PIN_ANGLE_ENCODER_A, INPUT);			// Channel A
	SetPinMode(PIN_ANGLE_ENCODER_B, INPUT);			// Channel B

	// Stepper motor
	SetPinMode(PIN_OUT_MOTOR_1A, OUTPUT);
	SetPinMode(PIN_OUT_MOTOR_1B, OUTPUT);
	SetPinMode(PIN_OUT_MOTOR_2A, OUTPUT);
	SetPinMode(PIN_OUT_MOTOR_2B, OUTPUT);


	// FRONT PINS

	// LEDs
	SetPinMode(PIN_OUT_STATUS_LED, OUTPUT);
	SetPinMode(PIN_OUT_ACTION_LED, OUTPUT);

	// Mast control inputs
	SetPinMode(PIN_BUTTON_ACTION, INPUT_PULLUP);
	SetPinMode(PIN_BUTTON_ONESHOT, INPUT_PULLUP);
	SetPinMode(PIN_BUTTON_LATCH, INPUT_PULLUP);
}

INLINE VOID InitializeTimers(CDWORD inputProcessInterval_uS, CDWORD processTimerOverflow_uS)
{
	// Timer 2: Angle adjustment task; CTC mode
	RESET_SFR(TCCR2A);
	RESET_SFR(TCCR2B);
	RESET_SFR(TIMSK2);
	SET_SFR_BITS(TCCR2B, WITH_BIT(PROCESS_TIMER_PRESCALER_FLAGS, B(WGM21)));
	OCR2A = (CBYTE)(inputProcessInterval_uS / processTimerOverflow_uS);
	OCR2B = (CBYTE)(inputProcessInterval_uS / processTimerOverflow_uS);
	SET_SFR_BITS(TIMSK2, B(OCIE2A) | B(OCIE2B));
}

INLINE VOID InitializeInterrupts()
{
	// External interrupts: Angle encoder
	RESET_SFR(EICRA);
	RESET_SFR(EIMSK);
	SET_SFR_BITS(EICRA, B(ISC10) | B(ISC00));
	SET_SFR_BITS(EIMSK, B(INT1) | B(INT0));
}

#pragma endregion


#endif
