/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_fields.h"


#pragma region Datum DEFINITION

// CONSTRUCTORS

Datum::Datum() { }

//Datum::Datum(CBYTE byteSize)
//{
//	this->Bytes = new byte[byteSize];
//}

Datum::Datum(RCDATUM other)
{
	this->Bytes = other.Bytes;
}

Datum::Datum(RRDATUM other)
{	
	new (this) Datum(other.Bytes);
}

Datum::Datum(PBYTE value) : Bytes(value) { }

Datum::Datum(PCHAR value) : String(value) { }

Datum::Datum(PBITPACK value) : BitFields(value) { }

Datum::Datum(RCHAR value) : CharPtr(&value) { }

Datum::Datum(RBYTE value) : BytePtr(&value) { }

Datum::Datum(RBOOL value) : BoolPtr(&value) { }

Datum::Datum(RSHORT value) : ShortPtr(&value) { }

Datum::Datum(RWORD value) : WordPtr(&value) { }

Datum::Datum(RLONG value) : LongPtr(&value) { }
		
Datum::Datum(RDWORD value) : DWordPtr(&value) { }

Datum::Datum(RFLOAT value) : FloatPtr(&value) { }


// OPERATORS

RDATUM Datum::operator =(RCDATUM rValue)
{
	*this = Datum(rValue);
	return *this;
}

RDATUM Datum::operator =(RRDATUM rValue)
{
	*this = Datum(rValue);
	return *this;
}

Datum::operator PCBYTE() const
{
	return MAKE_CONST(this->Bytes);
}

Datum::operator PBYTE()
{
	return this->Bytes;
}

Datum::operator PCCHAR() const
{
	return MAKE_CONST(this->String);
}

Datum::operator PCHAR()
{
	return this->String;
}

Datum::operator PCBITPACK() const
{
	return MAKE_CONST(this->BitFields);
}

Datum::operator PBITPACK()
{
	return this->BitFields;
}

Datum::operator CCHAR() const
{
	return MAKE_CONST(*this->CharPtr);
}

Datum::operator RCHAR()
{
	return *this->CharPtr;
}

Datum::operator CBYTE() const
{
	return MAKE_CONST(*this->BytePtr);
}

Datum::operator RBYTE()
{
	return *this->BytePtr;	
}

Datum::operator CBOOL() const
{
	return MAKE_CONST(*this->BoolPtr);
}

Datum::operator RBOOL()
{
	return *this->BoolPtr;
}

Datum::operator CSHORT() const
{
	return MAKE_CONST(*this->ShortPtr);
}

Datum::operator RSHORT()
{
	return *this->ShortPtr;
}

Datum::operator CWORD() const
{
	return MAKE_CONST(*this->WordPtr);
}

Datum::operator RWORD()
{
	return *this->WordPtr;
}

Datum::operator CLONG() const
{
	return MAKE_CONST(*this->LongPtr);
}

Datum::operator RLONG()
{
	return *this->LongPtr;
}

Datum::operator CDWORD() const
{
	return MAKE_CONST(*this->DWordPtr);
}

Datum::operator RDWORD()
{
	return *this->DWordPtr;
}

Datum::operator CFLOAT() const
{
	return MAKE_CONST(*this->FloatPtr);
}

Datum::operator RFLOAT()
{
	return *this->FloatPtr;
}


// USER METHODS

//VOID Datum::FreePtr()
//{
//	if (this->Bytes)
//		delete this->Bytes;
//}
//
//VOID Datum::FreeData()
//{
//	if (this->Bytes)
//	{
//		delete [] this->Bytes;
//		delete this->Bytes;
//	}
//}

#pragma endregion



#pragma region Field DEFINITION

// CONSTRUCTORS

Field::Field(DataType dataType) : _DataType(dataType) { }

Field::Field(RCFIELD other)
{
	this->_Value = other._Value;
	this->_DataType = other._DataType;
}

Field::Field(RRFIELD other)
{	
	new (this) Field(other._Value, other._DataType);
}

Field::Field(RCDATUM value, DataType dataType) : _Value(value), _DataType(dataType) { }

Field::Field(PBYTE value) : _Value(value), _DataType(DataType::BYTES_FIELD) { }

Field::Field(PCHAR value) : _Value(value), _DataType(DataType::STRING_FIELD) { }

Field::Field(PBITPACK value) : _Value(value), _DataType(DataType::BIT_FIELD) { }

Field::Field(RCHAR value) : _Value(value), _DataType(DataType::CHAR_FIELD) { }

Field::Field(RBYTE value) : _Value(value), _DataType(DataType::BYTE_FIELD) { }

Field::Field(RBOOL value) : _Value(value), _DataType(DataType::BOOL_FIELD) { }

Field::Field(RSHORT value) : _Value(value), _DataType(DataType::SHORT_FIELD) { }

Field::Field(RWORD value) : _Value(value), _DataType(DataType::WORD_FIELD) { }

Field::Field(RLONG value) : _Value(value), _DataType(DataType::LONG_FIELD) { }
		
Field::Field(RDWORD value) : _Value(value), _DataType(DataType::DWORD_FIELD) { }

Field::Field(RFLOAT value) : _Value(value), _DataType(DataType::FLOAT_FIELD) { }


// OPERATORS

RFIELD Field::operator =(RCFIELD rValue)
{
	*this = Field(rValue);
	return *this;
}

RFIELD Field::operator =(RRFIELD rValue)
{
	*this = Field(rValue);
	return *this;
}

Field::operator PCBYTE() const
{
	return _Value;
}

Field::operator PBYTE()
{
	return _Value;
}

Field::operator PCCHAR() const
{
	return _Value;
}

Field::operator PCHAR()
{
	return _Value;
}

Field::operator PCBITPACK() const
{
	return _Value;
}

Field::operator PBITPACK()
{
	return _Value;
}

Field::operator CCHAR() const
{
	return _Value;
}

Field::operator RCHAR()
{
	return _Value;
}

Field::operator CBYTE() const
{
	return _Value;
}

Field::operator RBYTE()
{
	return _Value;
}

Field::operator CBOOL() const
{
	return _Value;
}

Field::operator RBOOL()
{
	return _Value;
}

Field::operator CSHORT() const
{
	return _Value;
}

Field::operator RSHORT()
{
	return _Value;
}

Field::operator CWORD() const
{
	return _Value;
}

Field::operator RWORD()
{
	return _Value;
}

Field::operator CLONG() const
{
	return _Value;
}

Field::operator RLONG()
{
	return _Value;
}

Field::operator CDWORD() const
{
	return _Value;
}

Field::operator RDWORD()
{
	return _Value;
}

Field::operator CFLOAT() const
{
	return _Value;
}

Field::operator RFLOAT()
{
	return _Value;
}


// STATIC FUNCTIONS

RFIELD Field::NULL_OBJECT()
{
	STATIC Field NULL_FIELD((PBYTE)0);
	return NULL_FIELD;
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


#pragma endregion
