/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright � 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_fields.h"


#pragma region Datum DEFINITION

// CONSTRUCTORS/DESTRUCTOR

Datum::Datum() : Bytes(0) { }

Datum::Datum(RCDATUM other)
{
	this->Bytes = other.Bytes;
}

Datum::Datum(RRDATUM other)
{
	*this = other.Bytes;
}

Datum::Datum(PBYTE value) : Bytes(value) { }

Datum::Datum(PCHAR value) : String(value) { }

Datum::Datum(RBITPACK value) : BitsPtr(&value) { }

Datum::Datum(RCHAR value) : CharPtr(&value) { }

Datum::Datum(RBYTE value) : BytePtr(&value) { }

Datum::Datum(RBOOL value) : BoolPtr(&value) { }

Datum::Datum(RSHORT value) : ShortPtr(&value) { }

Datum::Datum(RWORD value) : WordPtr(&value) { }

Datum::Datum(RLONG value) : LongPtr(&value) { }
		
Datum::Datum(RDWORD value) : DWordPtr(&value) { }

Datum::Datum(RFLOAT value) : FloatPtr(&value) { }

Datum::~Datum()
{
	if (this->Bytes)
		delete this->Bytes;
}


// OPERATORS

RDATUM Datum::operator =(RCDATUM other)
{
	*this = Datum(other);
	return *this;
}

RDATUM Datum::operator =(RRDATUM other)
{
	*this = Datum(other);
	return *this;
}

#pragma endregion



#pragma region Field DEFINITION

// CONSTRUCTORS/DESTRUCTOR

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
}

Field::Field(RSHORT value)
{
}

Field::Field(RLONG value)
{
}

Field::Field(RFLOAT value)
{
}


// OPERATORS


// USER METHODS

CONST DataSize Field::GetDataSize() const
{
	return static_cast<DataSize>(MASK(_DataType, DATA_SIZE_MASK));
}

CONST DataType Field::GetDataType() const
{
	return _DataType;
}


// USER METHODS

template<typename TVal>
CONST TVal Field::GetValue() const
{
	return reinterpret_cast<TVal>(*(_Value.Bytes));
}

template<>
CONST CHAR Field::GetValue<CHAR>() const
{
	return static_cast<CCHAR>(*(_Value.CharPtr));
}

template<typename TVal>
VOID Field::SetValue(const TVal & value)
{

}

#pragma endregion
