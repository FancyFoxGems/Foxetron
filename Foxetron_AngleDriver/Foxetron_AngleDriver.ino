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

// ITTY BITTY
#include "IttyBitty.h"
#include "IttyBitty_EEPROM_I2C.h"

// PROJECT INCLUDES
#include "Foxetron_pins.h"

// PROJECT MODULES
#include "Foxetron_messages.h"

using namespace Foxetron;

// PROJECT LIBS

// 3RD-PARTY LIBS
#include "HalfStepper.h"					// well...1st-party in this case, really.

// ARDUINO LIBS
#include "SD.h"

// ARDUINO CORE
//#include <Arduino.h>							// included by project/3rd-party libs

// AVR LibC
//#include <avr/pgmspace.h>						// included by project 3rd-party libs

#pragma endregion


#pragma region DEFINES

// PROGRAM OPTIONS

#define DEBUG_INPUTS			0
#define DEBUG_INPUT_DELAY_MS	500

#if defined(DEBUG_INPUTS) && DEBUG_INPUTS != 1
	#undef DEBUG_INPUTS
#endif

#define SERIAL_BAUD_RATE		115200
#define SERIAL_DELAY_MS			1

#pragma endregion


#pragma region PROGRAM CONSTANTS

#pragma endregion


#pragma region PROGRAM VARIABLES

// INPUTS

// Angle encoder
VBOOL _AngleEncoderA	= FALSE;	// Pin 2 / PD2 (INT0)
VBOOL _AngleEncoderB	= FALSE;	// Pin 3 / PD3 (INT1)

VBOOL _AngleUp			= FALSE;
VDWORD _AngleReading	= 0;
WORD _AngleDelta		= 0;
WORD _AngleVelocity		= 0;

// Mast control inputs
VBOOL _ActionButton		= FALSE;	// Pin 15/A1 / PC1 (PCINT9)
VBOOL _OneShotButton	= FALSE;	// Pin 16/A2 / PC2 (PCINT10)
VBOOL _LatchButton		= FALSE;	// Pin 17/A3 / PC3 (PCINT11)


// OUTPUTS

// LEDs
VBOOL _StatusLed		= LOW;
VBOOL _ActionLed		= HIGH;


// STATE

WORD _Degrees				= 0;
WORD _DegreesNew			= 0;

Error _DriverError			= Error::SUCCESS;
PCCHAR _DriverStatusMsg		= "ASDF";
DriverStatus _DriverStatus	= DriverStatus::IDLE;

Error _ControllerError		= Error::SUCCESS;
PCCHAR _ControllerStatusMsg	= NULL;
ControllerStatus _ControllerStatus	= ControllerStatus::NONE;


#pragma region PROGRAM FUNCTION DECLARATIONS

VOID cleanUp();
VOID initializeInterrupts();

MESSAGEHANDLER onMessage;

VOID DEBUG_printInputValues();

#pragma endregion


#pragma region PROGRAM OUTLINE: ENTRY POINT, LOOP, ETC.

VOID setup()
{
	Serial.begin(SERIAL_BAUD_RATE);

#ifdef _DEBUG
	PrintLine("\nREADY!\n", Serial);
#endif

	atexit(cleanUp);

	//initializePins();
	//initializeInterrupts();

	//SetPinMode(SS, OUTPUT);
	pinMode(SS, OUTPUT);
	digitalWrite(SS, HIGH);

	Sd2Card card;
	PrintLine((CBYTE)card.init(SPI_QUARTER_SPEED, 10));
	
	PrintLine((CDWORD)card.cardSize());
	PrintLine((CBYTE)card.type());

	SDClass sd;
	PrintLine((CBYTE)sd.begin());

	File f = sd.open("TEST.TXT", FILE_WRITE);
	if (!f) PrintLine("OPEN FAILED");
	PrintLine((CBYTE)f.println("test"));
	f.flush();
	f.close();

	f = sd.open("TEST.TXT");
	if (!f) PrintLine("OPEN FAILED");
	BYTE buffer[5];
	PrintLine((CBYTE)f.read(buffer, 4));
	buffer[4] = '\0';
	PrintLine(reinterpret_cast<PCCHAR>(buffer));
	f.close();
}

VOID cleanUp()
{
	PrintString(F("** FATAL ERROR **"));
}

VOID serialEvent()
{
	//WaitForMessage(Serial, onMessage);
}

int freeRam()
{
	extern int __heap_start, *__brkval; 
	int v; 
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

VOID loop()
{	
#ifdef _DEBUG

	return;

	PrintString(F("\nRAM: "));
	PrintLine((WORD)freeRam());
	PrintLine();

	delay(3000);

#endif

#ifdef DEBUG_INPUTS

	DEBUG_printInputValues();

	_ActionLed = !_ActionLed;
	WRITE_ARDUINO_PIN(PIN_OUT_ACTION_LED, _ActionLed);

	_StatusLed = !_StatusLed;
	WRITE_ARDUINO_PIN(PIN_OUT_STATUS_LED, _StatusLed);

	delay(DEBUG_INPUT_DELAY_MS);

#endif
}

#pragma endregion


#pragma region INTERRUPT VECTORS and SUPPORTING MACROS & INLINED FUNCTIONS

// INT0/INT1: ANGLE ENCODER

#define _ISR_ANGLE_ENCODER_READ_CHANNEL(channel, other_channel, increment_comparison)					\
	_AngleEncoder ## channel = !_AngleEncoder ## channel;												\
	_AngleUp = (_AngleEncoder ## channel increment_comparison _AngleEncoder ## other_channel);			\
	_ISR_AngleEncoder_updateAngleReading();

STATIC INLINE VOID _ISR_AngleEncoder_updateAngleReading() ALWAYS_INLINE;

STATIC INLINE VOID _ISR_AngleEncoder_updateAngleReading()
{
	if (_AngleUp)
		++_AngleReading;
	else
		--_AngleReading;

#ifdef DEBUG_INPUTS
	PrintString(F("ANGLE: "));
	PrintLine((LONG)_AngleReading);
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

#pragma endregion


#pragma region PROGRAM FUNCTIONS

VOID initializeInterrupts()
{
	// External interrupts: Angle encoder
	EIMSK |= 0b00000011;
	EICRA &= 0b11110101;
	EICRA |= 0b00000101;
}
/*
VOID onMessage(PIMESSAGE message)
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
*/
#pragma endregion


#pragma region DEBUG UTILITY FUNCTIONS

VOID DEBUG_printInputValues()
{
	_AngleEncoderA	= CHECK_ARDUINO_PIN(PIN_ANGLE_ENCODER_A);
	_AngleEncoderB	= CHECK_ARDUINO_PIN(PIN_ANGLE_ENCODER_B);

	_ActionButton	= !CHECK_ARDUINO_PIN(PIN_BUTTON_ACTION);
	_OneShotButton	= !CHECK_ARDUINO_PIN(PIN_BUTTON_ONESHOT);
	_LatchButton	= !CHECK_ARDUINO_PIN(PIN_BUTTON_LATCH);


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
