/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_fields.h"


#pragma region FIELD DEFINITION

// CONSTRUCTORS

Field::Field(Datum & value, DataType type) : _Value(value), _Type(type) { }

Field::Field(BYTE * value, DataType type) : _Type(type)
{
	for (BYTE i = 0; i < SIZEOF(value); i++)
		_Value.Bytes[i] = value[i];
}

Field::Field(CHAR & value, DataType type) : _Type(type)
{
	_Value.CharVal = value;
}

Field::Field(short & value, DataType type) : _Type(type)
{
	_Value.ShortVal = value;
}

Field::Field(LONG & value, DataType type) : _Type(type)
{
	_Value.LongVal = value;
}

Field::Field(FLOAT & value, DataType type) : _Type(type)
{
	_Value.FloatVal = value;
}


// METHODS

template<typename TVal>
CONST TVal Field::GetValue() CONST
{
	return reinterpret_cast<TVal>(_Value.ByteVal);
}

template<>
CONST CHAR Field::GetValue<CHAR>() CONST
{
	return static_cast<CCHAR>(_Value.CharVal);
}


template<typename TVal>
VOID Field::SetValue(TVal &)
{
}

#pragma endregion FIELD DEFINITION
