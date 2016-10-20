/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright � 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_fields.h"


#pragma region Datum DEFINITION

// CONSTRUCTORS

Datum::Datum() : Bytes(0) { }

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

RDATUM Datum::operator =(PBYTE rValue)
{
	this->Bytes = rValue;
	return *this;
}

RDATUM Datum::operator =(PCHAR rValue)
{
	this->String = rValue;
	return *this;
}

RDATUM Datum::operator =(PBITPACK rValue)
{
	this->BitFields = rValue;
	return *this;
}

RDATUM Datum::operator =(RCCHAR rValue)
{
	*this->CharPtr = rValue;
	return *this;
}

RDATUM Datum::operator =(RCBYTE rValue)
{
	*this->BytePtr = rValue;
	return *this;
}

RDATUM Datum::operator =(RCBOOL rValue)
{
	*this->BoolPtr = rValue;
	return *this;
}

RDATUM Datum::operator =(RCSHORT rValue)
{
	*this->ShortPtr = rValue;
	return *this;
}

RDATUM Datum::operator =(RCWORD rValue)
{
	*this->WordPtr = rValue;
	return *this;
}

RDATUM Datum::operator =(RCLONG rValue)
{
	*this->LongPtr = rValue;
	return *this;
}

RDATUM Datum::operator =(RCDWORD rValue)
{
	*this->DWordPtr = rValue;
	return *this;
}

RDATUM Datum::operator =(RCFLOAT rValue)
{
	*this->FloatPtr = rValue;
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

Datum::operator RCCHAR() const
{
	return MAKE_CONST(*this->CharPtr);
}

Datum::operator RCHAR()
{
	return *this->CharPtr;
}

Datum::operator RCBYTE() const
{
	return MAKE_CONST(*this->BytePtr);
}

Datum::operator RBYTE()
{
	return *this->BytePtr;	
}

Datum::operator RCBOOL() const
{
	return MAKE_CONST(*this->BoolPtr);
}

Datum::operator RBOOL()
{
	return *this->BoolPtr;
}

Datum::operator RCSHORT() const
{
	return MAKE_CONST(*this->ShortPtr);
}

Datum::operator RSHORT()
{
	return *this->ShortPtr;
}

Datum::operator RCWORD() const
{
	return MAKE_CONST(*this->WordPtr);
}

Datum::operator RWORD()
{
	return *this->WordPtr;
}

Datum::operator RCLONG() const
{
	return MAKE_CONST(*this->LongPtr);
}

Datum::operator RLONG()
{
	return *this->LongPtr;
}

Datum::operator RCDWORD() const
{
	return MAKE_CONST(*this->DWordPtr);
}

Datum::operator RDWORD()
{
	return *this->DWordPtr;
}

Datum::operator RCFLOAT() const
{
	return MAKE_CONST(*this->FloatPtr);
}

Datum::operator RFLOAT()
{
	return *this->FloatPtr;
}


// USER METHODS

VOID Datum::FreePtr()
{
	if (this->Bytes)
		delete this->Bytes;
}

VOID Datum::FreeData()
{
	if (this->Bytes)
	{
		delete [] this->Bytes;
		delete this->Bytes;
	}
}

#pragma endregion



#pragma region Field DEFINITION

// CONSTRUCTORS

Field::Field(DataType dataType) : _DataType(dataType) { }

Field::Field(RCFIELD other)
{
	_Dispose = other._Dispose;
	_Value = other._Value;
	_DataType = other._DataType;
}

Field::Field(RRFIELD other)
{	
	new (this) Field(other._Value, other._DataType);
}

Field::Field(RCDATUM value, DataType dataType) : _Value(value), _DataType(dataType) { }

Field::Field(RCHAR value) : _Value(value), _DataType(DataType::CHAR_FIELD) { }

Field::Field(RBYTE value) : _Value(value), _DataType(DataType::BYTE_FIELD) { }

Field::Field(RBOOL value) : _Value(value), _DataType(DataType::BOOL_FIELD) { }

Field::Field(RSHORT value) : _Value(value), _DataType(DataType::SHORT_FIELD) { }

Field::Field(RWORD value) : _Value(value), _DataType(DataType::WORD_FIELD) { }

Field::Field(RLONG value) : _Value(value), _DataType(DataType::LONG_FIELD) { }
		
Field::Field(RDWORD value) : _Value(value), _DataType(DataType::DWORD_FIELD) { }

Field::Field(RFLOAT value) : _Value(value), _DataType(DataType::FLOAT_FIELD) { }

Field::~Field()
{
	if (_Dispose)
		_Value.FreePtr();
}


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

RFIELD Field::operator =(RCDATUM rValue)
{
	_Value = rValue;
	return *this;
}


// STATIC FUNCTIONS

RFIELD Field::NULL_OBJECT()
{
	STATIC Field NULL_FIELD;
	return NULL_FIELD;
}


// OPERATORS

Field::operator RCCHAR() const
{
	return _Value;
}

Field::operator RCHAR()
{
	return _Value;
}

Field::operator RCBYTE() const
{
	return _Value;
}

Field::operator RBYTE()
{
	return _Value;
}

Field::operator RCBOOL() const
{
	return _Value;
}

Field::operator RBOOL()
{
	return _Value;
}

Field::operator RCSHORT() const
{
	return _Value;
}

Field::operator RSHORT()
{
	return _Value;
}

Field::operator RCWORD() const
{
	return _Value;
}

Field::operator RWORD()
{
	return _Value;
}

Field::operator RCLONG() const
{
	return _Value;
}

Field::operator RLONG()
{
	return _Value;
}

Field::operator RCDWORD() const
{
	return _Value;
}

Field::operator RDWORD()
{
	return _Value;
}

Field::operator RCFLOAT() const
{
	return _Value;
}

Field::operator RFLOAT()
{
	return _Value;
}


// IField IMPLEMENTATIONS

CSIZE Field::Size() const
{
	return sizeof(Field);
}

CSIZE Field::FieldSize() const
{
	return sizeof(DataType) + this->GetDataSize();
}

CONST DataSize Field::GetDataSize() const
{
	return static_cast<DataSize>(MASK(_DataType, DATA_SIZE_MASK));
}

CONST DataType Field::GetDataType() const
{
	return _DataType;
}
		
PCBYTE Field::Bytes() const
{
	return _Value.Bytes;
}

PCCHAR Field::String() const
{
	return _Value.String;
}

#pragma endregion



#pragma region VarLengthField DEFINITION

// CONSTRUCTORS/DESTRUCTOR

VarLengthField::VarLengthField(CSIZE length, DataType dataType) : Field(dataType)
{
	switch (dataType)
	{
	case DataType::BYTES_FIELD:
		_Dispose = TRUE;
		_Value = new BYTE[length];
		_Length = length;
		break;

	case DataType::STRING_FIELD:
		_Dispose = TRUE;
		_Value = new CHAR[length];
		_Length = length;
		break;

	case DataType::BIT_FIELD:
		_Dispose = TRUE;
		_Value = new BITPACK[length];
		_Length = length;
		break;

	default:
		break;
	}
}

VarLengthField::VarLengthField(RCVARLENGTHFIELD other)
{
	_Dispose = other._Dispose;
	_Value = other._Value;
	_DataType = other._DataType;
	_Length = other._Length;
}

VarLengthField::VarLengthField(RRVARLENGTHFIELD other)
{	
	new (this) VarLengthField(other._Value, other._DataType);
}

VarLengthField::VarLengthField(RCDATUM value, DataType dataType) : Field(value, dataType)
{
	if (dataType == DataType::BYTES_FIELD || dataType == DataType::STRING_FIELD || dataType == DataType::BIT_FIELD)
		_Length = SIZEOF(*(PBYTE)_Value);
}

VarLengthField::VarLengthField(PBYTE value) : Field(DataType::BYTES_FIELD)
{
	_Length = SIZEOF(*(PBYTE)_Value);
}

VarLengthField::VarLengthField(PCHAR value) : Field(DataType::STRING_FIELD)
{
	_Length = SIZEOF(*(PBYTE)_Value);
}

VarLengthField::VarLengthField(PBITPACK value) : Field(DataType::BIT_FIELD)
{
	_Length = SIZEOF(*(PBYTE)_Value);
}

VarLengthField::~VarLengthField()
{
	if (_Dispose)
	{
		if (_Length > 0)
			_Value.FreeData();
		else
			_Value.FreePtr();
	}
}


// OPERATORS

RVARLENGTHFIELD VarLengthField::operator =(RCVARLENGTHFIELD rValue)
{
	*this = VarLengthField(rValue);
	return *this;
}

RVARLENGTHFIELD VarLengthField::operator =(RRVARLENGTHFIELD rValue)
{
	*this = VarLengthField(rValue);
	return *this;
}


VarLengthField::operator PCBYTE() const
{
	return _Value;
}

VarLengthField::operator PBYTE()
{
	return _Value;
}

VarLengthField::operator PCCHAR() const
{
	return _Value;
}

VarLengthField::operator PCHAR()
{
	return _Value;
}

VarLengthField::operator PCBITPACK() const
{
	return _Value;
}

VarLengthField::operator PBITPACK()
{
	return _Value;
}


// Field OVERRIDES

CSIZE VarLengthField::Size() const
{
	return sizeof(VarLengthField);
}

CSIZE VarLengthField::FieldSize() const
{
	if (_Length > 0)
		return _Length;

	return this->GetDataSize();
}

#pragma endregion