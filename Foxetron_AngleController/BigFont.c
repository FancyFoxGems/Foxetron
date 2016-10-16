/********************************************************************************************************************
* This file, redistributed with the Foxetron program suite, is part of an Arduino library in the Public Domain.     *
*********************************************************************************************************************/

#include "BigFont.h"

//************************************************************************
//*	A set of custom made large numbers for a 16x2 LCD using the
//*	LiquidCrystal librabry. Works with displays compatible with the
//*	Hitachi HD44780 driver.
//*
//*	orginal developed  by Michael Pilcher  2/9/2010
//*	there are 8 entries, 8 bytes per entry
//*	these are the building blocks to make the numbers
//*
//*	http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1265696343
//************************************************************************


//*	The LL, LT, UB, ect... are  abreviations to designate which segment was which when referencing the large '0'.
//*	LT= left top
//*	UB= upper bar
//*	RT= right top
//*	LL= lower left
//*	LB= lower bar
//*	LR= lower right
//*	UMB= upper middle bars(upper middle section of the '8')
//*	LMB= lower middle bars(lower middle section of the '8')


const uint8_t BF_fontShapes[] PROGMEM = {
//*	LT[8] =
  B00111,
  B01111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
//*	UB[8] =
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
//*	RT[8] =
  B11100,
  B11110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
//*	LL[8] =
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01111,
  B00111,
//*	LB[8] =
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
//*	LR[8] =
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11110,
  B11100,
//*	UMB[8] =
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
//*	LMB[8] =
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};

/* Stores the table storing character data and index into the corresponding
 * array for the character width that the data starts. 3 bits for
 * width and 5 bits for the index. index 0 of this table is for
 * ASCII 0x20 (space) */
const uint8_t BF_characters[] PROGMEM = {
  (BF_WIDTH1_TABLE << 5) | 0,         // 0x20 (space)
  (BF_WIDTH1_TABLE << 5) | 1,         // 0x21 !
  (BF_WIDTH3_SYMBOLS_TABLE << 5) | 0, // 0x22 "
  BF_NOT_SUPPORTED,                   // 0x23 #
  BF_NOT_SUPPORTED,                   // 0x24 $
  BF_NOT_SUPPORTED,                   // 0x25 %
  BF_NOT_SUPPORTED,                   // 0x26 &
  (BF_WIDTH1_TABLE << 5) | 2,         // 0x27 '
  (BF_WIDTH1_TABLE << 5) | 3,         // 0x28 (
  (BF_WIDTH1_TABLE << 5) | 4,         // 0x29 )
  BF_NOT_SUPPORTED,                   // 0x2A *
  (BF_WIDTH3_SYMBOLS_TABLE << 5) | 1, // 0x2B +
  (BF_WIDTH1_TABLE << 5) | 5,         // 0x2C ,
  (BF_WIDTH2_TABLE << 5) | 0,         // 0x2D -
  (BF_WIDTH1_TABLE << 5) | 6,         // 0x2E .
  BF_NOT_SUPPORTED,                   // 0x2F /
  (BF_WIDTH3_TABLE << 5) | 0,         // 0x30 0
  (BF_WIDTH3_TABLE << 5) | 1,         // 0x31 1
  (BF_WIDTH3_TABLE << 5) | 2,         // 0x32 2
  (BF_WIDTH3_TABLE << 5) | 3,         // 0x33 3
  (BF_WIDTH3_TABLE << 5) | 4,         // 0x34 4
  (BF_WIDTH3_TABLE << 5) | 5,         // 0x35 5
  (BF_WIDTH3_TABLE << 5) | 6,         // 0x36 6
  (BF_WIDTH3_TABLE << 5) | 7,         // 0x37 7
  (BF_WIDTH3_TABLE << 5) | 8,         // 0x38 8
  (BF_WIDTH3_TABLE << 5) | 9,         // 0x39 9
  (BF_WIDTH1_TABLE << 5) | 7,         // 0x3A :
  (BF_WIDTH1_TABLE << 5) | 8,         // 0x3B ;
  BF_NOT_SUPPORTED,                   // 0x3C <
  (BF_WIDTH2_TABLE << 5) | 1,         // 0x3D =
  BF_NOT_SUPPORTED,                  // 0x3E <
  (BF_WIDTH3_SYMBOLS_TABLE << 5) | 2, // 0x3F ?
  BF_NOT_SUPPORTED,                   // 0x40 @
  (BF_WIDTH3_TABLE << 5) | 10,        // 0x41 A
  (BF_WIDTH3_TABLE << 5) | 11,        // 0x42 B
  (BF_WIDTH3_TABLE << 5) | 12,        // 0x43 C
  (BF_WIDTH3_TABLE << 5) | 13,        // 0x44 D
  (BF_WIDTH3_TABLE << 5) | 14,        // 0x45 E
  (BF_WIDTH3_TABLE << 5) | 15,        // 0x46 F
  (BF_WIDTH3_TABLE << 5) | 16,        // 0x47 G
  (BF_WIDTH3_TABLE << 5) | 17,        // 0x48 H
  (BF_WIDTH3_TABLE << 5) | 18,        // 0x49 I
  (BF_WIDTH3_TABLE << 5) | 19,        // 0x4A J
  (BF_WIDTH3_TABLE << 5) | 20,        // 0x4B K
  (BF_WIDTH3_TABLE << 5) | 21,        // 0x4C L
  (BF_WIDTH5_TABLE << 5) | 0,         // 0x4D M
  (BF_WIDTH4_TABLE << 5) | 0,         // 0x4E N
  (BF_WIDTH3_TABLE << 5) | 0,         // 0x4F O; same as 0, so re-use
  (BF_WIDTH3_TABLE << 5) | 22,        // 0x50 P
  (BF_WIDTH4_TABLE << 5) | 1,         // 0x51 Q
  (BF_WIDTH3_TABLE << 5) | 23,        // 0x52 R
  (BF_WIDTH3_TABLE << 5) | 24,        // 0x53 S
  (BF_WIDTH3_TABLE << 5) | 25,        // 0x54 T
  (BF_WIDTH3_TABLE << 5) | 26,        // 0x55 U
  (BF_WIDTH4_TABLE << 5) | 2,         // 0x56 V
  (BF_WIDTH5_TABLE << 5) | 1,         // 0x57 W
  (BF_WIDTH3_TABLE << 5) | 27,        // 0x58 X
  (BF_WIDTH3_TABLE << 5) | 28,        // 0x59 Y
  (BF_WIDTH3_TABLE << 5) | 29         // 0x5A Z
};

const uint8_t BF_width1[] PROGMEM = {
  0x20, 0x20, // 0x20 space
  0x00, 0x04, // 0x21 !
  0x05, 0x20, // 0x27 '
  0x00, 0x03, // 0x28 (
  0x02, 0x05, // 0x29 )
  0x20, 0x05, // 0x2C ,
  0x20, 0x04, // 0x2E .
  0x6F, 0x6F, // 0x3A :
  0x01, 0x05  // 0x3B ;
};

const uint8_t BF_width2[] PROGMEM = {
  0x04, 0x04, 0x20, 0x20, // 0x2D -
  0x04, 0x04, 0x07, 0x07  // 0x3D =
};

const uint8_t BF_width3[] PROGMEM = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // 0x30 0, 0x4F O
  0x01, 0x02, 0x20, 0x04, 0xFF, 0x04, // 0x31 1
  0x06, 0x06, 0x02, 0x03, 0x04, 0x04, // 0x32 2
  0x06, 0x06, 0x02, 0x04, 0x04, 0x05, // 0x33 3
  0x03, 0x04, 0x02, 0x20, 0x20, 0x05, // 0x34 4
  0xFF, 0x06, 0x06, 0x04, 0x04, 0x05, // 0x35 5
  0x00, 0x06, 0x06, 0x03, 0x04, 0x05, // 0x36 6
  0x01, 0x01, 0x02, 0x20, 0x00, 0x20, // 0x37 7
  0x00, 0x06, 0x02, 0x03, 0x04, 0x05, // 0x38 8
  0x00, 0x06, 0x02, 0x20, 0x20, 0x05, // 0x39 9
  0x00, 0x06, 0x02, 0xFF, 0x20, 0xFF, // 0x41 A
  0xFF, 0x06, 0x05, 0xFF, 0x07, 0x02, // 0x42 B
  0x00, 0x01, 0x01, 0x03, 0x04, 0x04, // 0x43 C
  0xFF, 0x01, 0x02, 0xFF, 0x04, 0x05, // 0x44 D
  0xFF, 0x06, 0x06, 0xFF, 0x07, 0x07, // 0x45 E
  0xFF, 0x06, 0x06, 0xFF, 0x20, 0x20, // 0x46 F
  0x00, 0x01, 0x01, 0x03, 0x04, 0x02, // 0x47 G
  0xFF, 0x04, 0xFF, 0xFF, 0x20, 0xFF, // 0x48 H
  0x01, 0xFF, 0x01, 0x04, 0xFF, 0x04, // 0x49 I
  0x20, 0x20, 0xFF, 0x04, 0x04, 0x05, // 0x4A J
  0xFF, 0x04, 0x05, 0xFF, 0x20, 0x02, // 0x4B K
  0xFF, 0x20, 0x20, 0xFF, 0x04, 0x04, // 0x4C L
  0x00, 0x06, 0x02, 0x03, 0x20, 0x20, // 0x50 P
  0x00, 0x06, 0x05, 0x03, 0x20, 0x02, // 0x52 R
  0x00, 0x06, 0x06, 0x07, 0x07, 0x05, // 0x53 S
  0x01, 0x02, 0x01, 0x20, 0x05, 0x20, // 0x54 T
  0x02, 0x20, 0x02, 0x03, 0x04, 0x05, // 0x55 U
  0x03, 0x04, 0x05, 0x00, 0x20, 0x02, // 0x58 X
  0x03, 0x04, 0x05, 0x20, 0x05, 0x20, // 0x59 Y
  0x01, 0x06, 0x05, 0x00, 0x07, 0x04  // 0x5A Z
};

const uint8_t BF_width3Symbols[] PROGMEM = {
  0x05, 0x20, 0x05, 0x20, 0x20, 0x20, // 0x22 "
  0x04, 0xFF, 0x04, 0x01, 0xFF, 0x01, // 0x2B +
  0x01, 0x06, 0x02, 0x20, 0x07, 0x20  // 0x3F ?
};

const uint8_t BF_width4[] PROGMEM = {
  0x00, 0x03, 0x20, 0x02, 0x03, 0x20, 0x02, 0x05, // 0x4E N
  0x00, 0x01, 0x02, 0x20, 0x03, 0x04, 0x03, 0x04, // 0x51 Q
  0x03, 0x20, 0x20, 0x05, 0x20, 0x03, 0x05, 0x20  // 0x56 V
};

const uint8_t BF_width5[] PROGMEM = {
  0x00, 0x01, 0x03, 0x01, 0x02, 0x03, 0x20, 0x20, 0x20, 0x05, // 0x4D M
  0x00, 0x20, 0x20, 0x20, 0x02, 0x03, 0x04, 0x00, 0x04, 0x05  // 0x57 W
};
