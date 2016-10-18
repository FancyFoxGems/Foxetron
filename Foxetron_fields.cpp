/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_fields.h"


#pragma region FIELD DEFINITION

// CONSTRUCTORS

Field::Field(Datum & value, DataType type) : _Value(value), _Type(type) { }

Field::Field(byte * value, DataType type) : _Type(type)
{
	for (byte i = 0; i < sizeof(value); i++)
		_Value.Bytes[i] = value[i];
}

Field::Field(char & value, DataType type) : _Type(type)
{
	_Value.CharVal = value;
}

Field::Field(short & value, DataType type) : _Type(type)
{
	_Value.ShortVal = value;
}

Field::Field(long & value, DataType type) : _Type(type)
{
	_Value.LongVal = value;
}

Field::Field(float & value, DataType type) : _Type(type)
{
	_Value.FloatVal = value;
}


// METHODS

template<typename TVal>
const TVal Field::GetValue() const
{
	return reinterpret_cast<TVal>(_Value.ByteVal);
}

template<>
const char Field::GetValue<char>() const
{
	return static_cast<const char>(_Value.CharVal);
}


template<typename TVal>
void Field::SetValue(TVal &)
{
}

#pragma endregion FIELD DEFINITION
