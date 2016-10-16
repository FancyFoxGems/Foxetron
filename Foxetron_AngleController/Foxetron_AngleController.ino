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


/* INCLUDES */

// PROJECT INCLUDES
#include "Foxetron_LCD_chars.h"

// PROJECT LIBS
#include "libs/BigCrystal_I2C.custom.h"
#include "libs/MENWIZ.custom.h"
#include "libs/phi_prompt.custom.h"

// 3RD-PARTY LIBS
#include "VaRGB.h"
#include "VaRGBCurves.h"

// ARDUINO LIBS
#include <EEPROM.h>

// AVR LIBS
//#include <avr/pgmspace.h>



/* DEFINES */

// PROGRAM OPTIONS

#define DEBUG_INPUTS			0
#define SERIAL_ENABLE
#define SERIAL_BAUD_RATE		115200

#ifdef DEBUG_INPUTS
#if DEBUG_INPUTS != 1
#undef DEBUG_INPUTS
#elif !defined(SERIAL_ENABLE)
#define SERIAL_ENABLE
#endif
#endif


// LCD CONSTANTS

#define LCD_I2C_ADDRESS			0x27
#define LCD_CHAR_COLS			16
#define LCD_CHAR_ROWS			2


// INPUT PINS

#define PIN_ANGLE_ENCODER_A		2	// Pin 2 / PD2 (INT0)
#define PIN_ANGLE_ENCODER_B		3	// Pin 3 / PD3 (INT1)
//#define PIN_ANGLE_ENCODER_Z	4	// Pin 4 / PD4 (PCINT20)	- [UNUSED]
//#define PIN_ANGLE_ENCODER_U	5	// Pin 5 / PD5 (PCINT21)	- [UNUSED]

#define PIN_LED_BUTTON_1		4	// Pin 4 / PD4 (PCINT20
#define PIN_LED_BUTTON_2		5	// Pin 5 / PD5 (PCINT21)
#define PIN_LED_BUTTON_3		6	// Pin 6 / PD6 (PCINT22)
#define PIN_LED_BUTTON_4		7	// Pin 7 / PD7 (PCINT23)
#define PIN_LED_BUTTON_5		8	// Pin 8 / PB0 (PCINT0)

#define PIN_12					12	// Pin 12 / PB4 (PCINT4)	- [FREE]

#define PIN_ADC_6				6	// A6 / ADC6				- [FREE]

#define PIN_ADC_MODE_SWITCH 	7	// A7 / ADC7

#define PIN_MENU_ENCODER_A		14	// Pin 14/A0 / PC0 (PCINT8)
#define PIN_MENU_ENCODER_B		15	// Pin 15/A1 / PC1 (PCINT9)

#define PIN_SELECT_BUTTON		16	// Pin 16/A2 / PC2 (PCINT10)
#define PIN_SHIFT_BUTTON		17	// Pin 17/A3 / PC3 (PCINT11)


// OUTPUT PINS

#define PIN_PWM_RGB_LED_RED		9	// Pin 9 / PB1
#define PIN_PWM_RGB_LED_GREEN	10	// Pin 10 / PB2
#define PIN_PWM_RGB_LED_BLUE	11	// Pin 11 / PB3

#define PIN_OUT_STATUS_LED		13	// Pin 13 / PB5



/* MACROS */

#if defined(ARDUINO) && ARDUINO >= 100
#endif

/*
#define READ_PORT(port_Letter, state_variable)			\
	asm volatile("\t"									\
		"push %0"	"\n\t"								\
		"in %0, %1"	"\n"								\
	: "=&r" (current) : "I" (_SFR_IO_ADDR(PINC)) );
*/



/* FLASH DATA */





/* EEPROM VARIABLES */



/* PROGRAM CONSTANTS */



/* GLOBAL VARIABLES */

// INPUTS

volatile bool _AngleEncoderA	= false;	// Pin 2 / PD2 (INT0)
volatile bool _AngleEncoderB	= false;	// Pin 3 / PD3 (INT1)
//volatile bool _AngleEncoderZ		= 0;	// Pin 4 / PD4 (PCINT20)	- [UNUSED]
//volatile bool _AngleEncoderU		= 0;	// Pin 5 / PD5 (PCINT21)	- [UNUSED]

volatile bool _AngleUp			= false;
volatile uint32_t _AngleReading = 0;
word _AngleDelta				= 0;
word _AngleVelocity				= 0;

volatile bool _LedButton1		= false;	// Pin 4 / PD4 (PCINT20)
volatile bool _LedButton2		= false;	// Pin 5 / PD5 (PCINT21)
volatile bool _LedButton3		= false;	// Pin 6 / PD6 (PCINT22)
volatile bool _LedButton4		= false;	// Pin 7 / PD7 (PCINT23)
volatile bool _LedButton5		= false;	// Pin 8 / PB0 (PCINT0)

// [FREE PIN: Pin 12 / PB4 (PCINT4)]

// [FREE PIN: Pin A6 / ADC6]

bool _modeSwitch				= false;	// A7 / ADC7
word _modeSwitchVal				= 0;

// Menu rotary encoder
volatile bool _MenuEncoderA		= false;	// Pin 14/A0 / PC0 (PCINT8)
volatile bool _MenuEncoderB		= false;	// Pin 15/A1 / PC1 (PCINT9)

bool _MenuUp					= false;
uint32_t _MenuReading			= 0;
word _MenuDelta					= 0;
word _MenuVelocity				= 0;

volatile bool _SelectButton		= false;	// Pin 16/A2 / PC2 (PCINT10)
volatile bool _ShiftButton		= false;	// Pin 17/A3 / PC3 (PCINT11)


// OUTPUTS

// LEDs
byte _RgbRed					= 0;		// Pin 9 / PB1
byte _RgbGreen					= 0;		// Pin 10 / PB2
byte _RgbBlue					= 0;		// Pin 11 / PB3

bool _StatusLED					= LOW;		// Pin 13 / PB5


// LCD display
BigCrystal_I2C LCD(LCD_I2C_ADDRESS, LCD_CHAR_COLS, LCD_CHAR_ROWS);	// Pin A4/A5 (I2C)


// RGB LED / VaRGB CONFIGURATION

using namespace vargb;

void _RGB_callback_SetColor(ColorSettings * colors);
void _RGB_callback_ScheduleComplete(Schedule * schedule);

VaRGB RGB(_RGB_callback_SetColor, _RGB_callback_ScheduleComplete);

Schedule * _RgbSchedule = new Schedule();

Curve::Flasher * _RgbCurveFlasher = new Curve::Flasher(VaRGB_COLOR_MAXVALUE, VaRGB_COLOR_MAXVALUE, VaRGB_COLOR_MAXVALUE, 6, 20);

Curve::Sine * _RgbCurveSine = new Curve::Sine(500, VaRGB_COLOR_MAXVALUE, 500, 6, 2);

Curve::Linear * _RgbCurves[] = {
	// start black
	new Curve::Linear(0, 0, 0, 0),
	// go to ~1/2 red, over one 5 seconds
	new Curve::Linear(500, 0, 0, 5),
	// go to red+blue, over 2s
	new Curve::Linear(1000, 0, 1000, 2),
	// back down to ~1/2 red over 2s
	new Curve::Linear(500, 0, 0, 2),
	// fade to black for 5s
	new Curve::Linear(0, 0, 0, 5),
};



/* PROGRAM CODE */

// PROGRAM OUTLINE: ENTRY POINT & LOOP

void setup()
{
#ifdef SERIAL_ENABLE
	Serial.begin(SERIAL_BAUD_RATE);
#endif

	initializePins();
	initializeLCD();
	initializeInterrupts();

	LCD.printBig(F("Fox"), 2, 0);

	_RgbSchedule->addTransition(_RgbCurveFlasher);
	_RgbSchedule->addTransition(_RgbCurveSine);

	for (uint8_t i = 0; i < 5; i++)
		_RgbSchedule->addTransition(_RgbCurves[i]);

	RGB.setSchedule(_RgbSchedule);
}

void loop()
{
	RGB.tickAndDelay();
	LCD.clear();
	LCD.home();
	LCD.print(_AngleReading);
	Serial.println(_AngleReading);

#ifdef DEBUG_INPUTS

	_DEBUG_printInputValues();

	_StatusLED = !_StatusLED;
	digitalWrite(13, _StatusLED);

	delay(100);

#endif
}


/* INTERRUPT VECTORS and SUPPORTING MACROS & INLINED FUNCTIONS */

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
}// TIMER EVENTS// TIMER 2 OVERFLOWISR(TIMER2_OVF_vect, ISR_NOBLOCK){}// SERIAL EVENTS// USART RECEIVE/*ISR(USART_RX_vect, ISR_NOBLOCK){}*/

// PROGRAM FUNCTIONS

void initializePins()
{
	// REAR PINS

	// [Pins 0 (RX) & 1 (TX): USART serial communication --[angle feedback]--> AVR uC stepper motor angle controller]

	// Angle encoder
	pinMode(PIN_ANGLE_ENCODER_A, INPUT);			// Channel A
	pinMode(PIN_ANGLE_ENCODER_B, INPUT);			// Channel B

	// LED buttons
	pinMode(PIN_LED_BUTTON_1, INPUT);				// LED button #1
	pinMode(PIN_LED_BUTTON_2, INPUT);				// LED button #2
	pinMode(PIN_LED_BUTTON_3, INPUT);				// LED button #3
	pinMode(PIN_LED_BUTTON_4, INPUT);				// LED button #4
	pinMode(PIN_LED_BUTTON_5, INPUT);				// LED button #5

	// [Pins 9-11: RGB LED - 9: red / 10: green / 11: blue]
	pinMode(PIN_PWM_RGB_LED_RED, OUTPUT);
	pinMode(PIN_PWM_RGB_LED_GREEN, OUTPUT);
	pinMode(PIN_PWM_RGB_LED_BLUE, OUTPUT);

	// [Pin 12 (PB4); FREE]
	pinMode(PIN_12, INPUT_PULLUP);


	// FRONT PINS

	pinMode(PIN_OUT_STATUS_LED, OUTPUT);			// Status LED

	// Menu encoder
	pinMode(PIN_MENU_ENCODER_A, INPUT_PULLUP);		// Channel A
	pinMode(PIN_MENU_ENCODER_B, INPUT_PULLUP);		// Channel B

	// Menu buttons
	pinMode(PIN_SELECT_BUTTON, INPUT_PULLUP);		// Select button
	pinMode(PIN_SHIFT_BUTTON, INPUT_PULLUP);		// Shift button

	// [Pin A7 (ADC6; analog only): Mode switch
	pinMode(PIN_ADC_MODE_SWITCH, INPUT_PULLUP);

	// [Pin A6 (ADC6; analog only); FREE]
	pinMode(PIN_ADC_6, INPUT_PULLUP);

	// [Pins 18 & 19 - SDA: 18/A4 & SCL: 19/A5 | I2C --> PCF8574T port expander --> RGB HD44780]
}

void initializeLCD()
{
	LCD.init();
	LCD.backlight();
	LCD.home();

	// Load large font
	uint8_t customChar[8];
	for (uint8_t i = 0; i < 8; i++)
	{
		for (uint8_t j = 0; j < 8; j++)
			customChar[j] = pgm_read_byte_near(BF_fontShapes + (i * 8) + j);
		LCD.createChar(i, customChar);
	}
}

void initializeInterrupts()
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


// VaRGB CALLBACK FUNCTIONS

void _RGB_callback_SetColor(ColorSettings * colors)
{
	analogWrite(PIN_PWM_RGB_LED_RED, colors->red);
	analogWrite(PIN_PWM_RGB_LED_GREEN, colors->green);
	analogWrite(PIN_PWM_RGB_LED_BLUE, colors->blue);
}

void _RGB_callback_ScheduleComplete(Schedule * schedule)
{
	RGB.resetTicks();
	RGB.setSchedule(schedule);
}


// DEBUG UTILITY FUNCTIONS

void _DEBUG_printLCDSplash()
{
	LCD.print(F("Foxetron test..."));

	delay(200);

	for (byte i = 0; i < 16; i++)
	{
		LCD.scrollDisplayLeft();
		delay(20);
	}

	LCD.clear();
}

void _DEBUG_displayKeyCodes(void)
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


void _DEBUG_printInputValues()
{
	static char valStr[5];

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

	_modeSwitch = analogRead(7) > 500 ? true : false;

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

void _DEBUG_createCustomChars()
{
	LCD.createChar(0, LCD_CHAR_FOX);
	LCD.createChar(1, LCD_CHAR_GEM_SMALL);
	LCD.createChar(2, LCD_CHAR_GEM);

	LCD.createChar(3, LCD_CHAR_ANGLE);
	LCD.createChar(4, LCD_CHAR_ANGLE_2);
	LCD.createChar(5, LCD_CHAR_CHECKMARK);
	LCD.createChar(6, LCD_CHAR_CLOCKWISE);
	LCD.createChar(7, LCD_CHAR_NOTES);

	_DEBUG_displayKeyCodes();


	LCD.createChar(0, LCD_CHAR_SHAPE_ROUND);
	LCD.createChar(1, LCD_CHAR_SHAPE_OVAL);
	LCD.createChar(2, LCD_CHAR_SHAPE_CUSHION);
	LCD.createChar(3, LCD_CHAR_SHAPE_EMERALD);
	LCD.createChar(4, LCD_CHAR_SHAPE_BAGUETTE);
	LCD.createChar(5, LCD_CHAR_SHAPE_TRILLIANT);
	LCD.createChar(6, LCD_CHAR_SHAPE_TRIANGLE);
	LCD.createChar(7, LCD_CHAR_SHAPE_SPECIAL);

	_DEBUG_displayKeyCodes();


	LCD.createChar(0, LCD_CHAR_CUT_BRILLIANT);
	LCD.createChar(1, LCD_CHAR_CUT_STEP);

	LCD.createChar(2, LCD_CHAR_ARROW_UP);
	LCD.createChar(3, LCD_CHAR_ARROW_DOWN);
	LCD.createChar(4, LCD_CHAR_ARROW_LEFT_LARGE);
	LCD.createChar(5, LCD_CHAR_ARROW_RIGHT_LARGE);

	_DEBUG_displayKeyCodes();
}
