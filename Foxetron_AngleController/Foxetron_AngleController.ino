/* INCLUDES */

// AVR LIBS
#include <avr/pgmspace.h>

// ARDUINO LIBS

// 3RD-PARTY LIBS
#include "BigCrystal_I2C.h"
#include "VaRGB.h"
#include "VaRGBCurves.h"



/* DEFINES */

// PROGRAM OPTIONS

#define DEBUG_INPUTS			0
#define SERIAL_ENABLE
#define SERIAL_BAUD_RATE		115200

#if defined(DEBUG_INPUTS)
	#if DEBUG_INPUTS != 1
		#undef DEBUG_INPUTS
	#elif !defined(SERIAL_ENABLE)
		#define SERIAL_ENABLE
	#endif
#endif


// LCD SYMBOL CHARACTERS

// ARROWS
#define LCD_SYMBOL_SCROLL_RIGHT			0x10
#define LCD_SYMBOL_SCROLL_LEFT			0x11
#define LCD_SYMBOL_DOUBLE_ARROW_UP		0x15
#define LCD_SYMBOL_DOUBLE_ARROW_DOWN	0x16
#define LCD_SYMBOL_SPLIT_CIRCLE			0x17
#define LCD_SYMBOL_ARROW_RETURN			0x18
#define LCD_SYMBOL_ARROW_UP				0x19
#define LCD_SYMBOL_ARROW_DOWN			0x1A
#define LCD_SYMBOL_ARROW_RIGHT			0x1A
#define LCD_SYMBOL_ARROW_LEFT			0x1B
#define LCD_SYMBOL_SCROLL_UP			0x1E
#define LCD_SYMBOL_SCROLL_DOWN			0x1F


#define LCD_SYMBOL_HOUSE				0x7f
#define LCD_SYMBOL_NOTE					0x91
#define LCD_SYMBOL_BELL					0x98
#define LCD_SYMBOL_HEART				0x9d
#define LCD_SYMBOL_COPYRIGHT			0xa9
#define LCD_SYMBOL_RESERVED				0xae


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
#define READ_PORT(port_letter, state_variable)			\
	asm volatile("\t"									\
		"push %0"	"\n\t"								\
		"in %0, %1"	"\n"								\
	: "=&r" (current) : "I" (_SFR_IO_ADDR(PINC)) );
*/



/* FLASH DATA */

// CUSTOM LCD CHARACTERS

// Miscellaneous icons
PROGMEM const char LCD_CHAR_FOX[8]					= { 0x11, 0x1b, 0x1f, 0x11, 0x1b, 0x11, 0xe, 0x4 };
PROGMEM const char LCD_CHAR_GEM[8]					= { 0x0, 0x0, 0xe, 0x1b, 0x15, 0xa, 0x4, 0x0 };
PROGMEM const char LCD_CHAR_GEM_SMALL[8]			= { 0x0, 0x0, 0x0, 0xe, 0x11, 0xa, 0x4, 0x0 };

// Angle symbols
PROGMEM const char LCD_CHAR_ANGLE[8]				= { 0x1, 0x3, 0x6, 0xc, 0x1a, 0x12, 0x1f, 0x0 };
PROGMEM const char LCD_CHAR_ANGLE_2[8]				= { 0x0, 0x1, 0x2, 0x4, 0xa, 0x12, 0x1f, 0x0 };
PROGMEM const char LCD_CHAR_CLOCKWISE[8]			= { 0x0, 0xe, 0x1b, 0x11, 0x5, 0xf, 0x4, 0x0 };
PROGMEM const char LCD_CHAR_COUNTER_CCW[8]			= { 0x0, 0xe, 0x1b, 0x11, 0x14, 0x1e, 0x4, 0x0 };

// Miscellaneous symbols
PROGMEM const char LCD_CHAR_CHECKMARK[8]			= { 0x0, 0x0, 0x1, 0x3, 0x16, 0x1c, 0x8,0x0 };
PROGMEM const char LCD_CHAR_BOX_EMPTY[8]			= { 0x0, 0x1f, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1f };
PROGMEM const char LCD_CHAR_BOX_CHECKMARK[8]		= { 0x0, 0x1f, 0x11, 0x13, 0x1f, 0x15, 0x11, 0x1f };
PROGMEM const char LCD_CHAR_WARNING[8]				= { 0x0, 0xe, 0x1b, 0x1b, 0x1f, 0x1b, 0xe, 0x0 };
PROGMEM const char LCD_CHAR_NOT_ALLOWED[8]			= { 0xe, 0x1f, 0x19, 0x1d, 0x17, 0x13, 0x1f, 0xe };
PROGMEM const char LCD_CHAR_LIGHTNING[8]			= { 0x3, 0x6, 0xc, 0xf, 0x3, 0x6, 0xc, 0x8 };
PROGMEM const char LCD_CHAR_NOTES[8]				= { 0x3, 0x7, 0xd, 0x9, 0x9, 0xb, 0x1b, 0x18 };
PROGMEM const char LCD_CHAR_CLOCK_A[8]				= { 0x7, 0xc, 0xa, 0x9, 0x8, 0x8, 0x7, 0x0 };
PROGMEM const char LCD_CHAR_CLOCK_B[8]				= { 0x1c, 0x2, 0x2, 0x1a, 0x2, 0x2, 0x1c, 0x0 };
PROGMEM const char LCD_CHAR_HOURGLASS_1[8]			= { 0x1f, 0x1f, 0xe, 0x4, 0xa, 0x11, 0x11, 0x1f };
PROGMEM const char LCD_CHAR_HOURGLASS_2[8]			= { 0x1f, 0x11, 0xe, 0x4, 0xa, 0x11, 0x1f, 0x1f };
PROGMEM const char LCD_CHAR_HOURGLASS_3[8]			= { 0x1f, 0x11, 0xa, 0x4, 0xa, 0x1f, 0x1f, 0x1f };
PROGMEM const char LCD_CHAR_HOURGLASS_4[8]			= { 0x1f, 0x11, 0xa, 0x4, 0xe, 0x1f, 0x1f, 0x1f };

// Gem cut icons
PROGMEM const char LCD_CHAR_CUT_BRILLIANT[8]		= { 0x0, 0xe, 0x15, 0x1b, 0x15, 0xa, 0x4, 0x0 };
PROGMEM const char LCD_CHAR_CUT_STEP[8]				= { 0x0, 0xe, 0x11, 0x1f, 0x11, 0x1f, 0xa, 0x4 };

// Gem shape (outline) icons
PROGMEM const char LCD_CHAR_SHAPE_ROUND[8]			= { 0x0, 0x0, 0xe, 0x1b, 0x11, 0x1b, 0xe, 0x0 };
PROGMEM const char LCD_CHAR_SHAPE_OVAL[8]			= { 0x0, 0xe, 0x1b, 0x11, 0x11, 0x1b, 0xe, 0x0 };
PROGMEM const char LCD_CHAR_SHAPE_CUSHION[8]		= { 0x0, 0x6, 0xf, 0x19, 0x13, 0x1e, 0xc, 0x0 };
PROGMEM const char LCD_CHAR_SHAPE_SQUARE[8]			= { 0x0, 0x1f, 0x11, 0x11, 0x11, 0x11, 0x1f, 0x0 };
PROGMEM const char LCD_CHAR_SHAPE_BAGUETTE[8]		= { 0x0, 0xe, 0xa, 0xa, 0xa, 0xa, 0xe, 0x0 };
PROGMEM const char LCD_CHAR_SHAPE_EMERALD[8]		= { 0xe, 0x1b, 0x11, 0x11, 0x11, 0x11, 0x1b, 0xe };
PROGMEM const char LCD_CHAR_SHAPE_TRILLIANT[8]		= { 0x0, 0x4, 0xe, 0x1b, 0x11, 0x1b, 0xe, 0x0 };
PROGMEM const char LCD_CHAR_SHAPE_TRIANGLE[8]		= { 0x0, 0x4, 0xe, 0xa, 0x1b, 0x11, 0x1f, 0x0 };
PROGMEM const char LCD_CHAR_SHAPE_SPECIAL[8]		= { 0x0, 0x0, 0xa, 0x15, 0x11, 0xa, 0x4, 0x0 };

// Small arrow icons
PROGMEM const char LCD_CHAR_ARROW_UP_SMALL[8]		= { 0x0, 0x0, 0x4, 0xe, 0x4, 0x4, 0x0, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_DOWN_SMALL[8]		= { 0x0, 0x0, 0x4, 0x4, 0xe, 0x4, 0x0, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_LEFT_SMALL[8]		= { 0x0, 0x0, 0x0, 0x8, 0x1e, 0x8, 0x0, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_RIGHT_SMALL[8]	= { 0x0, 0x0, 0x0, 0x2, 0xf, 0x2, 0x0, 0x0 };

// Medium arrow icons
PROGMEM const char LCD_CHAR_ARROW_UP[8]				= { 0x0, 0x4, 0xe, 0x15, 0x4, 0x4, 0x4, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_DOWN[8]			= { 0x0, 0x4, 0x4, 0x4, 0x15, 0xe, 0x4, 0x0 };

// Large arrow icons
PROGMEM const char LCD_CHAR_ARROW_LEFT_LARGE[8]		= { 0x0, 0x0, 0x4, 0xc, 0x1f, 0xc, 0x4, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_RIGHT_LARGE[8]	= { 0x0, 0x0, 0x4, 0x6, 0x1f, 0x6, 0x4, 0x0 };

// Scrollbar icons
PROGMEM const char LCD_CHAR_SCROLLBAR_TOP[8]		= { 0x1f, 0x1f, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
PROGMEM const char LCD_CHAR_SCROLLBAR_BOTTOM[8]		= { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0x1f };
PROGMEM const char LCD_CHAR_SCROLLBAR_1[8]			= { 0x1f, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
PROGMEM const char LCD_CHAR_SCROLLBAR_2[8]			= { 0x0, 0x0, 0x1f, 0x0, 0x0, 0x0, 0x0, 0x0 };
PROGMEM const char LCD_CHAR_SCROLLBAR_3[8]			= { 0x0, 0x0, 0x0, 0x0, 0x1f, 0x0, 0x0, 0x0 };
PROGMEM const char LCD_CHAR_SCROLLBAR_4[8]			= { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0x0 };

// Bar graph icons
const char * const &  LCD_CHAR_BARGRAPH_EMPTY		= LCD_CHAR_SCROLLBAR_3;
PROGMEM const char LCD_CHAR_BARGRAPH_FULL[8]		= { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f };
PROGMEM const char LCD_CHAR_BARGRAPH_SEMI_FULL_1[8]	= { 0x15, 0xa, 0x15, 0xa, 0x15, 0xa, 0x15, 0xa };
PROGMEM const char LCD_CHAR_BARGRAPH_SEMI_FULL_2[8]	= { 0xa, 0x15, 0xa, 0x15, 0xa, 0x15, 0xa, 0x15 };
PROGMEM const char LCD_CHAR_BARGRAPH_1[8]			= { 0x10, 0x10, 0x10, 0x10, 0x1f, 0x10, 0x10, 0x10 };
PROGMEM const char LCD_CHAR_BARGRAPH_2[8]			= { 0x18, 0x18, 0x18, 0x18, 0x1f, 0x18, 0x18, 0x18 };
PROGMEM const char LCD_CHAR_BARGRAPH_3[8]			= { 0x1c, 0x1c, 0x1c, 0x1c, 0x1f, 0x1c, 0x1c, 0x1c };
PROGMEM const char LCD_CHAR_BARGRAPH_4[8]			= { 0x1e, 0x1e, 0x1e, 0x1e, 0x1f, 0x1e, 0x1e, 0x1e };



/* EEPROM VARIABLES */



/* PROGRAM CONSTANTS */



/* GLOBAL VARIABLES */

// INPUTS

volatile bool _AngleControllerA	= 0;	// Pin 2 / PD2 (INT0)
volatile bool _AngleControllerB	= 0;	// Pin 3 / PD3 (INT1)
//volatile bool _AngleControllerZ	= 0;	// Pin 4 / PD4 (PCINT20)	- [UNUSED]
//volatile bool _AngleControllerU	= 0;	// Pin 5 / PD5 (PCINT21)	- [UNUSED]

volatile bool _angleUp			= 0;
volatile uint32_t _angleReading	= 0;
word _angleDelta				= 0;
word _angleVelocity				= 0;

volatile bool _ledButton1		= 0;	// Pin 4 / PD4 (PCINT20)
volatile bool _ledButton2		= 0;	// Pin 5 / PD5 (PCINT21)
volatile bool _ledButton3		= 0;	// Pin 6 / PD6 (PCINT22)
volatile bool _ledButton4		= 0;	// Pin 7 / PD7 (PCINT23)
volatile bool _ledButton5		= 0;	// Pin 8 / PB0 (PCINT0)

// [FREE PIN: Pin 12 / PB4 (PCINT4)]

// [FREE PIN: Pin A6 / ADC6]

bool _modeSwitch				= 0;	// A7 / ADC7
word _modeSwitchVal				= 0;

volatile bool _menuEncoderA		= 0;	// Pin 14/A0 / PC0 (PCINT8)
volatile bool _menuEncoderB		= 0;	// Pin 15/A1 / PC1 (PCINT9)

bool _menuUp					= 0;
uint32_t _menuReading			= 0;
word _menuDelta					= 0;
word _menuVelocity				= 0;

volatile bool _selectButton		= 0;	// Pin 16/A2 / PC2 (PCINT10)
volatile bool _shiftButton		= 0;	// Pin 17/A3 / PC3 (PCINT11)


// OUTPUTS

byte _rgbRed					= 0;	// Pin 9 / PB1
byte _rgbGreen					= 0;	// Pin 10 / PB2
byte _rgbBlue					= 0;	// Pin 11 / PB3

bool _statusLED					= 0;	// Pin 13 / PB5

BigCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_CHAR_COLS, LCD_CHAR_ROWS);	// Pin A4/A5 (I2C)


// VaRGB CONFIGURATION

using namespace vargb;

void _RGB_callback_setColor(ColorSettings * colors);
void _RGB_callback_scheduleComplete(Schedule * schedule);
VaRGB _vaRGB(_RGB_callback_setColor, _RGB_callback_scheduleComplete);

Schedule * _rgbSchedule =  new Schedule();

Curve::Flasher * _rgbCurveFlasher = new Curve::Flasher(VaRGB_COLOR_MAXVALUE, VaRGB_COLOR_MAXVALUE, VaRGB_COLOR_MAXVALUE, 6, 20);

Curve::Sine * _rgbCurveSine = new Curve::Sine(500, VaRGB_COLOR_MAXVALUE, 500, 6, 2);

Curve::Linear * _rgbCurves[] = {
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

	lcd.printBig("Fox", 2, 0);

	_rgbSchedule->addTransition(_rgbCurveFlasher);
	_rgbSchedule->addTransition(_rgbCurveSine);

	for (uint8_t i = 0; i < 5; i++)
		_rgbSchedule->addTransition(_rgbCurves[i]);

	_vaRGB.setSchedule(_rgbSchedule);
}

void loop()
{
	_vaRGB.tickAndDelay();
	lcd.clear();
	lcd.home();
	lcd.print(_angleReading);
	Serial.println(_angleReading);

#ifdef DEBUG_INPUTS

	_DEBUG_printInputValues();

	_statusLED = !_statusLED;
	digitalWrite(13, _statusLED);

	delay(100);

#endif
}


/* INTERRUPT VECTORS and SUPPORTING MACROS & INLINED FUNCTIONS */

// INT0/INT1: ANGLE ENCODER

#define _ISR_ANGLE_ENCODER_READ_CHANNEL(channel, other_channel, increment_comparison)					\
	_AngleController ## channel = !_AngleController ## channel;												\
	_angleUp = (_AngleController ## channel increment_comparison _AngleController ## other_channel);			\
	_ISR_AngleController_updateAngleReading();

static inline void _ISR_AngleController_updateAngleReading() __attribute__((always_inline));

static inline void _ISR_AngleController_updateAngleReading()
{
	if (_angleUp)
		++_angleReading;
	else
		--_angleReading;
}

// CHANNEL A
ISR(INT0_vect)
{
	_ISR_ANGLE_ENCODER_READ_CHANNEL(A, B, ==);
}

// CHANNEL B
ISR(INT1_vect)
{
	_ISR_ANGLE_ENCODER_READ_CHANNEL(B, A, !=);
}


// PCINT0/PCINT1/PCINT2: LED BUTTONS, MENU ENCODER, and MENU BUTTONS

// PORT B (PCINT0:7): LED BUTTON 5
ISR(PCINT0_vect, ISR_NOBLOCK)
{
	_ledButton5 = PINB && (1 >> 0);
	//_pin12var = PINB && (1 >> 4);
}

// PORT C (PCINT8:14): MENU ENCODER & BUTTONS
ISR(PCINT1_vect, ISR_NOBLOCK)
{
	_menuEncoderA = PINC && (1 >> 3);
	_menuEncoderB = PINC && (1 >> 2);
	_selectButton = PINC && (1 >> 1);
	_shiftButton = PINC && (1 >> 0);
}

// PORT D (PCINT16:23): LED BUTTONS 1-4
ISR(PCINT2_vect, ISR_NOBLOCK)
{
	_ledButton1 = PIND && (1 >> 4);
	_ledButton2 = PIND && (1 >> 5);
	_ledButton3 = PIND && (1 >> 6);
	_ledButton4 = PIND && (1 >> 7);
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
	lcd.init();
	lcd.backlight();
	lcd.home();

	// Load large font
	uint8_t customChar[8];
	for (uint8_t i = 0; i < 8; i++)
	{
		for (uint8_t j = 0; j < 8; j++)
			customChar[j] = pgm_read_byte(BF_fontShapes + (i * 8) + j);
		lcd.createChar(i, customChar);
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

void _RGB_callback_setColor(ColorSettings * colors)
{
	analogWrite(PIN_PWM_RGB_LED_RED, colors->red);
	analogWrite(PIN_PWM_RGB_LED_GREEN, colors->green);
	analogWrite(PIN_PWM_RGB_LED_BLUE, colors->blue);
}

void _RGB_callback_scheduleComplete(Schedule * schedule)
{
  _vaRGB.resetTicks();
  _vaRGB.setSchedule(schedule);
}


// DEBUG UTILITY FUNCTIONS

void _DEBUG_printLCDSplash()
{
	lcd.print("Foxetron test...");

	delay(200);

	for (byte i = 0; i < 16; i++)
	{
		lcd.scrollDisplayLeft();
		delay(20);
	}

	lcd.clear();
}

void _DEBUG_displayKeyCodes(void)
{
	uint8_t i = 0;

	while (i < 1)
	{
		lcd.clear();

		lcd.print("Codes 0x");

		lcd.print(i, HEX);
		lcd.print("-0x");
		lcd.print(i + 16, HEX);

		lcd.setCursor(0, 1);

		for (int j = 0; j < 8; j++)
		lcd.write(i + j);

		i+=16;

		while (!Serial.available()) delay(100);
		Serial.read();
	}
}


void _DEBUG_printInputValues()
{
	static char valStr[5];

	_AngleControllerA = digitalRead(2);
	_AngleControllerB = digitalRead(3);

	_ledButton1 = digitalRead(4);
	if (_ledButton1)
	{
		PORTD |= (1 << 4);
		DDRD |= (1 << 4);
		PORTD &= ~(1 << 4);
		DDRD &= ~(1 << 4);
	}

	_ledButton2 = digitalRead(5);
	if (_ledButton2)
	{
		PORTD |= (1 << 5);
		DDRD |= (1 << 5);
		PORTD &= ~(1 << 5);
		DDRD &= ~(1 << 5);
	}

	_ledButton3 = digitalRead(6);
	if (_ledButton3)
	{
		PORTD |= (1 << 6);
		DDRD |= (1 << 6);
		PORTD &= ~(1 << 6);
		DDRD &= ~(1 << 6);
	}

	_ledButton4 = digitalRead(7);
	if (_ledButton4)
	{
		PORTD |= (1 << 7);
		DDRD |= (1 << 7);
		PORTD &= ~(1 << 7);
		DDRD &= ~(1 << 7);
	}

	_ledButton5 = digitalRead(8);
	if (_ledButton5)
	{
		PORTB |= (1 << 0);
		DDRB |= (1 << 0);
		PORTB &= ~(1 << 0);
		DDRB &= ~(1 << 0);
	}

	_modeSwitch = analogRead(7) > 500 ? true : false;

	_menuEncoderA = digitalRead(14);
	_menuEncoderB = digitalRead(15);

	_selectButton = !digitalRead(16);
	_shiftButton = !digitalRead(17);


	// Rear inputs

	lcd.home();

	lcd.setCursor(0, 0);
	lcd.print(itoa(_AngleControllerA, valStr, 2));

	lcd.setCursor(2, 0);
	lcd.print(itoa(_AngleControllerB, valStr, 2));

	lcd.setCursor(4, 0);
	lcd.print(itoa(_ledButton1, valStr, 2));

	lcd.setCursor(6, 0);
	lcd.print(itoa(_ledButton2, valStr, 2));

	lcd.setCursor(8, 0);
	lcd.print(itoa(_ledButton3, valStr, 2));

	lcd.setCursor(10, 0);
	lcd.print(itoa(_ledButton4, valStr, 2));

	lcd.setCursor(12, 0);
	lcd.print(itoa(_ledButton5, valStr, 2));


	// Front inputs

	lcd.setCursor(4, 1);
	lcd.print(itoa(_modeSwitchVal, valStr, 10));

	lcd.setCursor(8, 1);
	lcd.print(itoa(_menuEncoderA, valStr, 2));

	lcd.setCursor(10, 1);
	lcd.print(itoa(_menuEncoderB, valStr, 2));

	lcd.setCursor(12, 1);
	lcd.print(itoa(_selectButton, valStr, 2));

	lcd.setCursor(14, 1);
	lcd.print(itoa(_shiftButton, valStr, 2));


	Serial.print(itoa(_AngleControllerA, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(_AngleControllerB, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(_ledButton1, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(_ledButton2, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(_ledButton3, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(_ledButton4, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(_ledButton5, valStr, 2));
	Serial.println();

	Serial.print(itoa(_modeSwitchVal, valStr, 10));
	Serial.print(" / ");
	Serial.print(itoa(_modeSwitch, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(_menuEncoderA, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(_menuEncoderB, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(_selectButton, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(_shiftButton, valStr, 2));
	Serial.println();

	Serial.println();
}

void _DEBUG_createCustomChars()
{
	lcd.createChar(0, LCD_CHAR_FOX);
	lcd.createChar(1, LCD_CHAR_GEM_SMALL);
	lcd.createChar(2, LCD_CHAR_GEM);

	lcd.createChar(3, LCD_CHAR_ANGLE);
	lcd.createChar(4, LCD_CHAR_ANGLE_2);
	lcd.createChar(5, LCD_CHAR_CHECKMARK);
	lcd.createChar(6, LCD_CHAR_CLOCKWISE);
	lcd.createChar(7, LCD_CHAR_NOTES);

	_DEBUG_displayKeyCodes();


	lcd.createChar(0, LCD_CHAR_SHAPE_ROUND);
	lcd.createChar(1, LCD_CHAR_SHAPE_OVAL);
	lcd.createChar(2, LCD_CHAR_SHAPE_CUSHION);
	lcd.createChar(3, LCD_CHAR_SHAPE_EMERALD);
	lcd.createChar(4, LCD_CHAR_SHAPE_BAGUETTE);
	lcd.createChar(5, LCD_CHAR_SHAPE_TRILLIANT);
	lcd.createChar(6, LCD_CHAR_SHAPE_TRIANGLE);
	lcd.createChar(7, LCD_CHAR_SHAPE_SPECIAL);

	_DEBUG_displayKeyCodes();


	lcd.createChar(0, LCD_CHAR_CUT_BRILLIANT);
	lcd.createChar(1, LCD_CHAR_CUT_STEP);

	lcd.createChar(2, LCD_CHAR_ARROW_UP);
	lcd.createChar(3, LCD_CHAR_ARROW_DOWN);
	lcd.createChar(4, LCD_CHAR_ARROW_LEFT_LARGE);
	lcd.createChar(5, LCD_CHAR_ARROW_RIGHT_LARGE);

	_DEBUG_displayKeyCodes();
}
