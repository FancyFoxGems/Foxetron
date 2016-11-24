/************************************************************************************************************************************
* This file, redistributed with the Foxetron program suite, is part of an Arduino library released without a specified license.     *
*************************************************************************************************************************************/
//YWROBOT
#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h

#include <inttypes.h>
#include "Print.h"
#include "Wire.h"


// display constants/options

#define LCD_DEFAULT_I2C_ADDRESS			0x27

#define LCD_DEFAULT_COLS				16
#define LCD_DEFAULT_ROWS				2

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B00000100  // Enable bit
#define Rw B00000010  // Read/Write bit
#define Rs B00000001  // Register select bit


// Grove RGB LCD support

#define RGB_ADDRESS     (0xC4 >> 1)

#define REG_RED         0x04        // pwm2
#define REG_GREEN       0x03        // pwm1
#define REG_BLUE        0x02        // pwm0

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_OUTPUT      0x08

#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3


class LiquidCrystal_I2C : public Print
{
public:
	LiquidCrystal_I2C(uint8_t lcd_Addr = LCD_DEFAULT_I2C_ADDRESS,
		uint8_t lcd_cols = LCD_DEFAULT_COLS, uint8_t lcd_rows = LCD_DEFAULT_ROWS);

	void begin(uint8_t cols = LCD_DEFAULT_COLS, uint8_t rows = LCD_DEFAULT_ROWS, uint8_t charsize = LCD_5x8DOTS);

	void clear();
	void home();

	void noDisplay();
	void display();
	void noBlink();
	void blink();
	void noCursor();
	void cursor();
	void noBacklight();
	void backlight();
	void autoscroll();
	void noAutoscroll();

	void scrollDisplayLeft();
	void scrollDisplayRight();
	void printLeft();
	void printRight();
	void leftToRight();
	void rightToLeft();
	void shiftIncrement();
	void shiftDecrement();

	void createChar(uint8_t, uint8_t[]);
	void createChar(uint8_t location, const char *charmap);
	// Example: 	const char bell[8] PROGMEM = {B00100,B01110,B01110,B01110,B11111,B00000,B00100,B00000};

	void setCursor(uint8_t, uint8_t);

#if defined(ARDUINO) && ARDUINO >= 100
	virtual size_t write(uint8_t);
#else
	virtual void write(uint8_t);
#endif

	void command(uint8_t);
	void init();

	////compatibility API function aliases
	void blink_on();						// alias for blink()
	void blink_off();       					// alias for noBlink()
	void cursor_on();      	 					// alias for cursor()
	void cursor_off();      					// alias for noCursor()

	void setBacklight(uint8_t new_val);				// alias for backlight() and nobacklight()

	void load_custom_character(uint8_t char_num, uint8_t *rows);	// alias for createChar()

	void printstr(const char[]);

	////Unsupported API functions (not implemented in this library)
	uint8_t status();

	void setContrast(uint8_t new_val);

	uint8_t keypad();

	void setDelay(int, int);

	void on();
	void off();

	uint8_t init_bargraph(uint8_t graphtype);

	void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);
	void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);

	// color control
	void setRGB(unsigned char r, unsigned char g, unsigned char b);               // set rgb
	void setPWM(unsigned char color, unsigned char pwm) { setReg(color, pwm); }      // set pwm

	void setColor(unsigned char color);
	void setColorAll() { setRGB(0, 0, 0); }
	void setColorWhite() { setRGB(255, 255, 255); }

	// blink the LED backlight
	void blinkLED();
	void noBlinkLED();


private:

	uint8_t _Addr;
	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
	uint8_t _numlines;
	uint8_t _cols;
	uint8_t _rows;
	uint8_t _backlightval;

	void init_priv();

	void send(uint8_t, uint8_t);
	void write4bits(uint8_t);
	void expanderWrite(uint8_t);
	void pulseEnable(uint8_t);

	void setReg(unsigned char addr, unsigned char dta);
};

#endif
