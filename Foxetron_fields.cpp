/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright � 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_fields.h"


#pragma region FIELD DEFINITION

// CONSTRUCTORS

Field::Field(RDATUM value, DataType type) : _Value(value), _DataType(type) { }

Field::Field(PBYTE value, DataType type) : _DataType(type)
{
	for (BYTE i = 0; i < SIZEOF(value); i++)
		_Value.Bytes[i] = value[i];
}

Field::Field(RCHAR value, DataType type) : _DataType(type)
{
	_Value.CharVal = value;
}

Field::Field(RSHORT value, DataType type) : _DataType(type)
{
	_Value.ShortVal = value;
}

Field::Field(RLONG value, DataType type) : _DataType(type)
{
	_Value.LongVal = value;
}

Field::Field(RFLOAT value, DataType type) : _DataType(type)
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
VOID Field::SetValue(TVal & value)
{

}

#pragma endregion FIELD DEFINITION