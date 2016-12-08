/***********************************************************************************************
* [Foxetron_AngleController_pins.h]: PIN DEFINITIONS FOR THE FOXETRON ANGLE CONTROLLER
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_PINS_H
#define _FOXETRON_PINS_H


#pragma region INCLUDES

//  ITTY BITTY
#include "IttyBitty_gpio.h"

using namespace IttyBitty;


#pragma endregion


#pragma region DEFINES

// INPUT PINS

//#define PIN_ANGLE_ENCODER_A	2	// Pin 2 / PD2 (INT0)	- [UNUSED]
//#define PIN_ANGLE_ENCODER_B	3	// Pin 3 / PD3 (INT1)	- [UNUSED]
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

#pragma endregion


#pragma region PROGRAM FUNCTIONS

INLINE VOID InitializePins()
{
	// REAR PINS

	// [Pins 0 (RX) & 1 (TX): USART serial communication --[angle feedback]--> AVR uC stepper motor angle controller]

	// LED buttons
	SetPinMode(PIN_LED_BUTTON_1, INPUT);				// LED button #1
	SetPinMode(PIN_LED_BUTTON_2, INPUT);				// LED button #2
	SetPinMode(PIN_LED_BUTTON_3, INPUT);				// LED button #3
	SetPinMode(PIN_LED_BUTTON_4, INPUT);				// LED button #4
	SetPinMode(PIN_LED_BUTTON_5, INPUT);				// LED button #5

	// [Pins 9-11: RGB LED - 9: red / 10: green / 11: blue]
	SetPinMode(PIN_PWM_RGB_LED_RED, OUTPUT);
	SetPinMode(PIN_PWM_RGB_LED_GREEN, OUTPUT);
	SetPinMode(PIN_PWM_RGB_LED_BLUE, OUTPUT);

	// [Pin 12 (PB4); FREE]
	SetPinMode(PIN_12, INPUT_PULLUP);


	// FRONT PINS

	SetPinMode(PIN_OUT_STATUS_LED, OUTPUT);			// Status LED

	// Menu encoder
	SetPinMode(PIN_MENU_ENCODER_A, INPUT_PULLUP);		// Channel A
	SetPinMode(PIN_MENU_ENCODER_B, INPUT_PULLUP);		// Channel B

	// Menu buttons
	SetPinMode(PIN_SELECT_BUTTON, INPUT_PULLUP);		// Select button
	SetPinMode(PIN_SHIFT_BUTTON, INPUT_PULLUP);		// Shift button

	// [Pin A7 (ADC6; analog only): Mode switch
	SetPinMode(PIN_ADC_MODE_SWITCH, INPUT_PULLUP);

	// [Pin A6 (ADC6; analog only); FREE]
	SetPinMode(PIN_ADC_6, INPUT_PULLUP);

	// [Pins 18 & 19 - SDA: 18/A4 & SCL: 19/A5 | I2C --> PCF8574T port expander --> RGB HD44780]
}

#pragma endregion


#endif
