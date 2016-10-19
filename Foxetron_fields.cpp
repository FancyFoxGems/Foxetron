/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_fields.h"


/* Field DEFINITION */

#pragma region CONSTRUCTORS

RFIELD Field::NULL_OBJECT()
{
	STATIC FIELD NULL_FIELD((BYTE)0);
	return NULL_FIELD;
}

Field::Field(RCDATUM value, DataType type) : _Value(value), _DataType(type) { }

Field::Field(PBYTE value, DataType type) : _DataType(type)
{
	for (BYTE i = 0; i < SIZEOF(value); i++)
		_Value.Bytes[i] = value[i];
}

Field::Field(RCHAR value)
{
	_Value.CharVal = value;
}

Field::Field(RSHORT value)
{
	_Value.ShortVal = value;
}

Field::Field(RLONG value)
{
	_Value.LongVal = value;
}

Field::Field(RFLOAT value)
{
	_Value.FloatVal = value;
}

#pragma endregion


#pragma region METHODS

template<typename TVal>
CONST TVal Field::GetValue() const
{
	return reinterpret_cast<TVal>(_Value.ByteVal);
}

template<>
CONST CHAR Field::GetValue<CHAR>() const
{
	return static_cast<CCHAR>(_Value.CharVal);
}

template<typename TVal>
VOID Field::SetValue(const TVal & value)
{

}

#pragma endregion
