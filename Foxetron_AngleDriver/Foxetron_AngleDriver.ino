/*****************************************************************************************************
* Name:		Foxetron Angle Driver (Firmware Code; Part of the Foxetron Program Suite)
* Created:	10/2/2016
* Author:	Tom Biuso <tjbtech@yahoo.com>
*
* Target Architecture:	Atmel AVR / ATmega series 8-bit MCUs
* Supported Platforms:	Arduino, AVR LibC, (AVR GCC)
*
*		Memory Usage:	~19.15 KB Program Memory (Flash ROM) / 1565 B SRAM
*		NOTE: ^-- w/ #define NO_ITTYBITTY_FULL_BYTES
*
* [Hardware Platform]
*	MCU:			Arduino Nano w/ Atmel ATmega 328P
*	Motor Driver:		Dual L9110s-based chip module
*	Stepper Motor:	2-phase bipolar, NEMA 17, 1.8° steps, 50 oz. @ 2.8 V / 1.68 A
*	Other Components:	1 × Blue LED 5mm
*	Hardware:		Aluminum enclosure; GT2 36T pulley; GT2 belt, ~10 in.;
*						headers, connectors, wires, mounts, etc.
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


// GCC WARNING SUPPRESSIONS

#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wconversion-null"
#pragma GCC diagnostic ignored "-Wchar-subscripts"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wsequence-point"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#pragma GCC diagnostic ignored "-Wpointer-arith"
#pragma GCC diagnostic ignored "-Wvirtual-move-assign"


#pragma region INCLUDES

// PROJECT INCLUDES
#include "Foxetron_pins.h"

// PROJECT MODULES
#include "Foxetron_messages.h"

using namespace Foxetron;

// PROJECT LIBS

// ITTY BITTY
#include "IttyBitty.h"

using namespace IttyBitty;

// 3RD-PARTY LIBS
#include "HalfStepper.h"				// Well...1st-party in this case, really.

// ARDUINO LIBS

// ARDUINO CORE
//#include "Arduino.h"						// Included by project/3rd-party libs

// AVR LibC
//#include <avr/pgmspace.h>					// Included by project 3rd-party libs

#pragma endregion


#pragma region DEFINES

// PROGRAM OPTIONS

#define DEBUG_MEMORY_INFO_INTERVAL_MS			3000			// Period by which available RAM should be printed when debugging

#define DEBUG_INPUTS							0				// Whether to print values of pin input signals
#define DEBUG_INPUTS_INTERVAL_MS				500				// Period by which input signal values should be printed when debugging

#if defined(DEBUG_INPUTS) && DEBUG_INPUTS != 1
	#undef DEBUG_INPUTS
#endif

#define SERIAL_BAUD_RATE						115200			// (Debugging) UART baud rate
#define SERIAL_DELAY_MS							1				// Delay for waiting on serial buffer to flush when printing debug statements

#define ANGLE_DEGREE_PRECISION_FACTOR			100				// Scaling factor of °s for angle measurement & adjustment calculations
#define ANGLE_ACTUAL_DEGRESS_PER_DEGREE_VALUE	ANGLE_DEGREE_PRECISION_FACTOR

#define ANGLE_ERROR_CORRECTION_FACTOR			0				// (Additional) correction to apply to the motor's error margin for angle ajustments


// PROGRAM CONSTANTS

#define ANGLE_BASE_PRECISION_FACTOR					ANGLE_DEGREE_PRECISION_FACTOR
		// ^-- Scaling factor of °s and steps used in derived scaling factors for angle measurement & adjustment calculations

#define ANGLE_STEP_PRECISION_FACTOR_FACTOR			10
#define ANGLE_POINT_STEP_PRECISION_FACTOR_FACTOR	100

#define ANGLE_ENCODER_STEPS_RESOLUTION				8192		// 2,048 steps(/360°)/channel × 2 channels × 2 (quadrature signal encoding)
#define ANGLE_MOTOR_STEPS_RESOLUTION				144000		// 200 steps(/revolution) × 2 (half-stepping mode) × 2 (dual phasing) × 180 (revolutions/360°)
																		// NOTE: Foxetron angle adjustment factor: 2°/revolution --^

#pragma endregion


#pragma region PROGRAM CONSTANTS

CDWORD ANGLE_STEP_PRECISION_FACTOR			= (CDWORD)ANGLE_BASE_PRECISION_FACTOR * (CDWORD)ANGLE_STEP_PRECISION_FACTOR_FACTOR;			// 1,000
CDWORD ANGLE_POINT_STEP_PRECISION_FACTOR	= (CDWORD)ANGLE_POINT_STEP_PRECISION_FACTOR_FACTOR * (CDWORD)ANGLE_STEP_PRECISION_FACTOR;	// 100,000

CDWORD ANGLE_POINTS_PER_ACTUAL_DEGREE		= (CDWORD)ANGLE_POINT_STEP_PRECISION_FACTOR;												// 100,000
CDWORD ANGLE_POINTS_PER_DEGREE_VALUE		= (CDWORD)ANGLE_POINT_STEP_PRECISION_FACTOR / (CDWORD)ANGLE_DEGREE_PRECISION_FACTOR;		// 1,000

CDWORD ANGLE_ENCODER_STEPS_PER_POINT		= (CDWORD)ANGLE_ENCODER_STEPS_RESOLUTION * ANGLE_POINT_STEP_PRECISION_FACTOR/(CDWORD)360;	// > 22,755 (22.75 steps/°)
CDWORD ANGLE_ENCODER_STEP_ERROR_MARGIN		= ANGLE_ENCODER_STEPS_PER_POINT / 2 + (CDWORD)ANGLE_ERROR_CORRECTION_FACTOR;				// +/- > 11,377 (11.37 steps)
CDWORD ANGLE_ENCODER_POINTS_PER_STEP		= (CDWORD)360 * ANGLE_POINT_STEP_PRECISION_FACTOR/(CDWORD)ANGLE_ENCODER_STEPS_RESOLUTION;	// > 4,394 points (0.04394°/step)
CDWORD ANGLE_ENCODER_POINT_ERROR_MARGIN		= ANGLE_ENCODER_POINTS_PER_STEP / 2 + (CDWORD)ANGLE_ERROR_CORRECTION_FACTOR;				// +/- > 2,197 points  (0.02197°)

CDWORD ANGLE_MOTOR_STEPS_PER_POINT			= (CDWORD)ANGLE_MOTOR_STEPS_RESOLUTION * ANGLE_POINT_STEP_PRECISION_FACTOR/(CDWORD)360;		// 400,000 (400.00 steps/°)
CDWORD ANGLE_MOTOR_STEP_ERROR_MARGIN		= ANGLE_MOTOR_STEPS_PER_POINT / 2;															// +/- 200,000 (200.00 steps)
CDWORD ANGLE_MOTOR_POINTS_PER_STEP			= (CDWORD)360 * ANGLE_POINT_STEP_PRECISION_FACTOR/(CDWORD)ANGLE_MOTOR_STEPS_RESOLUTION;		// 250 points (0.00250°/step)
CDWORD ANGLE_MOTOR_POINT_ERROR_MARGIN		= ANGLE_MOTOR_POINTS_PER_STEP / 2;															// +/- 125 points (0.00125°)

#pragma endregion


#pragma region PROGRAM VARIABLES

// INPUTS

// Angle encoder
VBOOL _AngleEncoderA		= FALSE;	// Pin 2 / PD2 (INT0)
VBOOL _AngleEncoderB		= FALSE;	// Pin 3 / PD3 (INT1)
//VBOOL _AngleEncoderZ		= 0;	// Pin 4 / PD4 (PCINT20)	- [UNUSED]
//VBOOL _AngleEncoderU		= 0;	// Pin 5 / PD5 (PCINT21)	- [UNUSED]

VBOOL _AngleEncoderUp		= FALSE;
VDWORD _AngleEncoderSteps	= 0;
WORD _AngleEncoderDelta		= 0;
WORD _AngleEncoderVelocity	= 0;

// Mast control inputs
VBOOL _ActionButton			= FALSE;	// Pin 15/A1 / PC1 (PCINT9)
VBOOL _OneShotButton		= FALSE;	// Pin 16/A2 / PC2 (PCINT10)
VBOOL _LatchButton			= FALSE;	// Pin 17/A3 / PC3 (PCINT11)


// OUTPUTS

// LEDs
VBOOL _StatusLed			= LOW;
VBOOL _ActionLed			= HIGH;


// STATE

#ifdef _DEBUG

DWORD _MemoryInfoLastMS		= 0;

#ifdef DEBUG_INPUTS
DWORD _PrintInputsLastMS	= 0;
#endif

#endif

WORD _Degrees				= 0;	// × ANGLE_PRECISION_FACTOR precision scaling factor
WORD _DegreesNew			= 0;	// × ANGLE_PRECISION_FACTOR precision scaling factor

BOOL _AngleSet				= TRUE;

Error _DriverError			= Error::SUCCESS;
PCCHAR _DriverStatusMsg		= "ASDF";
DriverStatus _DriverStatus	= DriverStatus::IDLE;

Error _ControllerError		= Error::SUCCESS;
PCCHAR _ControllerStatusMsg	= NULL;
ControllerStatus _ControllerStatus	= ControllerStatus::NONE;

#pragma endregion


#pragma region ANGLE MEASUREMENT & ADJUSTMENT EXPRESSION FUNCTIONS

STATIC CWORD PointsFromAngleEncoderSteps()
{
	return (CWORD)(_AngleEncoderSteps * ANGLE_STEP_PRECISION_FACTOR / ANGLE_ENCODER_STEPS_PER_POINT);
}

STATIC CBOOL IsAngleAdjustmentWithinErrorMargin(RCDWORD targetMotorSteps)
{
	return (CDWORD)_AngleEncoderSteps * ANGLE_STEP_PRECISION_FACTOR / ANGLE_ENCODER_STEPS_PER_POINT
			- targetMotorSteps * ANGLE_STEP_PRECISION_FACTOR / ANGLE_MOTOR_STEPS_PER_POINT
		< ANGLE_ENCODER_POINT_ERROR_MARGIN / ANGLE_POINTS_PER_DEGREE_VALUE;
}

#pragma endregion


#pragma region PROGRAM FUNCTION DECLARATIONS

VOID CleanUp();
VOID InitializeInterrupts();

MESSAGEHANDLER OnMessage;

VOID DEBUG_PrintInputValues();

#pragma endregion


#pragma region PROGRAM OUTLINE: ENTRY POINT, LOOP, ETC.

VOID setup()
{
	Serial.begin(SERIAL_BAUD_RATE);

	atexit(CleanUp);

	InitializePins();
	InitializeInterrupts();


#ifdef _DEBUG

	PrintLine("\nREADY!\n", Serial);

	_MemoryInfoLastMS = millis();

	#ifdef DEBUG_INPUTS
		_PrintInputsLastMS = millis();
	#endif

#endif
}

VOID cleanUp()
{
	PrintString(F("** FATAL ERROR **"));
}

VOID serialEvent()
{
	WaitForMessage(Serial, OnMessage);
}

VOID loop()
{


#ifdef _DEBUG

	if (_MemoryInfoLastMS + DEBUG_MEMORY_INFO_INTERVAL_MS >= millis())
	{
		PrintString(F("\nRAM: "));
		PrintLine((CWORD)SramFree());
		PrintLine();

		_ActionLed = !_ActionLed;
		WritePin(PIN_OUT_ACTION_LED, _ActionLed);

		_StatusLed = !_StatusLed;
		WritePin(PIN_OUT_STATUS_LED, _StatusLed);

		_MemoryInfoLastMS = millis();
	}

	#ifdef DEBUG_INPUTS

	if (_PrintInputsLastMS + DEBUG_INPUTS_INTERVAL_MS >= millis())
	{
		DEBUG_PrintInputValues();

		_PrintInputsLastMS = millis();
	}

	#endif

#endif
}

#pragma endregion


#pragma region INTERRUPT VECTORS and SUPPORTING MACROS & INLINED FUNCTIONS

// INT0/INT1: ANGLE ENCODER

#define _ISR_ANGLE_ENCODER_READ_CHANNEL(channel, other_channel, increment_comparison)					\
	_AngleEncoder ## channel = !_AngleEncoder ## channel;												\
	_AngleEncoderUp = (_AngleEncoder ## channel increment_comparison _AngleEncoder ## other_channel);	\
	_ISR_AngleEncoder_UpdateAngleSteps();

STATIC INLINE VOID _ISR_AngleEncoder_UpdateAngleSteps() ALWAYS_INLINE;

STATIC INLINE VOID _ISR_AngleEncoder_UpdateAngleSteps()
{
	if (_AngleEncoderUp)
		++_AngleEncoderSteps;
	else
		--_AngleEncoderSteps;

#ifdef DEBUG_INPUTS
	PrintString(F("ANGLE: "));
	PrintLine((CDWORD)_AngleEncoderSteps);
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

#pragma endregion


#pragma region PROGRAM FUNCTIONS

VOID CleanUp() { }

VOID InitializeInterrupts()
{
	// External interrupts: Angle encoder
	EIMSK |= 0b00000011;
	EICRA &= 0b11110101;
	EICRA |= 0b00000101;
}

VOID OnMessage(PIMESSAGE message)
{
	CONST MessageCode msgCode = static_cast<CONST MessageCode>(message->GetMessageCode());

#ifdef DEBUG_MESSAGES
	PrintString(F("NEW MSG - CODE: "));
	PrintLine(message->GetMessageCode());
#endif

	BOOL msgHandled = FALSE;
	PPVOID results = NULL;
	PPCVOID state = NULL;

	switch (msgCode)
	{
	case MessageCode::ANGLE_REQUEST:

		state = new PCVOID[2] { &_DriverError, &_Degrees };
		msgHandled = reinterpret_cast<PANGLEREQUEST>(message)->Handle(NULL, state);

		break;


	case MessageCode::NEWANGLE_REQUEST:

		results = new PVOID[1] { &_DegreesNew };
		state = new PCVOID[1] { &_DriverError };
		msgHandled = reinterpret_cast<PNEWANGLEREQUEST>(message)->Handle(results, state);

	#ifdef DEBUG_MESSAGES
		PrintLine(_DegreesNew);
	#endif

		_AngleSet = FALSE;

		break;


	case MessageCode::STATUS_REQUEST:

		state = new PCVOID[4] { &_DriverError, &_DriverStatusMsg, &_DriverStatus, (PCVOID)TRUE };
		msgHandled = reinterpret_cast<PSTATUSREQUEST>(message)->Handle(NULL, state);

		break;


	case MessageCode::CONTROLLER_STATUS:

		results = new PVOID[3] { &_ControllerError, &_ControllerStatusMsg, &_ControllerStatus };
		msgHandled = reinterpret_cast<PCONTROLLERSTATUSRESPONSE>(message)->Handle(results);

	#ifdef DEBUG_MESSAGES
		PrintLine((BYTE)_ControllerError);
		PrintLine(_ControllerStatusMsg);
		PrintLine((BYTE)_ControllerStatus);
	#endif

		break;



	case MessageCode::ANGLE_RESPONSE:

		results = new PVOID[2] { &_DriverError, &_Degrees };
		msgHandled = reinterpret_cast<PANGLERESPONSE>(message)->Handle(results, state);

	#ifdef DEBUG_MESSAGES
		PrintLine((BYTE)_DriverError);
		PrintLine(_Degrees);
	#endif

		break;


	case MessageCode::NEWANGLE_RESPONSE:

		results = new PVOID[1] { &_DriverError };
		msgHandled = reinterpret_cast<PNEWANGLERESPONSE>(message)->Handle(results);

	#ifdef DEBUG_MESSAGES
		PrintLine((BYTE)_DriverError);
	#endif

		break;


	case MessageCode::DRIVER_STATUS:

		results = new PVOID[3] { &_DriverError, &_DriverStatusMsg, &_DriverStatus };
		msgHandled = reinterpret_cast<PDRIVERSTATUSRESPONSE>(message)->Handle(results);

	#ifdef DEBUG_MESSAGES
		PrintLine((BYTE)_DriverError);
		PrintLine(_DriverStatusMsg);
		PrintLine((BYTE)_DriverStatus);
	#endif

		break;



	default:

		break;
	}

	if (results)
	{
		delete[] results;
		results = NULL;
	}

	if (state)
	{
		delete[] state;
		state = NULL;
	}
}

#pragma endregion


#pragma region DEBUG UTILITY FUNCTIONS

VOID DEBUG_PrintInputValues()
{
	_AngleEncoderA	= CheckPinSet(PIN_ANGLE_ENCODER_A);
	_AngleEncoderB	= CheckPinSet(PIN_ANGLE_ENCODER_B);

	_ActionButton	= CheckPinUnset(PIN_BUTTON_ACTION);
	_OneShotButton	= CheckPinUnset(PIN_BUTTON_ONESHOT);
	_LatchButton	= CheckPinUnset(PIN_BUTTON_LATCH);


	// REAR INPUTS

	PrintBit((BIT)_AngleEncoderA);
	PrintString(F(" "));
	PrintBit((BIT)_AngleEncoderB);
	PrintLine();


	// FRONT INPUTS

	PrintBit((BIT)_LatchButton);
	PrintString(F(" "));
	PrintBit((BIT)_OneShotButton);
	PrintString(F(" "));
	PrintBit((BIT)_ActionButton);
	PrintLine();

	PrintLine();
}

#pragma endregion
