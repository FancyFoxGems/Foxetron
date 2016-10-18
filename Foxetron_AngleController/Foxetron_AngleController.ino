/*****************************************************************************************************
* Name:		Foxetron Angle Controller (Firmware Code; Part of the Foxetron Program Suite)
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
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#pragma GCC diagnostic ignored "-Wpointer-arith"


#pragma region INCLUDES

// ITTY BITTY
#include "IttyBitty_info.h"

// PROJECT INCLUDES
#include "Foxetron_pins.h"
#include "Foxetron_LCD_chars.h"

// PROJECT MODULES
#include "Foxetron_messages.h"
#include "Foxetron_LCD.h"
#include "Foxetron_RGB.h"
#include "Foxetron_EEPROM.h"

// PROJECT LIBS
//#include "libs/LiquidCrystal_I2C.custom.h"		// included by other project libs
#include "libs/BigCrystal_I2C.custom.h"
#include "libs/MENWIZ.custom.h"
#include "libs/phi_prompt.custom.h"

// 3RD-PARTY LIBS
//#include "VaRGB.h"							// included by Foxetron_RGB
//#include "VaRGBCurves.h"						// included by Foxetron_RGB

// ARDUINO LIBS
//#include <EEPROM.h>							// included by Foxetron_EEPROM
//#include <Wire.h>								// included by project/3rd-party libs

// ARDUINO CORE
//#include <Arduino.h>							// included by project/3rd-party libs

// AVR LibC
//#include <avr/pgmspace.h>						// included by project 3rd-party libs

#pragma endregion


#pragma region DEFINES

// PROGRAM OPTIONS

#define DEBUG_INPUTS			0
#define SERIAL_ENABLE
#define SERIAL_BAUD_RATE		115200

#ifdef DEBUG_INPUTS && DEBUG_INPUTS != 1
	#undef DEBUG_INPUTS
#endif


// LCD CONSTANTS

#define LCD_I2C_ADDRESS			0x27
#define LCD_CHAR_COLS			16
#define LCD_CHAR_ROWS			2



/* MACROS */

/*
#define READ_PORT(port_Letter, state_variable)			\
	asm volatile("\t"									\
		"push %0"	"\n\t"								\
		"in %0, %1"	"\n"								\
	: "=&r" (current) : "I" (_SFR_IO_ADDR(PINC)) );
*/

#define COUNT(var) (SIZEOF(var) / SIZEOF(0[var]))

#pragma endregion


#pragma region PROGRAM CONSTANTS

// FLASH DATA

#pragma endregion


#pragma region PROGRAM VARIABLES

// EEPROM VARIABLES


// INPUTS

VBOOL _AngleEncoderA	= FALSE;	// Pin 2 / PD2 (INT0)
VBOOL _AngleEncoderB	= FALSE;	// Pin 3 / PD3 (INT1)
//VBOOL _AngleEncoderZ		= 0;	// Pin 4 / PD4 (PCINT20)	- [UNUSED]
//VBOOL _AngleEncoderU		= 0;	// Pin 5 / PD5 (PCINT21)	- [UNUSED]

VBOOL _AngleUp			= FALSE;
VDWORD _AngleReading	= 0;
WORD _AngleDelta		= 0;
WORD _AngleVelocity		= 0;

VBOOL _LedButton1		= FALSE;	// Pin 4 / PD4 (PCINT20)
VBOOL _LedButton2		= FALSE;	// Pin 5 / PD5 (PCINT21)
VBOOL _LedButton3		= FALSE;	// Pin 6 / PD6 (PCINT22)
VBOOL _LedButton4		= FALSE;	// Pin 7 / PD7 (PCINT23)
VBOOL _LedButton5		= FALSE;	// Pin 8 / PB0 (PCINT0)

// [FREE PIN: Pin 12 / PB4 (PCINT4)]

// [FREE PIN: Pin A6 / ADC6]

BOOL _modeSwitch		= FALSE;	// A7 / ADC7
WORD _modeSwitchVal		= 0;

// Menu rotary encoder
VBOOL _MenuEncoderA		= FALSE;	// Pin 14/A0 / PC0 (PCINT8)
VBOOL _MenuEncoderB		= FALSE;	// Pin 15/A1 / PC1 (PCINT9)

BOOL _MenuUp			= FALSE;
DWORD _MenuReading		= 0;
WORD _MenuDelta			= 0;
WORD _MenuVelocity		= 0;

VBOOL _SelectButton		= FALSE;	// Pin 16/A2 / PC2 (PCINT10)
VBOOL _ShiftButton		= FALSE;	// Pin 17/A3 / PC3 (PCINT11)


// OUTPUTS

// LEDs
BYTE _RgbRed			= 0;		// Pin 9 / PB1
BYTE _RgbGreen			= 0;		// Pin 10 / PB2
BYTE _RgbBlue			= 0;		// Pin 11 / PB3

BOOL _StatusLED			= LOW;		// Pin 13 / PB5

#pragma endregion


#pragma region PROGRAM OUTLINE: ENTRY POINT, LOOP, ETC.

VOID setup()
{
	Serial.begin(SERIAL_BAUD_RATE);

	atexit(cleanUp);

	initializePins();
	initializeLCD();
	initializeInterrupts();

	initializeRGB();

	LCD.printBig(F("Fox"), 2, 0);
}

VOID cleanUp()
{
	freeRGB();
}

VOID serialEvent()
{
	while (Serial.available())
	{

	}
}

VOID loop()
{
	RGB.tickAndDelay();

	LCD.clear();
	LCD.home();
	LCD.print(_AngleReading);
	Serial.println(_AngleReading);

#ifdef DEBUG_INPUTS

	DEBUG_printInputValues();

	_StatusLED = !_StatusLED;
	digitalWrite(13, _StatusLED);

	delay(100);

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
}

// CHANNEL A
ISR(INT0_vect)
{
	_ISR_ANGLE_ENCODER_READ_CHANNEL(A, B, == );
}

// CHANNEL B
ISR(INT1_vect)
{
	_ISR_ANGLE_ENCODER_READ_CHANNEL(B, A, != );
}


// PCINT0/PCINT1/PCINT2: LED BUTTONS, MENU ENCODER, and MENU BUTTONS

// PORT B (PCINT0:7): LED BUTTON 5
ISR(PCINT0_vect, ISR_NOBLOCK)
{
	_LedButton5 = PINB && (1 >> 0);
	//_pin12var = PINB && (1 >> 4);
}

// PORT C (PCINT8:14): MENU ENCODER & BUTTONS
ISR(PCINT1_vect, ISR_NOBLOCK)
{
	_MenuEncoderA = PINC && (1 >> 3);
	_MenuEncoderB = PINC && (1 >> 2);
	_SelectButton = PINC && (1 >> 1);
	_ShiftButton = PINC && (1 >> 0);
}

// PORT D (PCINT16:23): LED BUTTONS 1-4
ISR(PCINT2_vect, ISR_NOBLOCK)
{
	_LedButton1 = PIND && (1 >> 4);
	_LedButton2 = PIND && (1 >> 5);
	_LedButton3 = PIND && (1 >> 6);
	_LedButton4 = PIND && (1 >> 7);
}// TIMER EVENTS// TIMER 2 OVERFLOWISR(TIMER2_OVF_vect, ISR_NOBLOCK){}// SERIAL EVENTS// USART RECEIVE/*ISR(USART_RX_vect, ISR_NOBLOCK){}*/#pragma endregion


#pragma region PROGRAM FUNCTIONS

VOID initializeInterrupts()
{
	// External interrupts: Angle encoder
	EIMSK |= 0b00000011;
	EICRA &= 0b11110101;
	EICRA |= 0b00000101;

	// Pin change interrupts: LED buttons, menu encoder, and menu buttons
	PCICR |= 0b00000111;
	PCMSK0 = 0b00010001;
	PCMSK1 = 0b00001111;
	PCMSK2 = 0b11110000;
}

VOID printLCDSplash()
{
	LCD.print(F("Foxetron test..."));

	delay(200);

	for (BYTE i = 0; i < 16; i++)
	{
		LCD.scrollDisplayLeft();
		delay(20);
	}

	LCD.clear();
}

#pragma endregion


#pragma region DEBUG UTILITY FUNCTIONS

// DEBUG UTILITY FUNCTIONS

VOID DEBUG_displayKeyCodes()
{
	uint8_t i = 0;

	while (i < 1)
	{
		LCD.clear();

		LCD.print(F("Codes 0x"));

		LCD.print(i, HEX);
		LCD.print(F("-0x"));
		LCD.print(i + 16, HEX);

		LCD.setCursor(0, 1);

		for (int j = 0; j < 8; j++)
			LCD.write(i + j);

		i += 16;

		while (!Serial.available()) delay(100);
		Serial.read();
	}
}


VOID DEBUG_printInputValues()
{
	STATIC CHAR valStr[5];

	_AngleEncoderA = digitalRead(2);
	_AngleEncoderB = digitalRead(3);

	_LedButton1 = digitalRead(4);
	if (_LedButton1)
	{
		PORTD |= (1 << 4);
		DDRD |= (1 << 4);
		PORTD &= ~(1 << 4);
		DDRD &= ~(1 << 4);
	}

	_LedButton2 = digitalRead(5);
	if (_LedButton2)
	{
		PORTD |= (1 << 5);
		DDRD |= (1 << 5);
		PORTD &= ~(1 << 5);
		DDRD &= ~(1 << 5);
	}

	_LedButton3 = digitalRead(6);
	if (_LedButton3)
	{
		PORTD |= (1 << 6);
		DDRD |= (1 << 6);
		PORTD &= ~(1 << 6);
		DDRD &= ~(1 << 6);
	}

	_LedButton4 = digitalRead(7);
	if (_LedButton4)
	{
		PORTD |= (1 << 7);
		DDRD |= (1 << 7);
		PORTD &= ~(1 << 7);
		DDRD &= ~(1 << 7);
	}

	_LedButton5 = digitalRead(8);
	if (_LedButton5)
	{
		PORTB |= (1 << 0);
		DDRB |= (1 << 0);
		PORTB &= ~(1 << 0);
		DDRB &= ~(1 << 0);
	}

	_modeSwitch = analogRead(7) > 500 ? TRUE : FALSE;

	_MenuEncoderA = digitalRead(14);
	_MenuEncoderB = digitalRead(15);

	_SelectButton = !digitalRead(16);
	_ShiftButton = !digitalRead(17);


	// REAR INPUTS

	LCD.home();

	LCD.setCursor(0, 0);
	LCD.print(itoa(_AngleEncoderA, valStr, 2));

	LCD.setCursor(2, 0);
	LCD.print(itoa(_AngleEncoderB, valStr, 2));

	LCD.setCursor(4, 0);
	LCD.print(itoa(_LedButton1, valStr, 2));

	LCD.setCursor(6, 0);
	LCD.print(itoa(_LedButton2, valStr, 2));

	LCD.setCursor(8, 0);
	LCD.print(itoa(_LedButton3, valStr, 2));

	LCD.setCursor(10, 0);
	LCD.print(itoa(_LedButton4, valStr, 2));

	LCD.setCursor(12, 0);
	LCD.print(itoa(_LedButton5, valStr, 2));


	// FRONT INPUTS

	LCD.setCursor(4, 1);
	LCD.print(itoa(_modeSwitchVal, valStr, 10));

	LCD.setCursor(8, 1);
	LCD.print(itoa(_MenuEncoderA, valStr, 2));

	LCD.setCursor(10, 1);
	LCD.print(itoa(_MenuEncoderB, valStr, 2));

	LCD.setCursor(12, 1);
	LCD.print(itoa(_SelectButton, valStr, 2));

	LCD.setCursor(14, 1);
	LCD.print(itoa(_ShiftButton, valStr, 2));


	Serial.print(itoa(_AngleEncoderA, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_AngleEncoderB, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_LedButton1, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_LedButton2, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_LedButton3, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_LedButton4, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_LedButton5, valStr, 2));
	Serial.println();

	Serial.print(itoa(_modeSwitchVal, valStr, 10));
	Serial.print(F(" / "));
	Serial.print(itoa(_modeSwitch, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_MenuEncoderA, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_MenuEncoderB, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_SelectButton, valStr, 2));
	Serial.print(F(" "));
	Serial.print(itoa(_ShiftButton, valStr, 2));
	Serial.println();

	Serial.println();
}

VOID DEBUG_displayCustomChars()
{
	LCD.createChar(0, LCD_CHAR_FOX);
	LCD.createChar(1, LCD_CHAR_GEM_SMALL);
	LCD.createChar(2, LCD_CHAR_GEM);

	LCD.createChar(3, LCD_CHAR_ANGLE);
	LCD.createChar(4, LCD_CHAR_ANGLE_2);
	LCD.createChar(5, LCD_CHAR_CHECKMARK);
	LCD.createChar(6, LCD_CHAR_CLOCKWISE);
	LCD.createChar(7, LCD_CHAR_NOTES);

	DEBUG_displayKeyCodes();


	LCD.createChar(0, LCD_CHAR_SHAPE_ROUND);
	LCD.createChar(1, LCD_CHAR_SHAPE_OVAL);
	LCD.createChar(2, LCD_CHAR_SHAPE_CUSHION);
	LCD.createChar(3, LCD_CHAR_SHAPE_EMERALD);
	LCD.createChar(4, LCD_CHAR_SHAPE_BAGUETTE);
	LCD.createChar(5, LCD_CHAR_SHAPE_TRILLIANT);
	LCD.createChar(6, LCD_CHAR_SHAPE_TRIANGLE);
	LCD.createChar(7, LCD_CHAR_SHAPE_SPECIAL);

	DEBUG_displayKeyCodes();


	LCD.createChar(0, LCD_CHAR_CUT_BRILLIANT);
	LCD.createChar(1, LCD_CHAR_CUT_STEP);

	LCD.createChar(2, LCD_CHAR_ARROW_UP);
	LCD.createChar(3, LCD_CHAR_ARROW_DOWN);
	LCD.createChar(4, LCD_CHAR_ARROW_LEFT_LARGE);
	LCD.createChar(5, LCD_CHAR_ARROW_RIGHT_LARGE);

	DEBUG_displayKeyCodes();
}

#pragma endregion
