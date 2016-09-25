/* INCLUDES */

// AVR LIBS
#include <avr/pgmspace.h>

// ARDUINO LIBS
#include <Wire.h>

// 3RD-PARTY LIBS
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>



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



/* FLASH DATA */

// CUSTOM LCD CHARACTERS

// Miscellaneous icons
PROGMEM const char LCD_CHAR_FOX[8]					= { 0x11, 0x1b, 0x1f, 0x11, 0x1b, 0x11, 0xe, 0x4 };
PROGMEM const char LCD_CHAR_GEM[8]					= { 0x0, 0x0, 0xe, 0x1b, 0x15, 0xa, 0x4, 0x0 };
PROGMEM const char LCD_CHAR_GEM_SMALL[8]			= { 0x0, 0x0, 0x0, 0xe, 0x11, 0xa, 0x4, 0x0 };

// Angle symbols
PROGMEM const char LCD_CHAR_ANGLE[8]				= { 0x1, 0x3, 0x6, 0xc, 0x1a, 0x12, 0x1f, 0x0 };
PROGMEM const char LCD_CHAR_ANGLE_2[8]				= { 0x0, 0x1, 0x2, 0x4, 0xa, 0x12, 0x1f, 0x0 };
PROGMEM const char LCD_CHAR_DEGREES[8]				= { 0x6, 0x9, 0x9, 0x6, 0x0, 0x0, 0x0, 0x0 };
PROGMEM const char LCD_CHAR_DEGREES_SMALL[8]		= { 0x4, 0xa, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0 };
PROGMEM const char LCD_CHAR_CLOCKWISE[8]			= { 0x0, 0xe, 0x1b, 0x11, 0x5, 0xf, 0x4, 0x0 };
PROGMEM const char LCD_CHAR_COUNTER_CCW[8]			= { 0x0, 0xe, 0x1b, 0x11, 0x14, 0x1e, 0x4, 0x0 };

// Miscellaneous symbols
PROGMEM const char LCD_CHAR_RETURN_ARROW[8]			= { 0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4 };
PROGMEM const char LCD_CHAR_CHECKMARK[8]			= { 0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0 };
PROGMEM const char LCD_CHAR_CLOCK[8]				= { 0x0, 0xe, 0x15, 0x17, 0xb, 0xe, 0x0 };
PROGMEM const char LCD_CHAR_NOTE[8]					= { 0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0 };
PROGMEM const char LCD_CHAR_HEART[8]				= { 0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0 };

// Small arrow icons
PROGMEM const char LCD_CHAR_ARROW_UP_SMALL[8]		= { 0x0, 0x0, 0x4, 0xe, 0x4, 0x4, 0x0, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_DOWN_SMALL[8]		= { 0x0, 0x0, 0x4, 0x4, 0xe, 0x4, 0x0, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_LEFT_SMALL[8]		= { 0x0, 0x0, 0x0, 0x8, 0x1e, 0x8, 0x0, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_RIGHT_SMALL[8]	= { 0x0, 0x0, 0x0, 0x2, 0xf, 0x2, 0x0, 0x0 };

// Medium arrow icons
PROGMEM const char LCD_CHAR_ARROW_UP[8]				= { 0x0, 0x4, 0xe, 0x15, 0x4, 0x4, 0x4, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_DOWN[8]			= { 0x0, 0x4, 0x4, 0x4, 0x15, 0xe, 0x4, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_LEFT[8]			= { 0x0, 0x0, 0x4, 0x2, 0x1f, 0x2, 0x4, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_RIGHT[8]			= { 0x0, 0x0, 0x4, 0x8, 0x1f, 0x8, 0x4, 0x0 };

// Large arrow icons
PROGMEM const char LCD_CHAR_ARROW_UP_LARGE[8]		= { 0x4, 0xe, 0x15, 0x4, 0x4, 0x4, 0x4, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_DOWN_LARGE[8]		= { 0x0, 0x4, 0x4, 0x4, 0x4, 0x15, 0xe, 0x4 };
PROGMEM const char LCD_CHAR_ARROW_LEFT_LARGE[8]		= { 0x0, 0x0, 0x4, 0xc, 0x1f, 0xc, 0x4, 0x0 };
PROGMEM const char LCD_CHAR_ARROW_RIGHT_LARGE[8]	= { 0x0, 0x0, 0x4, 0x6, 0x1f, 0x6, 0x4, 0x0 };

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



/* EEPROM VARIABLES */



/* PROGRAM CONSTANTS */



/* GLOBAL VARIABLES */

// INPUTS

volatile byte _pinStatesB		= 0;
volatile byte _pinStatesC		= 0;
volatile byte _pinStatesD		= 0;

volatile bool _angleEncoderA	= 0;	// Pin 2 / PD2 (INT0)
volatile bool _angleEncoderB	= 0;	// Pin 3 / PD3 (INT1)
//volatile bool _angleEncoderZ	= 0;	// Pin 4 / PD4 (PCINT20)	- [UNUSED]
//volatile bool _angleEncoderU	= 0;	// Pin 5 / PD5 (PCINT21)	- [UNUSED]

volatile bool _angleUp			= 0;
volatile uint32_t _angleReading	= 0;
word _angleDelta				= 0;
word _angleVelocity				= 0;

bool _ledButton1				= 0;	// Pin 4 / PD4 (PCINT20)
bool _ledButton2				= 0;	// Pin 5 / PD5 (PCINT21)
bool _ledButton3				= 0;	// Pin 6 / PD6 (PCINT22)
bool _ledButton4				= 0;	// Pin 7 / PD7 (PCINT23)
bool _ledButton5				= 0;	// Pin 8 / PB0 (PCINT0)

// [FREE PIN: Pin 12 / PB4 (PCINT4)]

// [FREE PIN: Pin A6 / ADC6]
//bool _ledButton5				= 0;	// A6 / ADC6				- [UNUSED]
//volatile word _ledButton5Val	= 0;

volatile bool _modeSwitch		= 0;	// A7 / ADC7
volatile word _modeSwitchVal	= 0;

bool _menuEncoderA				= 0;	// Pin 14/A0 / PC0 (PCINT8)
bool _menuEncoderB				= 0;	// Pin 15/A1 / PC1 (PCINT9)

bool _menuUp					= 0;
uint32_t _menuReading			= 0;
word _menuDelta					= 0;
word _menuVelocity				= 0;

bool _selectButton				= 0;	// Pin 16/A2 / PC2 (PCINT10)
bool _shiftButton				= 0;	// Pin 17/A3 / PC3 (PCINT11)


// OUTPUTS

byte _rgbRed					= 0;	// Pin 9 / PB1
byte _rgbGreen					= 0;	// Pin 10 / PB2
byte _rgbBlue					= 0;	// Pin 11 / PB3

bool _statusLED					= 0;	// Pin 13 / PB5


// OBJECTS

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_CHAR_COLS, LCD_CHAR_ROWS);



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
}

void loop()
{
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
	_angleEncoder ## channel = !_angleEncoder ## channel;												\
	_angleUp = (_angleEncoder ## channel increment_comparison _angleEncoder ## other_channel);			\
	_ISR_angleEncoder_updateAngleReading();

static inline void _ISR_angleEncoder_updateAngleReading() __attribute__((always_inline));

static inline void _ISR_angleEncoder_updateAngleReading()
{
	if (_angleUp)
		++_angleReading;
	else
		--_angleReading;
}

ISR(INT0_vect)
{
	_ISR_ANGLE_ENCODER_READ_CHANNEL(A, B, ==);
}

ISR(INT1_vect)
{
	_ISR_ANGLE_ENCODER_READ_CHANNEL(B, A, !=);
}


// PCINT0/PCINT1/PCINT2: LED BUTTONS, MENU ENCODER, and MENU BUTTONS


/*asm volatile("\t"	
		"push %0"	"\n\t"
		"in %0, %1"	"\n"
	: "=&r" (current) : "I" (_SFR_IO_ADDR(PINC)) );*/

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
}

void initializeInterrupts()
{
	// Angle encoder
	EIMSK |= 0b00000011;
	EICRA &= 0b11110101;
	EICRA |= 0b00000101;
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

void _DEBUG_createCustomChars()
{
	lcd.createChar(0, LCD_CHAR_FOX);
	lcd.createChar(1, LCD_CHAR_GEM_SMALL);
	lcd.createChar(2, LCD_CHAR_GEM);

	lcd.createChar(3, LCD_CHAR_ANGLE);
	lcd.createChar(4, LCD_CHAR_ANGLE_2);
	lcd.createChar(5, LCD_CHAR_DEGREES_SMALL);
	lcd.createChar(6, LCD_CHAR_DEGREES);
	lcd.createChar(7, LCD_CHAR_CLOCKWISE);

	_DEBUG_displayKeyCodes();


	lcd.createChar(0, LCD_CHAR_CUT_BRILLIANT);
	lcd.createChar(1, LCD_CHAR_CUT_STEP);
	lcd.createChar(2, LCD_CHAR_SHAPE_ROUND);
	lcd.createChar(3, LCD_CHAR_SHAPE_OVAL);
	lcd.createChar(4, LCD_CHAR_SHAPE_CUSHION);
	lcd.createChar(5, LCD_CHAR_SHAPE_EMERALD);
	lcd.createChar(6, LCD_CHAR_SHAPE_BAGUETTE);
	lcd.createChar(7, LCD_CHAR_SHAPE_TRILLIANT);

	_DEBUG_displayKeyCodes();


	lcd.createChar(0, LCD_CHAR_SHAPE_TRIANGLE);
	lcd.createChar(1, LCD_CHAR_SHAPE_SPECIAL);

	lcd.createChar(2, LCD_CHAR_COUNTER_CCW);
	lcd.createChar(3, LCD_CHAR_RETURN_ARROW);
	lcd.createChar(4, LCD_CHAR_CHECKMARK);
	lcd.createChar(5, LCD_CHAR_CLOCK);
	lcd.createChar(6, LCD_CHAR_NOTE);
	lcd.createChar(7, LCD_CHAR_HEART);

	_DEBUG_displayKeyCodes();


	lcd.createChar(0, LCD_CHAR_ARROW_UP);
	lcd.createChar(1, LCD_CHAR_ARROW_DOWN);
	lcd.createChar(2, LCD_CHAR_ARROW_LEFT);
	lcd.createChar(3, LCD_CHAR_ARROW_RIGHT);
	lcd.createChar(4, LCD_CHAR_ARROW_UP_LARGE);
	lcd.createChar(5, LCD_CHAR_ARROW_DOWN_LARGE);
	lcd.createChar(6, LCD_CHAR_ARROW_LEFT_LARGE);
	lcd.createChar(7, LCD_CHAR_ARROW_RIGHT_LARGE);

	_DEBUG_displayKeyCodes();
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

	_angleEncoderA = digitalRead(2);
	_angleEncoderB = digitalRead(3);

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
	lcd.print(itoa(_angleEncoderA, valStr, 2));

	lcd.setCursor(2, 0);
	lcd.print(itoa(_angleEncoderB, valStr, 2));

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


	Serial.print(itoa(_angleEncoderA, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(_angleEncoderB, valStr, 2));
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
	Serial.print("\n");

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
	Serial.print('\n');
	Serial.print('\n');
}
