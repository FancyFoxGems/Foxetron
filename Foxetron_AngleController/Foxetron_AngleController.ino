/*****************************************************************************************************
* Name:		Foxetron Angle Controller (Firmware Code; Part of the Foxetron Program Suite)
* Created:	10/2/2016
* Author:	Tom Biuso <tjbtech@yahoo.com>
*
* Target Architecture:	Atmel AVR / ATmega series 8-bit MCUs
* Supported Platforms:	Arduino, AVR LibC, (AVR GCC)
*
*		Memory Usage:	~26.87 KB Program Memory (Flash ROM) / 1604 B SRAM
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
#include "Foxetron_AngleController_initialize.h"

// PROJECT MODULES
#include "Foxetron_AngleController_LCD.h"
#include "Foxetron_AngleController_RGB.h"
#include "Foxetron_AngleController_EEPROM.h"

// PROJECT LIBS
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

#define INPUT_PROCESS_INTERVAL_uS		5000	// Period by which input state changes should be polled and handled

#define MODE_SWITCH_THRESHOLD			500		// ADC value at which >= readings set mode switch state to TRUE


// SPASH SCREEN OPTIONS

#define LCD_SPLASH_DELAY_1_MS			1000
#define LCD_SPLASH_DELAY_2_MS			2000
#define LCD_SPLASH_SCROLLS				120
#define LCD_SPLASH_SPEED_MS				30

#pragma endregion


#pragma region PROGRAM CONSTANTS

// FLASH DATA

#pragma endregion


#pragma region PROGRAM VARIABLES

// EEPROM VARIABLES


// INPUTS

// Angle measurement encoder (absolute/position)
UNUSED VBOOL _AngleEncoderA	= FALSE;	// Pin 2 / PD2 (INT0)
UNUSED VBOOL _AngleEncoderB	= FALSE;	// Pin 3 / PD3 (INT1)
UNUSED VBOOL _AngleEncoderZ	= FALSE;	// Pin 4 / PD4 (PCINT20)
UNUSED VBOOL _AngleEncoderU	= FALSE;	// Pin 5 / PD5 (PCINT21)

// Lighted buttons
VBOOL _LedButton1			= FALSE;	// Pin 4 / PD4 (PCINT20)
VBOOL _LedButton2			= FALSE;	// Pin 5 / PD5 (PCINT21)
VBOOL _LedButton3			= FALSE;	// Pin 6 / PD6 (PCINT22)
VBOOL _LedButton4			= FALSE;	// Pin 7 / PD7 (PCINT23)
VBOOL _LedButton5			= FALSE;	// Pin 8 / PB0 (PCINT0)

// [FREE PIN: Pin 12 / PB4 (PCINT4)]

// [FREE PIN: Pin A6 / ADC6]

// Mode switch (SPDT)
VBOOL _ModeSwitch			= FALSE;	// A7 / ADC7
VWORD _ModeSwitchVal		= 0;

// Menu navigation rotary encoder
VBOOL _MenuEncoderA			= FALSE;	// Pin 14/A0 / PC0 (PCINT8)
VBOOL _MenuEncoderB			= FALSE;	// Pin 15/A1 / PC1 (PCINT9)

// Unlit buttons
VBOOL _SelectButton			= FALSE;	// Pin 16/A2 / PC2 (PCINT10)
VBOOL _ShiftButton			= FALSE;	// Pin 17/A3 / PC3 (PCINT11)


// OUTPUTS

// RGB LED
VBYTE _RgbRed				= 0;		// Pin 9 / PB1
VBYTE _RgbGreen				= 0;		// Pin 10 / PB2
VBYTE _RgbBlue				= 0;		// Pin 11 / PB3

// On-board status LED
VBOOL _StatusLed			= LOW;		// Pin 13 / PB5


// STATE

VBOOL _MenuEncoderUp				= FALSE;
VDWORD _MenuEncoderSteps			= 0;
DWORD _MenuEncoderStepsLast			= 0;
WORD _MenuEncoderVelocity			= 0;

ANGLEMODE _AngleMode				= AngleMode::ABSOLUTE;
SHORT _CalibrationSteps				= 0;

WORD _Degrees						= 0;	// × ANGLE_PRECISION_FACTOR precision scaling factor
WORD _DegreesNew					= 0;	// × ANGLE_PRECISION_FACTOR precision scaling factor

Error _ControllerError				= Error::SUCCESS;
PCCHAR _ControllerStatusMsg			= NULL;
ControllerStatus _ControllerStatus	= ControllerStatus::NONE;

Error _DriverError					= Error::SUCCESS;
PCCHAR _DriverStatusMsg				= NULL;
DriverStatus _DriverStatus			= DriverStatus::IDLE;


// DEBUG

#ifdef _DEBUG

#ifdef DEBUG_MEMORY_INTERVAL_MS
LONG _LastPrintMemoryMs	= 0;
#endif

#ifdef DEBUG_INPUTS_INTERVAL_MS
LONG _LastPrintInputsMs	= 0;
#endif

#ifdef DEBUG_STATE_INTERVAL_MS
LONG _LastPrintStateMs	= 0;
#endif

#endif	// _DEBUG


// Menu-driven GUI

MenUI UI;

#pragma endregion


#pragma region PROGRAM FUNCTION DECLARATIONS

STATIC VOID CleanUp();

STATIC MESSAGEHANDLER OnMessage;

STATIC INLINE VOID PrintLCDSplash() ALWAYS_INLINE;

#pragma endregion


#pragma region DEBUG UTILITY FUNCTION DECLARATIONS

#ifdef _DEBUG

#ifdef DEBUG_MEMORY
STATIC INLINE VOID DEBUG_PrintMemory() ALWAYS_INLINE;
#endif

#ifdef DEBUG_INPUTS
STATIC INLINE VOID DEBUG_PrintInputs() ALWAYS_INLINE;
#endif

#ifdef DEBUG_STATE
STATIC INLINE VOID DEBUG_PrintState() ALWAYS_INLINE;
#endif

STATIC INLINE VOID DEBUG_DisplayCustomChars() ALWAYS_INLINE;
STATIC VOID DEBUG_DisplayKeyCodes();

#endif	// _DEBUG

#pragma endregion


#pragma region PROGRAM OUTLINE: ENTRY POINT, LOOP, ETC.

//#include "libs/MENWIZ.custom.h"
//menwiz UI;
//int Nav, list, sp=110;
//bool done;

//INT GetMenuNav()
//{
//	if (done)
//		Nav = MW_BTNULL;
//	else
//		done = TRUE;
//
//	return Nav;
//}

VOID setup()
{
	atexit(CleanUp);

	Serial.begin(SERIAL_BAUD_RATE);

	cli();

	InitializePins();
	InitializeInterrupts();
	InitializeTimers(INPUT_PROCESS_INTERVAL_uS);

	sei();

	LCD_Initialize();
	//RGB_Initialize();

	//PrintLCDSplash();

	//_menu *menu, *menu1, *menu2;

	//UI.addUsrNav(&GetMenuNav, 6);
	//UI.begin(20,4);

	//menu = UI.addMenu(MW_ROOT, NULL, F("Foxetron"));
	//	menu1 = UI.addMenu(MW_VAR, menu, F("Backlight"));
	//		menu1->addVar(MW_ACTION, LCD_ToggleBacklight);
	//	menu1 = UI.addMenu(MW_VAR, menu, F("INT Variable"));
	//		menu1->addVar(MW_AUTO_INT, &sp, 0, 120, 10);
	//	menu1 = UI.addMenu(MW_SUBMENU, menu, F("Node3"));
	//		menu2 = UI.addMenu(MW_VAR, menu1, F("Choice"));
	//			menu2->addVar(MW_LIST, &list);
	//				menu2->addItem(MW_LIST, F("Option1"));
	//				menu2->addItem(MW_LIST, F("Option2"));
	//				menu2->addItem(MW_LIST, F("Option3"));

	//UI.draw();

#ifdef _DEBUG
	PrintLine();
	PrintLine();
	PrintLine(F("READY!"));
#endif

#ifdef DEBUG_MEMORY_INTERVAL_MS
	DEBUG_PrintMemory();
	_LastPrintMemoryMs = millis();
#endif

#ifdef DEBUG_INPUTS_INTERVAL_MS
	DEBUG_PrintInputs();
	_LastPrintInputsMs = millis();
#endif

#ifdef DEBUG_STATE_INTERVAL_MS
	DEBUG_PrintState();
	_LastPrintStateMs = millis();
#endif
}

BOOL stopped = FALSE;

VOID serialEvent()
{
	//switch (Serial.read())
	//{
	//case '.':
	//	stopped = !stopped;
	//	break;

	//case 'u':
	//case 'w':
	//	Nav = MW_BTU;
	//	break;

	//case 'd':
	//case 'z':
	//	Nav = MW_BTD;
	//	break;

	//case 'l':
	//case 'a':
	//	Nav = MW_BTL;
	//	break;

	//case 'r':
	//case 's':
	//	Nav = MW_BTR;
	//	break;

	//case 'c':
	//case 'f':
	//	Nav = MW_BTC;
	//	break;

	//case 'e':
	//case 'x':
	//	Nav = MW_BTE;
	//	break;

	//default:
	//	Nav = MW_BTNULL;
	//}

	//done = FALSE;

	//UI.draw();

	//WaitForMessage(Serial, OnMessage);
}

VOID loop()
{
	if (stopped)
	{
		delay(1000);
	}
	else
	{
		//RGB_Step();
	}

#ifdef DEBUG_MEMORY_INTERVAL_MS
	if (HAS_INTERVAL_ELAPSED(_LastPrintMemoryMs, DEBUG_MEMORY_INTERVAL_MS, millis()))
	{
		DEBUG_PrintMemory();
		_LastPrintMemoryMs = millis();
	}
#endif

#ifdef DEBUG_INPUTS_INTERVAL_MS
	if (HAS_INTERVAL_ELAPSED(_LastPrintInputsMs, DEBUG_INPUTS_INTERVAL_MS, millis()))
	{
		DEBUG_PrintInputs();
		_LastPrintInputsMs = millis();
	}
#endif

#ifdef DEBUG_STATE_INTERVAL_MS
	if (HAS_INTERVAL_ELAPSED(_LastPrintStateMs, DEBUG_STATE_INTERVAL_MS, millis()))
	{
		DEBUG_PrintState();
		_LastPrintStateMs = millis();
	}
#endif
}

#pragma endregion


#pragma region INTERRUPT VECTORS and SUPPORTING MACROS & INLINED FUNCTIONS

// INT0/INT1: MENU ENCODER

#define _ISR_MENU_ENCODER_READ_CHANNEL(other_channel_set, up_on_equal)	\
	_OptionEncoderUp = XOR(up_on_equal, other_channel_set);				\
	if (NOT other_channel_set) _ISR_UpdateEncoderSteps();

STATIC INLINE VOID _ISR_UpdateEncoderSteps() ALWAYS_INLINE;
STATIC INLINE VOID _ISR_UpdateEncoderSteps()
{
	if (_MenuEncoderUp)
		++_MenuEncoderSteps;
	else
		--_MenuEncoderSteps;


#ifdef DEBUG_INPUTS
	PrintLine();
	PrintString(F("MENU ENCODER: "));
	PrintLine((CDWORD)_MenuEncoderSteps);
#endif
}

// CHANNEL A
ISR(INT0_vect)
{
	_ISR_MENU_ENCODER_READ_CHANNEL(PD3_IS_SET(), TRUE);
}

// CHANNEL B
ISR(INT1_vect)
{
	_ISR_MENU_ENCODER_READ_CHANNEL(PD2_IS_SET(), FALSE);
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
	/*if (_MenuEncoderA != PC1_IS_SET())
	{
		_MenuEncoderA = PC1_IS_SET();
		_MenuEncoderUp = (_MenuEncoderA == _MenuEncoderB);
		_ISR_Encoder_UpdateEncoderSteps();
	}

	if (_MenuEncoderB != PC0_IS_SET())
	{
		_MenuEncoderB = PC0_IS_SET();
		_MenuEncoderUp = (_MenuEncoderA != _MenuEncoderB);
		_ISR_Encoder_UpdateEncoderSteps();
	}*/

	_SelectButton = PC2_IS_SET();
	_ShiftButton = PC3_IS_SET();
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
	_MenuEncoderStepsLast = _MenuEncoderSteps;
	_ModeSwitch	= analogRead(PIN_ADC_MODE_SWITCH) > MODE_SWITCH_THRESHOLD;}#pragma endregion


#pragma region PROGRAM FUNCTIONS

VOID CleanUp()
{
	LCD_Free();
	RGB_Free();
}

VOID OnMessage(PIMESSAGE message)
{
	CONST MessageCode msgCode = static_cast<CONST MessageCode>(message->GetMessageCode());

#ifdef DEBUG_MESSAGES
	PrintLine();
	PrintString(F("NEW MSG - CODE: "));
	PrintLine(message->GetMessageCode());
#endif

	BOOL msgHandled = FALSE;
	PPTR results = NULL;
	PCPTR state = NULL;

	switch (msgCode)
	{
	case MessageCode::CALIBRATE_REQUEST:

		results = new PTR[2] { &_AngleMode, &_CalibrationSteps };
		state = new CPTR[1] { &_ControllerError };
		msgHandled = reinterpret_cast<PCALIBRATEREQUEST>(message)->Handle(results, state);

	#ifdef DEBUG_MESSAGES
		PrintLine((CBOOL)_AngleMode);
		PrintLine((CSHORT)_CalibrationSteps);
	#endif

		break;



	case MessageCode::STATUS_REQUEST:

		state = new CPTR[4] { &_ControllerError, &_ControllerStatusMsg, &_ControllerStatus, NULL };
		msgHandled = reinterpret_cast<PSTATUSREQUEST>(message)->Handle(NULL, state);

		break;


	case MessageCode::CALIBRATE_RESPONSE:

		results = new PTR[1] { &_DriverError };
		msgHandled = reinterpret_cast<PCALIBRATERESPONSE>(message)->Handle(results, state);

	#ifdef DEBUG_MESSAGES
		PrintLine((CBYTE)_DriverError);
	#endif

		break;


	case MessageCode::ANGLE_RESPONSE:

		results = new PTR[2] { &_DriverError, &_Degrees };
		msgHandled = reinterpret_cast<PANGLERESPONSE>(message)->Handle(results, state);

	#ifdef DEBUG_MESSAGES
		PrintLine((CBYTE)_DriverError);
		PrintLine(_Degrees);
	#endif

		break;


	case MessageCode::NEWANGLE_RESPONSE:

		results = new PTR[1] { &_DriverError };
		msgHandled = reinterpret_cast<PNEWANGLERESPONSE>(message)->Handle(results);

	#ifdef DEBUG_MESSAGES
		PrintLine((CBYTE)_DriverError);
	#endif

		break;


	case MessageCode::DRIVER_STATUS:

		results = new PTR[3] { &_DriverError, &_DriverStatusMsg, &_DriverStatus };
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

	if (!msgHandled)
		OnError();
}

VOID PrintLCDSplash()
{
	LCD->PrintStringBig_P(F("Fox"), 4, 0);

	delay(LCD_SPLASH_DELAY_1_MS);

	for (BYTE i = 0; i < LCD_SPLASH_SCROLLS; i++)
	{
		LCD->ScrollLeft();
		delay(LCD_SPLASH_SPEED_MS);
	}

	LCD->Clear();

	LCD->LoadCustomChar_P(0x0, LCD_CHAR_FOX);
	LCD->LoadCustomChar_P(0x1, LCD_CHAR_GEM_SMALL);

	LCD->WriteAt(0x0, 0, 0);
	LCD->PrintString_P(F("Foxetron"), 2, 0);
	LCD->WriteAt(LCD_SYMBOL_COPYRIGHT, 11, 0);
	LCD->PrintString_P(F("2016"), 12, 0);
	LCD->WriteAt(0x1, 0, 1);
	LCD->PrintString_P(F("Fancy Fox Gems"), 1, 1);
	LCD->WriteAt(0x1, 15, 1);

	delay(LCD_SPLASH_DELAY_2_MS);

	LCD->Clear();
}

#pragma endregion


#pragma region DEBUG UTILITY FUNCTIONS

#ifdef _DEBUG

#ifdef DEBUG_MEMORY
VOID DEBUG_PrintMemory()
{
	_StatusLed = NOT _StatusLed;
	WritePin(PIN_OUT_STATUS_LED, _StatusLed);
	
	PrintLine();
	PrintString(F("\nRAM: "));
	PrintLine((CWORD)SramFree());
}
#endif	// DEBUG_MEMORY

#ifdef DEBUG_INPUTS
VOID DEBUG_PrintInputs()
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

	_ModeSwitch		= analogRead(PIN_ADC_MODE_SWITCH) > MODE_SWITCH_THRESHOLD;

	_MenuEncoderA	= CheckPinSet(PIN_MENU_ENCODER_A);
	_MenuEncoderB	= CheckPinSet(PIN_MENU_ENCODER_B);

	_SelectButton	= CheckPinUnset(PIN_SELECT_BUTTON);
	_ShiftButton	= CheckPinUnset(PIN_SHIFT_BUTTON);

	PrintLine();


	// REAR INPUTS

	LCD->Clear();

	LCD->Home();

	LCD->MoveCursor(4, 0);
	LCD->print(itoa(_LedButton1, valStr, 2));

	LCD->MoveCursor(6, 0);
	LCD->print(itoa(_LedButton2, valStr, 2));

	LCD->MoveCursor(8, 0);
	LCD->print(itoa(_LedButton3, valStr, 2));

	LCD->MoveCursor(10, 0);
	LCD->print(itoa(_LedButton4, valStr, 2));

	LCD->MoveCursor(12, 0);
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

	LCD->MoveCursor(4, 1);
	LCD->print(itoa(_ModeSwitchVal, valStr, 10));

	LCD->MoveCursor(8, 1);
	LCD->print(itoa(_MenuEncoderA, valStr, 2));

	LCD->MoveCursor(10, 1);
	LCD->print(itoa(_MenuEncoderB, valStr, 2));

	LCD->MoveCursor(12, 1);
	LCD->print(itoa(_SelectButton, valStr, 2));

	LCD->MoveCursor(14, 1);
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
}
#endif	// DEBUG_INPUTS

#ifdef DEBUG_STATE
VOID DEBUG_PrintState()
{
	// TODO: Implement
}
#endif	// DEBUG_STATE

VOID DEBUG_DisplayCustomChars()
{
	LCD->LoadCustomChar_P(0, LCD_CHAR_FOX);
	LCD->LoadCustomChar_P(1, LCD_CHAR_GEM_SMALL);
	LCD->LoadCustomChar_P(2, LCD_CHAR_GEM);

	LCD->LoadCustomChar_P(3, LCD_CHAR_ANGLE);
	LCD->LoadCustomChar_P(4, LCD_CHAR_ANGLE_2);
	LCD->LoadCustomChar_P(5, LCD_CHAR_CHECKMARK);
	LCD->LoadCustomChar_P(6, LCD_CHAR_CLOCKWISE);
	LCD->LoadCustomChar_P(7, LCD_CHAR_NOTES);

	DEBUG_DisplayKeyCodes();


	LCD->LoadCustomChar_P(0, LCD_CHAR_SHAPE_ROUND);
	LCD->LoadCustomChar_P(1, LCD_CHAR_SHAPE_OVAL);
	LCD->LoadCustomChar_P(2, LCD_CHAR_SHAPE_CUSHION);
	LCD->LoadCustomChar_P(3, LCD_CHAR_SHAPE_EMERALD);
	LCD->LoadCustomChar_P(4, LCD_CHAR_SHAPE_BAGUETTE);
	LCD->LoadCustomChar_P(5, LCD_CHAR_SHAPE_TRILLIANT);
	LCD->LoadCustomChar_P(6, LCD_CHAR_SHAPE_TRIANGLE);
	LCD->LoadCustomChar_P(7, LCD_CHAR_SHAPE_SPECIAL);

	DEBUG_DisplayKeyCodes();


	LCD->LoadCustomChar_P(0, LCD_CHAR_CUT_BRILLIANT);
	LCD->LoadCustomChar_P(1, LCD_CHAR_CUT_STEP);

	LCD->LoadCustomChar_P(2, LCD_CHAR_ARROW_UP);
	LCD->LoadCustomChar_P(3, LCD_CHAR_ARROW_DOWN);
	LCD->LoadCustomChar_P(4, LCD_CHAR_ARROW_LEFT_LARGE);
	LCD->LoadCustomChar_P(5, LCD_CHAR_ARROW_RIGHT_LARGE);

	DEBUG_DisplayKeyCodes();
}

#define LCD_CHAR_ROWS_TO_DISPLAY	1

VOID DEBUG_DisplayKeyCodes()
{
	uint8_t i = 0;

	while (i < LCD_CHAR_ROWS_TO_DISPLAY)
	{
		LCD->Clear();

		LCD->print(F("Codes 0x"));

		LCD->print(i, HEX);
		LCD->print(F("-0x"));
		LCD->print(i + 16, HEX);

		LCD->MoveCursor(0, 1);

		for (int j = 0; j < 8; j++)
			LCD->write(i + j);

		i += 16;

		while (!Serial.available()) delay(100);

		Serial.read();
	}
}

#endif	// _DEBUG

#pragma endregion
