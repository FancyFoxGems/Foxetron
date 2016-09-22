/* INCLUDES */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>



/* DEFINES */

// PROGRAM OPTIONS
		asdf
#define DEBUG_INPUTS		1
#define SERIAL_ENABLE
#define SERIAL_BAUD_RATE	115200

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
#define PIN_ANGLE_ENCODER_Z		4	// Pin 4 / PD4 (PCINT20)
#define PIN_ANGLE_ENCODER_U		5	// Pin 5 / PD5 (PCINT21)

#define PIN_LED_BUTTON_1		6	// Pin 6 / PD6 (PCINT22)
#define PIN_LED_BUTTON_2		7	// Pin 7 / PD7 (PCINT23)
#define PIN_LED_BUTTON_3		8	// Pin 8 / PB0 (PCINT0)
#define PIN_LED_BUTTON_4		12	// Pin 12 / PB4 (PCINT4)

#define PIN_ADC_LED_BUTTON_5	6	// A6 / ADC6

#define PIN_ADC_MODE_SWITCH 	7	// A6 / ADC6

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



/* FLASH DATA CONSTANTS */

// CUSTOM LCD CHARACTERS

byte note[8]	= {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
byte clockIcon[8] = {0x0, 0xe, 0x15, 0x17, 0xb, 0xe, 0x0};
byte heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
byte checkIcon[8] = {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0};
byte retarrow[8] = {	0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};

byte fox[8] = { 0x11, 0x1b, 0x1f, 0x11, 0x1b, 0x11, 0xe, 0x4 };
byte gemSmall[8] = { 0x0, 0x0, 0x0, 0xe, 0x11, 0xa, 0x4, 0x0 };
byte gem[8] = { 0x0, 0x0, 0xe, 0x1b, 0x15, 0xa, 0x4, 0x0 };

byte angle1[8]	= {	0x1, 0x3, 0x6, 0xc, 0x1a, 0x12, 0x1f, 0x0 };
byte angle2[8] = { 0x0, 0x1, 0x2, 0x4, 0xa, 0x12, 0x1f, 0x0 };
byte degrees1[8]	= { 0x6, 0x9, 0x9, 0x6, 0x0, 0x0, 0x0, 0x0 };
byte degrees2[8] = { 0x4, 0xa, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0 };
byte clockwise[8] = { 0x0, 0xe, 0x1b, 0x11, 0x5, 0xf, 0x4, 0x0 };
byte counterClockwise[8] = { 0x0, 0xe, 0x1b, 0x11, 0x14, 0x1e, 0x4, 0x0 };

byte brilliantCut[8] = {	0x0, 0xe, 0x15, 0x1b, 0x15, 0xa, 0x4, 0x0 };
byte stepCut[8] = {	0x0, 0xe, 0x11, 0x1f, 0x11, 0x1f, 0xa, 0x4 };

byte roundShape[8] = {	0x0, 0x0, 0xe, 0x1b, 0x11, 0x1b, 0xe, 0x0 };
byte ovalShape[8] = { 0x0, 0xe, 0x1b, 0x11, 0x11, 0x1b, 0xe, 0x0 };
byte cushionShape[8] = { 0x0, 0x6, 0xf, 0x19, 0x13, 0x1e, 0xc, 0x0 };
byte squareShape[8] = {	0x0, 0x1f, 0x11, 0x11, 0x11, 0x11, 0x1f, 0x0 };
byte baguetteShape[8] = { 0x0, 0xe, 0xa, 0xa, 0xa, 0xa, 0xe, 0x0 };
byte emeraldShape[8] = {	0xe, 0x1b, 0x11, 0x11, 0x11, 0x11, 0x1b, 0xe };
byte trilliantShape[8] = { 0x0, 0x4, 0xe, 0x1b, 0x11, 0x1b, 0xe, 0x0 };
byte triangleShape[8] = { 0x0, 0x4, 0xe, 0xa, 0x1b, 0x11, 0x1f, 0x0 };
byte specialShape[8] = { 0x0, 0x0, 0xa, 0x15, 0x11, 0xa, 0x4, 0x0 };

byte upArrowSmall[8] = { 0x0, 0x0, 0x4, 0xe, 0x4, 0x4, 0x0, 0x0 };
byte downArrowSmall[8] = { 0x0, 0x0, 0x4, 0x4, 0xe, 0x4, 0x0, 0x0 };
byte leftArrowSmall[8] = { 0x0, 0x0, 0x0, 0x8, 0x1e, 0x8, 0x0, 0x0 };
byte rightArrowSmall[8] = { 0x0, 0x0, 0x0, 0x2, 0xf, 0x2, 0x0, 0x0 };

byte upArrow[8] = { 0x0, 0x4, 0xe, 0x15, 0x4, 0x4, 0x4, 0x0 };
byte downArrow[8] = { 0x0, 0x4, 0x4, 0x4, 0x15, 0xe, 0x4, 0x0 };
byte leftArrow[8] = { 0x0, 0x0, 0x4, 0x2, 0x1f, 0x2, 0x4, 0x0 };
byte rightArrow[8] = { 0x0, 0x0, 0x4, 0x8, 0x1f, 0x8, 0x4, 0x0 };

byte upArrowLarge[8] = { 0x4, 0xe, 0x15, 0x4, 0x4, 0x4, 0x4, 0x0 };
byte downArrowLarge[8] = { 0x0, 0x4, 0x4, 0x4, 0x4, 0x15, 0xe, 0x4 };
byte leftArrowLarge[8] = { 0x0,0x0,0x4,0xc,0x1f,0xc,0x4,0x0 };
byte rightArrowLarge[8] = { 0x0,0x0,0x4,0x6,0x1f,0x6,0x4,0x0 };



/* EEPROM VARIABLES */



/* PROGRAM CONSTANTS */



/* GLOBAL VARIABLES */

// INPUTS

bool angleEncoderA = 0;		// Pin 2 / PD2 (INT0)
bool angleEncoderB = 0;		// Pin 3 / PD3 (INT1)
bool angleEncoderZ = 0;		// Pin 4 / PD4 (PCINT20)
bool angleEncoderU = 0;		// Pin 5 / PD5 (PCINT21)

bool ledButton1 = 0;		// Pin 6 / PD6 (PCINT22)
bool ledButton2 = 0;		// Pin 7 / PD7 (PCINT23)
bool ledButton3 = 0;		// Pin 8 / PB0 (PCINT0)
bool ledButton4 = 0;		// Pin 12 / PB4 (PCINT4)

bool ledButton5 = 0;		// A6 / ADC6
word ledButton5Val = 0;

bool modeSwitch = 0;		// A7 / ADC7
word modeSwitchVal = 0;

bool menuEncoderA = 0;		// Pin 14/A0 / PC0 (PCINT8)
bool menuEncoderB = 0;		// Pin 15/A1 / PC1 (PCINT9)

bool selectButton = 0;		// Pin 16/A2 / PC2 (PCINT10)
bool shiftButton = 0;		// Pin 17/A3 / PC3 (PCINT11)


// OUTPUTS

byte rgbRed = 0;			// Pin 9 / PB1
byte rgbGreen = 0;			// Pin 10 / PB2
byte rgbBlue = 0;			// Pin 11 / PB3

bool statusLED = 0;			// Pin 13 / PB5


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
}

void loop()
{
	_DEBUG_printInputValues();

	statusLED = !statusLED;
	digitalWrite(13, statusLED);

	delay(100);
}


// PROGRAM FUNCTIONS

void initializePins()
{
	// REAR PINS

	// [Pins 0 (RX) & 1 (TX): USART serial communication --[angle feedback]--> AVR uC stepper motor angle controller]

	// Angle encoder
	pinMode(PIN_ANGLE_ENCODER_A, INPUT_PULLUP);		// Channel A
	pinMode(PIN_ANGLE_ENCODER_B, INPUT_PULLUP);		// Channel B
	pinMode(PIN_ANGLE_ENCODER_Z, INPUT_PULLUP);		// Channel Z
	pinMode(PIN_ANGLE_ENCODER_U, INPUT_PULLUP);		// Channel U

	// LED buttons
	pinMode(PIN_LED_BUTTON_1, INPUT);				// LED button #1
	pinMode(PIN_LED_BUTTON_1, INPUT);				// LED button #2
	pinMode(PIN_LED_BUTTON_1, INPUT);				// LED button #3

	// [Pins 9-11: RGB LED - 9: red / 10: green / 11: blue]

	pinMode(PIN_LED_BUTTON_4, INPUT);				// LED button #4


	// FRONT PINS

	pinMode(PIN_OUT_STATUS_LED, OUTPUT);			// Status LED

	// Menu encoder
	pinMode(PIN_MENU_ENCODER_A, INPUT_PULLUP);		// Channel A
	pinMode(PIN_MENU_ENCODER_B, INPUT_PULLUP);		// Channel B

	// Menu buttons
	pinMode(PIN_SELECT_BUTTON, INPUT_PULLUP);		// Select button
	pinMode(PIN_SHIFT_BUTTON, INPUT_PULLUP);		// Shift button

	// [Pins A7 (ADC6; analog only): Mode switch

	// [Pins A6 (ADC6; analog only): LED Button # 5

	// [Pins 18 & 19 - SDA: 18/A4 & SCL: 19/A5 | I2C --> PCF8574T port expander --> RGB HD44780]
}

void initializeLCD()
{
	lcd.init();
	lcd.backlight();
	lcd.home();
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
	lcd.createChar(0, fox);
	lcd.createChar(1, gemSmall);
	lcd.createChar(2, gem);

	lcd.createChar(3, angle1);
	lcd.createChar(4, angle2);
	lcd.createChar(5, degrees1);
	lcd.createChar(6, degrees2);
	lcd.createChar(7, clockwise);

	_DEBUG_displayKeyCodes();


	lcd.createChar(0, brilliantCut);
	lcd.createChar(1, stepCut);
	lcd.createChar(2, roundShape);
	lcd.createChar(3, ovalShape);
	lcd.createChar(4, cushionShape);
	lcd.createChar(5, emeraldShape);
	lcd.createChar(6, baguetteShape);
	lcd.createChar(7, trilliantShape);

	_DEBUG_displayKeyCodes();


	lcd.createChar(0, triangleShape);
	lcd.createChar(1, specialShape);

	lcd.createChar(2, counterClockwise);
	lcd.createChar(3, retarrow);
	lcd.createChar(4, heart);
	lcd.createChar(5, clockIcon);
	lcd.createChar(6, checkIcon);
	lcd.createChar(7, note);

	_DEBUG_displayKeyCodes();


	lcd.createChar(0, upArrow);
	lcd.createChar(1, downArrow);
	lcd.createChar(2, leftArrow);
	lcd.createChar(3, rightArrow);
	lcd.createChar(4, upArrowLarge);
	lcd.createChar(5, downArrowLarge);
	lcd.createChar(6, leftArrowLarge);
	lcd.createChar(7, rightArrowLarge);

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

	angleEncoderA = digitalRead(2);
	angleEncoderB = digitalRead(3);
	angleEncoderZ = digitalRead(4);
	angleEncoderU = digitalRead(5);

	ledButton1 = digitalRead(6);
	if (ledButton1)
	{
		PORTD |= (1 << 6);
		DDRD |= (1 << 6);
		PORTD &= ~(1 << 6);
		DDRD &= ~(1 << 6);
	}

	ledButton2 = digitalRead(7);
	if (ledButton2)
	{
		PORTD |= (1 << 7);
		DDRD |= (1 << 7);
		PORTD &= ~(1 << 7);
		DDRD &= ~(1 << 7);
	}

	ledButton3 = digitalRead(8);
	if (ledButton3)
	{
		PORTB |= (1 << 0);
		DDRB |= (1 << 0);
		PORTB &= ~(1 << 0);
		DDRB &= ~(1 << 0);
	}

	ledButton4 = digitalRead(12);
	if (ledButton4)
	{
		PORTB |= (1 << 4);
		DDRB |= (1 << 4);
		PORTB &= ~(1 << 4);
		DDRB &= ~(1 << 4);
	}

	ledButton5 = analogRead(6) > 500 ? true : false;

	modeSwitch = analogRead(7) > 500 ? true : false;

	menuEncoderA = digitalRead(14);
	menuEncoderB = digitalRead(15);

	selectButton = digitalRead(16);
	shiftButton = digitalRead(17);


	// Rear inputs

	lcd.home();

	lcd.setCursor(0, 0);
	lcd.print(itoa(angleEncoderA, valStr, 2));

	lcd.setCursor(2, 0);
	lcd.print(itoa(angleEncoderB, valStr, 2));

	lcd.setCursor(4, 0);
	lcd.print(itoa(angleEncoderZ, valStr, 2));

	lcd.setCursor(6, 0);
	lcd.print(itoa(angleEncoderU, valStr, 2));

	lcd.setCursor(8, 0);
	lcd.print(itoa(ledButton1, valStr, 2));

	lcd.setCursor(10, 0);
	lcd.print(itoa(ledButton2, valStr, 2));

	lcd.setCursor(12, 0);
	lcd.print(itoa(ledButton3, valStr, 2));

	lcd.setCursor(14, 0);
	lcd.print(itoa(ledButton4, valStr, 2));


	// Front inputs

	lcd.setCursor(0, 1);
	lcd.print(itoa(ledButton5, valStr, 2));

	lcd.setCursor(4, 1);
	lcd.print(itoa(modeSwitchVal, valStr, 10));

	lcd.setCursor(8, 1);
	lcd.print(itoa(menuEncoderA, valStr, 2));

	lcd.setCursor(10, 1);
	lcd.print(itoa(menuEncoderB, valStr, 2));

	lcd.setCursor(12, 1);
	lcd.print(itoa(selectButton, valStr, 2));

	lcd.setCursor(14, 1);
	lcd.print(itoa(shiftButton, valStr, 2));

#ifdef DEBUG_INPUTS

	Serial.print(itoa(angleEncoderA, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(angleEncoderB, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(angleEncoderZ, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(angleEncoderU, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(ledButton1, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(ledButton2, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(ledButton3, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(ledButton4, valStr, 2));
	Serial.print("\n");

	Serial.print(itoa(ledButton5Val, valStr, 10));
	Serial.print(" / ");
	Serial.print(itoa(ledButton5, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(modeSwitchVal, valStr, 10));
	Serial.print(" / ");
	Serial.print(itoa(modeSwitch, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(menuEncoderB, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(selectButton, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(shiftButton, valStr, 2));
	Serial.print('\n');
	Serial.print('\n');

#endif
}
