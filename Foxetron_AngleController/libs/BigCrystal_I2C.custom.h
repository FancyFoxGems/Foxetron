/********************************************************************************************************************
* This file, redistributed with the Foxetron program suite, is part of an Arduino library in the Public Domain.     *
*********************************************************************************************************************/

#ifndef BigCrystal_I2C_h
#define BigCrystal_I2C_h


// GCC WARNING SUPPRESSIONS

#pragma GCC diagnostic ignored "-Wunused-value"


#include "BigFont.h"
#include "LiquidCrystal_I2C.custom.h"


class BigCrystal_I2C : public LiquidCrystal_I2C
{
public:
	/* Creates BigCrystal_I2C instance for a display on the specified i2c address.
	 * Parameters
	 *   i2cAddr: the i2c address of the display
	 */
	BigCrystal_I2C(uint8_t i2cAddr, uint8_t lcd_cols, uint8_t lcd_rows);

	/* Returns the width in columns of the specified character.
	 * If the character cannot be printed big, then zero is returned.
	 * Parameters:
	 *   c: the character whose width is required
	 * Returns:
	 *   the width of the character, including the empty spacer column,
	 *   or zero if the character cannot be displayed.
	 */
	uint8_t widthBig(char c);

	/* Writes the specified character to the coordinate specified on the display.
	 * Parameters:
	 *     c: the character to display
	 *   col: the column on the display where the left edge of the character starts
	 *   row: the row on the display where the top of the character starts
	 * Returns:
	 *   the width of the character displayed, including the empty spacer column,
	 *   or zero if the character cannot be displayed.
	 */
	uint8_t writeBig(char c, uint8_t col, uint8_t row);

	/* Writes the specified string from left to right, starting at the specified
	 * display coordinate. No bounds checks are made to ensure that the string
	 * will fit on the display. On a four line or greater display, characters will
	 * not wrap onto lower display rows.
	 * Parameters:
	 *    str: the String to display
	 *    col: the column on the display where the left edge of the first character
	 *         starts
	 *    row: the row on the display where the top edge of characters start
	 * Returns:
	 *   the total width of all printed characters, including all empty spacer columns
	 */
	uint8_t printBig(char *str, uint8_t col, uint8_t row);
	uint8_t printBig(const __FlashStringHelper * str, uint8_t col, uint8_t row);
private:
	uint8_t getWidthFromTableCode(uint8_t tableCode);
	const uint8_t* getTable(uint8_t tableCode);
	void getTableCodeAndIndex(char c, uint8_t &tableCode, uint8_t &index);
	void clearColumn(uint8_t row, uint8_t col);
	char toUpperCase(char c);
	bool supported(char c);
};

#endif
