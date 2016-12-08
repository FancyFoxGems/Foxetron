/***********************************************************************************************
* [Foxetron_AngleController_EEPROM.h]: EEPROM MODULE FOR MANAGING USER SETTINGS & FILE RECORDS
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_EEPROM_H
#define _FOXETRON_EEPROM_H


#pragma region INCLUDES

// ITTY BITTY
#include "IttyBitty_DB.h"
#include "IttyBitty_storage_adapters.h"

using namespace IttyBitty;

// ARDUINO LIBS
//#include <EEPROM.h>			// included by DB 3rd-party library

#pragma endregion


#pragma region ExtEEPROM GLOBAL VARIABLE DECLARATION

EXTERN EEPROMI2C<> ExtEEPROM;

#pragma endregion

#endif
