/*****************************************************************************************************
* Name:		Foxetron Angle Controller (Firmware Code; Part of the Foxetron Program Suite)
* Created:	10/2/2016
* Author:	Tom Biuso <tjbtech@yahoo.com>
*
* Target Architecture:	Atmel AVR / ATmega series 8-bit MCUs
* Supported Platforms:	Arduino, AVR LibC, (AVR GCC)
*
*		Memory Usage:	~26.70 KB Program Memory (Flash ROM) / 1604 B SRAM
*		NOTE: ^-- w/ #define NO_ITTYBITTY_FULL_BYTES
*
* [Hardware Platform]
*	MCU:			Arduino Nano-like clone w/ Atmel ATmega 1284P
*	LCD:				HD44780 16×2, RGB backlight; I2C "backpack"
*	RTC & EEPROM:	Tiny RTC module w/ DS1307Z + 24C32N
*	Power Supply:		LM317 adjustable voltage regulator
*	Switches/Features:	1 × SPDT; 1 × rotary encoder w/ pushbutton switch
*	Buttons:			1 × square momentary, tactile; 1 × round momentary;
*						5 × momentary, tactile w/ red LED backlight
*	Other Components:	1 × RGB LED  10mm, diffused; 1 × Blue LED 5mm; 3-wire brushless fan
*	Hardware:		Aluminum enclosure; DC plug; headers, connectors, wires, mounts, etc.
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


#pragma region INCLUDES

// PROJECT INCLUDES
#include "Foxetron_common.h"
#include "Foxetron_AngleController_pins.h"
#include "Foxetron_AngleController_LCD_chars.h"

// PROJECT MODULES
#include "Foxetron_AngleController_LCD.h"
#include "Foxetron_AngleController_RGB.h"
#include "Foxetron_AngleController_EEPROM.h"

// PROJECT LIBS
//#include "libs/LiquidCrystal_I2C.custom.h"	// Included by other project libs
#include "libs/BigCrystal_I2C.custom.h"
#include "libs/MENWIZ.custom.h"
#include "libs/phi_Prompt.custom.h"
#include "libs/RTCLib.custom.h"

// 3RD-PARTY LIBS
//#include "VaRGB.h"						// Included by Foxetron_RGB
//#include "VaRGBCurves.h"					// Included by Foxetron_RGB

// ARDUINO LIBS
//#include "EEPROM.h"						// Included by project/3rd-party libs
//#include "Wire.h"							// Included by project/3rd-party libs

// ARDUINO CORE
//#include "Arduino.h"						// Included by project/3rd-party libs

// AVR LibC
//#include <avr/pgmspace.h>					// Included by project 3rd-party libs

#pragma endregion


#pragma region DEFINES

// PROGRAM OPTIONS

#define INPUT_PROCESS_INTERVAL_uS				5000			// Period by which input state changes should be polled and handled

#pragma endregion


#pragma region PROGRAM CONSTANTS

// FLASH DATA

#pragma endregion


#pragma region PROGRAM VARIABLES

// EEPROM VARIABLES


// INPUTS

//VBOOL _AngleEncoderA= FALSE;	// Pin 2 / PD2 (INT0) - [UNUSED]
//VBOOL _AngleEncoderB	= FALSE;	// Pin 3 / PD3 (INT1) - [UNUSED]
//VBOOL _AngleEncoderZ	= FALSE;	// Pin 4 / PD4 (PCINT20)	- [UNUSED]
//VBOOL _AngleEncoderU	= FALSE;	// Pin 5 / PD5 (PCINT21)	- [UNUSED]

VBOOL _LedButton1		= FALSE;	// Pin 4 / PD4 (PCINT20)
VBOOL _LedButton2		= FALSE;	// Pin 5 / PD5 (PCINT21)
VBOOL _LedButton3		= FALSE;	// Pin 6 / PD6 (PCINT22)
VBOOL _LedButton4		= FALSE;	// Pin 7 / PD7 (PCINT23)
VBOOL _LedButton5		= FALSE;	// Pin 8 / PB0 (PCINT0)

// [FREE PIN: Pin 12 / PB4 (PCINT4)]

// [FREE PIN: Pin A6 / ADC6]

VBOOL _ModeSwitch		= FALSE;	// A7 / ADC7
VWORD _ModeSwitchVal	= 0;

// Menu rotary encoder
VBOOL _MenuEncoderA		= FALSE;	// Pin 14/A0 / PC0 (PCINT8)
VBOOL _MenuEncoderB		= FALSE;	// Pin 15/A1 / PC1 (PCINT9)

VBOOL _SelectButton		= FALSE;	// Pin 16/A2 / PC2 (PCINT10)
VBOOL _ShiftButton		= FALSE;	// Pin 17/A3 / PC3 (PCINT11)


// OUTPUTS

// LEDs
VBYTE _RgbRed			= 0;		// Pin 9 / PB1
VBYTE _RgbGreen			= 0;		// Pin 10 / PB2
VBYTE _RgbBlue			= 0;		// Pin 11 / PB3

VBOOL _StatusLed		= LOW;		// Pin 13 / PB5


// STATE

#ifdef _DEBUG

DWORD _MemoryInfoLastMS				= 0;

#ifdef DEBUG_INPUTS
DWORD _PrintInputsLastMS			= 0;
#endif

#endif

VBOOL _MenuEncoderUp				= FALSE;
VDWORD _MenuEncoderSteps			= 0;
DWORD _MenuEncoderStepsLast			= 0;
WORD _MenuEncoderVelocity			= 0;

ANGLEMODE _AngleMode				= AngleMode::ABSOLUTE;
WORD _CalibrationDegrees			= 0;

WORD _Degrees						= 0;	// × ANGLE_PRECISION_FACTOR precision scaling factor
WORD _DegreesNew					= 0;	// × ANGLE_PRECISION_FACTOR precision scaling factor

Error _ControllerError				= Error::SUCCESS;
PCCHAR _ControllerStatusMsg			= NULL;
ControllerStatus _ControllerStatus	= ControllerStatus::NONE;

Error _DriverError					= Error::SUCCESS;
PCCHAR _DriverStatusMsg				= NULL;
DriverStatus _DriverStatus			= DriverStatus::IDLE;

#pragma endregion


#pragma region PROGRAM FUNCTION DECLARATIONS

VOID CleanUp();
VOID InitializeTimers();
VOID InitializeInterrupts();

MESSAGEHANDLER OnMessage;

VOID PrintLCDSplash();

VOID DEBUG_PrintInputValues();
VOID DEBUG_DisplayKeyCodes();
VOID DEBUG_DisplayCustomChars();

#pragma endregion


#pragma region PROGRAM OUTLINE: ENTRY POINT, LOOP, ETC.

VOID setup()
{
	Serial.begin(SERIAL_BAUD_RATE);

	atexit(CleanUp);

	cli();

	InitializePins();
	InitializeTimers();
	InitializeInterrupts();

	sei();

	LCD_Initialize();
	RGB_Initialize();

	LCD->printBig(F("Fox"), 2, 0);


#ifdef _DEBUG

	PrintLine(F("\nREADY!\n"), Serial);

	_MemoryInfoLastMS = millis();

	#ifdef DEBUG_INPUTS
		_PrintInputsLastMS = millis();
	#endif

#endif
}

VOID serialEvent()
{
	WaitForMessage(Serial, OnMessage);
}

VOID loop()
{
	RGB_Step();


#ifdef _DEBUG

	if (_MemoryInfoLastMS + DEBUG_MEMORY_INFO_INTERVAL_MS >= millis())
	{
		PrintString(F("\nRAM: "));
		PrintLine((CWORD)SramFree());
		PrintLine();

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

// INT0/INT1: MENU ENCODER

#define _ISR_MENU_ENCODER_READ_CHANNEL(channel, other_channel, increment_comparison)				\
	_MenuEncoder ## channel = !_MenuEncoder ## channel;												\
	_MenuEncoderUp = (_MenuEncoder ## channel increment_comparison _MenuEncoder ## other_channel);	\
	_ISR_Encoder_UpdateEncoderSteps();

STATIC INLINE VOID _ISR_Encoder_UpdateEncoderSteps() ALWAYS_INLINE;

STATIC INLINE VOID _ISR_Encoder_UpdateEncoderSteps()
{
	if (_MenuEncoderUp)
		++_MenuEncoderSteps;
	else
		--_MenuEncoderSteps;


#ifdef DEBUG_INPUTS
	PrintString(F("MENU ENCODER: "));
	PrintLine((CDWORD)_MenuEncoderSteps);
#endif
}

// CHANNEL A
ISR(INT0_vect)
{
	_ISR_MENU_ENCODER_READ_CHANNEL(A, B, == );
}

// CHANNEL B
ISR(INT1_vect)
{
	_ISR_MENU_ENCODER_READ_CHANNEL(B, A, != );
}


// PCINT0/PCINT1/PCINT2: LED BUTTONS, MENU ENCODER, and MENU BUTTONS

// PORT B (PCINT0:7): LED BUTTON 5
ISR(PCINT0_vect, ISR_NOBLOCK)
{
	_LedButton5 = PB0_IS_SET();
}

// PORT C (PCINT8:14): MENU ENCODER & BUTTONS
ISR(PCINT1_vect, ISR_NOBLOCK)
{
	if (_MenuEncoderA != PC3_IS_SET())
	{
		_MenuEncoderA = PC3_IS_SET();
		_MenuEncoderUp = (_MenuEncoderA == _MenuEncoderB);
		_ISR_Encoder_UpdateEncoderSteps();
	}

	if (_MenuEncoderB != PC2_IS_SET())
	{
		_MenuEncoderB = PC2_IS_SET();
		_MenuEncoderUp = (_MenuEncoderA != _MenuEncoderB);
		_ISR_Encoder_UpdateEncoderSteps();
	}

	_SelectButton = PC1_IS_SET();
	_ShiftButton = PC0_IS_SET();
}

// PORT D (PCINT16:23): LED BUTTONS 1-4
ISR(PCINT2_vect, ISR_NOBLOCK)
{
	_LedButton1 = PD4_IS_SET();
	_LedButton2 = PD5_IS_SET();
	_LedButton3 = PD6_IS_SET();
	_LedButton4 = PD7_IS_SET();
}// TIMER EVENTS

// TIMER 2: COMPARE MATCH A
ISR(TIMER2_COMPA_vect, ISR_NOBLOCK){
	_MenuEncoderVelocity = (_MenuEncoderStepsLast - _MenuEncoderSteps) * uS_PER_SECOND / PROCESS_TIMER_OVERFLOW_uS / OCR2A;
	_MenuEncoderStepsLast = _MenuEncoderSteps;}#pragma endregion


#pragma region PROGRAM FUNCTIONS

VOID CleanUp()
{
	LCD_Free();
	RGB_Free();
}

VOID InitializeTimers()
{
	// Timer 2: Angle adjustment task; CTC mode
	SET_BITS(TCCR2B, B(WGM21) OR B(CS22) OR B(CS21) OR B(CS20));
	OCR2A = (CBYTE)((CDWORD)INPUT_PROCESS_INTERVAL_uS / PROCESS_TIMER_OVERFLOW_uS);
	SET_BIT(TIMSK2, OCIE2A);
}

VOID InitializeInterrupts()
{
	// External interrupts: Angle encoder
	CLEAR_BITS(EICRA, B(ISC11) OR B(ISC01));
	SET_BITS(EICRA, B(ISC10) OR B(ISC00));
	SET_BITS(EIMSK, B(INT1) OR B(INT0));

	// Pin change interrupts: LED buttons, menu encoder, and menu buttons (ports B/C/D)
	SET_BITS(PCICR, B(PCIE2) OR B(PCIE1) OR B(PCIE0));
	SET_BITS(PCMSK0, 0b00010001);
	SET_BITS(PCMSK1, 0b00001111);
	SET_BITS(PCMSK2, 0b11110000);
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
	case MessageCode::CALIBRATE_REQUEST:

		state = new PCVOID[1] { &_ControllerError };
		results = new PVOID[2] { &_AngleMode, &_CalibrationDegrees };
		msgHandled = reinterpret_cast<PCALIBRATEREQUEST>(message)->Handle(results, state);

	#ifdef DEBUG_MESSAGES
		PrintLine((CBOOL)_AngleMode);
		PrintLine((CWORD)_CalibrationDegrees);
	#endif

		break;



	case MessageCode::STATUS_REQUEST:

		state = new PCVOID[4] { &_ControllerError, &_ControllerStatusMsg, &_ControllerStatus, NULL };
		msgHandled = reinterpret_cast<PSTATUSREQUEST>(message)->Handle(NULL, state);

		break;


	case MessageCode::CALIBRATE_RESPONSE:

		results = new PVOID[1] { &_DriverError };
		msgHandled = reinterpret_cast<PCALIBRATERESPONSE>(message)->Handle(results, state);

	#ifdef DEBUG_MESSAGES
		PrintLine((CBYTE)_DriverError);
	#endif

		break;


	case MessageCode::ANGLE_RESPONSE:

		results = new PVOID[2] { &_DriverError, &_Degrees };
		msgHandled = reinterpret_cast<PANGLERESPONSE>(message)->Handle(results, state);

	#ifdef DEBUG_MESSAGES
		PrintLine((CBYTE)_DriverError);
		PrintLine(_Degrees);
	#endif

		break;


	case MessageCode::NEWANGLE_RESPONSE:

		results = new PVOID[1] { &_DriverError };
		msgHandled = reinterpret_cast<PNEWANGLERESPONSE>(message)->Handle(results);

	#ifdef DEBUG_MESSAGES
		PrintLine((CBYTE)_DriverError);
	#endif

		break;


	case MessageCode::DRIVER_STATUS:

		results = new PVOID[3] { &_DriverError, &_DriverStatusMsg, &_DriverStatus };
		msgHandled = reinterpret_cast<PDRIVERSTATUSRESPONSE>(message)->Handle(results);

	#ifdef DEBUG_MESSAGES
		PrintLine((CBYTE)_DriverError);
		if (_DriverStatusMsg)
			PrintLine(_DriverStatusMsg);
		PrintLine((CBYTE)_DriverStatus);
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

VOID PrintLCDSplash()
{
	LCD->print(F("Foxetron test..."));

	delay(200);

	for (BYTE i = 0; i < 16; i++)
	{
		LCD->scrollDisplayLeft();
		delay(20);
	}

	LCD->clear();
}

#pragma endregion


#pragma region DEBUG UTILITY FUNCTIONS

VOID DEBUG_PrintInputValues()
{
	STATIC CHAR valStr[5];

	_LedButton1 = CheckPinSet(PIN_LED_BUTTON_1);
	if (_LedButton1)
		ResetPin(PIN_LED_BUTTON_1);

	_LedButton2 = CheckPinSet(PIN_LED_BUTTON_2);
	if (_LedButton2)
		ResetPin(PIN_LED_BUTTON_2);

	_LedButton3 = CheckPinSet(PIN_LED_BUTTON_3);
	if (_LedButton3)
		ResetPin(PIN_LED_BUTTON_3);

	_LedButton4 = CheckPinSet(PIN_LED_BUTTON_4);
	if (_LedButton4)
		ResetPin(PIN_LED_BUTTON_4);

	_LedButton5 = CheckPinSet(PIN_LED_BUTTON_5);
	if (_LedButton5)
		ResetPin(PIN_LED_BUTTON_5);

	_ModeSwitch		= analogRead(PIN_ADC_MODE_SWITCH) > 500 ? TRUE : FALSE;

	_MenuEncoderA	= CheckPinSet(PIN_MENU_ENCODER_A);
	_MenuEncoderB	= CheckPinSet(PIN_MENU_ENCODER_B);

	_SelectButton	= CheckPinUnset(PIN_SELECT_BUTTON);
	_ShiftButton	= CheckPinUnset(PIN_SHIFT_BUTTON);


	// REAR INPUTS

	LCD->clear();

	LCD->home();

	LCD->setCursor(4, 0);
	LCD->print(itoa(_LedButton1, valStr, 2));

	LCD->setCursor(6, 0);
	LCD->print(itoa(_LedButton2, valStr, 2));

	LCD->setCursor(8, 0);
	LCD->print(itoa(_LedButton3, valStr, 2));

	LCD->setCursor(10, 0);
	LCD->print(itoa(_LedButton4, valStr, 2));

	LCD->setCursor(12, 0);
	LCD->print(itoa(_LedButton5, valStr, 2));

	PrintBit((BIT)_LedButton1);
	PrintString(F(" "));
	PrintBit((BIT)_LedButton2);
	PrintString(F(" "));
	PrintBit((BIT)_LedButton3);
	PrintString(F(" "));
	PrintBit((BIT)_LedButton4);
	PrintString(F(" "));
	PrintBit((BIT)_LedButton5);
	PrintLine();


	// FRONT INPUTS

	LCD->setCursor(4, 1);
	LCD->print(itoa(_ModeSwitchVal, valStr, 10));

	LCD->setCursor(8, 1);
	LCD->print(itoa(_MenuEncoderA, valStr, 2));

	LCD->setCursor(10, 1);
	LCD->print(itoa(_MenuEncoderB, valStr, 2));

	LCD->setCursor(12, 1);
	LCD->print(itoa(_SelectButton, valStr, 2));

	LCD->setCursor(14, 1);
	LCD->print(itoa(_ShiftButton, valStr, 2));

	Serial.print(itoa(_ModeSwitchVal, valStr, 10));
	PrintString(F(" / "));
	PrintBit((BIT)_ModeSwitch);
	PrintString(F(" "));
	PrintBit((BIT)_MenuEncoderA);
	PrintString(F(" "));
	PrintBit((BIT)_MenuEncoderB);
	PrintString(F(" "));
	PrintBit((BIT)_SelectButton);
	PrintString(F(" "));
	PrintBit((BIT)_ShiftButton);
	PrintLine();

	PrintLine();
}

VOID DEBUG_DisplayKeyCodes()
{
	uint8_t i = 0;

	while (i < 1)
	{
		LCD->clear();

		LCD->print(F("Codes 0x"));

		LCD->print(i, HEX);
		LCD->print(F("-0x"));
		LCD->print(i + 16, HEX);

		LCD->setCursor(0, 1);

		for (int j = 0; j < 8; j++)
			LCD->write(i + j);

		i += 16;

		while (!Serial.available()) delay(100);
		Serial.read();
	}
}

VOID DEBUG_DisplayCustomChars()
{
	LCD->createChar(0, LCD_CHAR_FOX);
	LCD->createChar(1, LCD_CHAR_GEM_SMALL);
	LCD->createChar(2, LCD_CHAR_GEM);

	LCD->createChar(3, LCD_CHAR_ANGLE);
	LCD->createChar(4, LCD_CHAR_ANGLE_2);
	LCD->createChar(5, LCD_CHAR_CHECKMARK);
	LCD->createChar(6, LCD_CHAR_CLOCKWISE);
	LCD->createChar(7, LCD_CHAR_NOTES);

	DEBUG_DisplayKeyCodes();


	LCD->createChar(0, LCD_CHAR_SHAPE_ROUND);
	LCD->createChar(1, LCD_CHAR_SHAPE_OVAL);
	LCD->createChar(2, LCD_CHAR_SHAPE_CUSHION);
	LCD->createChar(3, LCD_CHAR_SHAPE_EMERALD);
	LCD->createChar(4, LCD_CHAR_SHAPE_BAGUETTE);
	LCD->createChar(5, LCD_CHAR_SHAPE_TRILLIANT);
	LCD->createChar(6, LCD_CHAR_SHAPE_TRIANGLE);
	LCD->createChar(7, LCD_CHAR_SHAPE_SPECIAL);

	DEBUG_DisplayKeyCodes();


	LCD->createChar(0, LCD_CHAR_CUT_BRILLIANT);
	LCD->createChar(1, LCD_CHAR_CUT_STEP);

	LCD->createChar(2, LCD_CHAR_ARROW_UP);
	LCD->createChar(3, LCD_CHAR_ARROW_DOWN);
	LCD->createChar(4, LCD_CHAR_ARROW_LEFT_LARGE);
	LCD->createChar(5, LCD_CHAR_ARROW_RIGHT_LARGE);

	DEBUG_DisplayKeyCodes();
}

#pragma endregion
