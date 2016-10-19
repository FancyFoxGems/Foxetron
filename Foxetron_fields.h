/***********************************************************************************************
* [Foxetron_fields.h]: UNIVERSAL DATA STRUCTURES FOR STORING SETTINGS & MESSAGE FIELDS
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_FIELDS_H
#define _FOXETRON_FIELDS_H


#include "IttyBitty_bits.h"

using namespace IttyBitty;


namespace Foxetron
{
#pragma region FORWARD DECLARATIONS & TYPE ALIASES

	typedef VOLATILE UNION Datum DATUM, * PDATUM, & RDATUM;
	typedef CONST UNION Datum CDATUM, * PCDATUM, & RCDATUM;

	CLASS IField;
	typedef IField IFIELD, *PIFIELD, &RIFIELD;
	typedef CONST IField CIFIELD, *PCIFIELD, &RCIFIELD;

	CLASS Field;
	typedef Field FIELD, *PFIELD, &RFIELD;
	typedef CONST Field CFIELD, *PCFIELD, &RCFIELD;

	template<typename T = Datum>
	CLASS TypedField;
	template<typename T = Datum>
	using TYPEDFIELD = TypedField<T>;
	template<typename T = Datum>
	using PTYPEDFIELD = TypedField<T> *;
	template<typename T = Datum>
	using RTYPEDFIELD = TypedField<T> &;
	template<typename T = Datum>
	using PPTYPEDFIELD = TypedField<T> **;
	template<typename T = Datum>
	using RRTYPEDFIELD = TypedField<T> &&;
	template<typename T = Datum>
	using CTYPEDFIELD = CONST TypedField<T>;
	template<typename T = Datum>
	using PCTYPEDFIELD = CONST TypedField<T> *;
	template<typename T = Datum>
	using RCTYPEDFIELD = CONST TypedField<T> &;
	template<typename T = Datum>
	using PPCTYPEDFIELD = CONST TypedField<T> **;

#pragma endregion


#pragma region ENUMS
	
	enum DataSize : BYTE
	{
		ONE_BYTE	= 0x10,
		TWO_BYTES	= 0x20,
		THREE_BYTES	= 0x40,
		FOUR_BYTES	= 0x80,
	};

	enum DataType : BYTE
	{
		BYTES_FIELD	= 0x0,

		BYTE_FIELD	= ONE_BYTE | 0x1,
		CHAR_FIELD	= ONE_BYTE | 0x2,
		BOOL_FIELD	= ONE_BYTE | 0x4,
		BIT_FIELD	= ONE_BYTE | 0x8,

		WORD_FIELD	= TWO_BYTES | 0x1,
		SHORT_FIELD	= TWO_BYTES | 0x2,

		DWORD_FIELD	= FOUR_BYTES | 0x1,
		LONG_FIELD	= FOUR_BYTES | 0x2,
		FLOAT_FIELD	= FOUR_BYTES | 0x4
	};

#pragma endregion


#pragma region Datum: UNIVERSAL 4-BYTE DATA TYPE UNION
	
	UNION PACKED Datum
	{
		BYTE Bytes[4];

		BITPACK	Bits;
		
		CHAR CharVal;
		BYTE ByteVal;
		BOOL BoolVal;
		
		SHORT ShortVal;
		WORD WordVal;
		
		LONG LongVal;
		DWORD DWordVal;
		FLOAT FloatVal;

		Datum() { }

		Datum(RCDATUM other)
		{
			this->LongVal = other.LongVal;
		}

		 Datum(PBYTE);

		 Datum(RBITPACK);

		 Datum(RCHAR);
		 Datum(RBYTE);
		 Datum(RBOOL);
		 Datum(RSHORT);
		 Datum(RWORD);
		 Datum(RLONG);
		 Datum(RDWORD);
		 Datum(RFLOAT);

		RDATUM operator =(RCDATUM other)
		{
			this->LongVal = other.LongVal;
			return *this;
		}
	};

#pragma endregion


#pragma region IField INTERFACE

	CLASS IField
	{
	public:

		VIRTUAL CSIZE Size() const = 0;
		
		VIRTUAL CSIZE ByteSize() const = 0;

		VIRTUAL CONST DataSize GetDataSize() const = 0;

		VIRTUAL CONST DataType GetDataType() const = 0;

		VIRTUAL PCBYTE Bytes() const = 0;
	};

#pragma endregion


#pragma region Field DECLARATION

	CLASS Field : public virtual IField
	{
	public:

		STATIC RFIELD NULL_OBJECT();

		// CONSTRUCTORS/DESTRUCTOR

		Field(RCFIELD);

		Field(RCDATUM, DataType = DataType::BYTES_FIELD);

		EXPLICIT Field(PBYTE, DataType = DataType::BYTES_FIELD);

		EXPLICIT Field(RBITPACK, DataType = DataType::BIT_FIELD);

		EXPLICIT Field(RCHAR);
		EXPLICIT Field(RBYTE);
		EXPLICIT Field(RBOOL);
		EXPLICIT Field(RSHORT);
		EXPLICIT Field(RWORD);
		EXPLICIT Field(RLONG);
		EXPLICIT Field(RDWORD);
		EXPLICIT Field(RFLOAT);

		VIRTUAL ~Field();

		RFIELD operator =(RCFIELD other);
		
		operator PCBYTE() const;
		operator PBYTE();
		operator CBITPACK() const;
		operator RBITPACK();

		operator CCHAR() const;
		operator RCHAR();
		operator CBYTE() const;
		operator RBYTE();
		operator CBOOL() const;
		operator RBOOL();
		operator CSHORT() const;
		operator RSHORT();
		operator CWORD() const;
		operator RWORD();
		operator CLONG() const;
		operator RLONG();
		operator CDWORD() const;
		operator RDWORD();
		operator CFLOAT() const;
		operator RFLOAT();

		VIRTUAL CSIZE Size() const;
		
		VIRTUAL CSIZE ByteSize() const;

		VIRTUAL CONST DataSize GetDataSize() const;

		VIRTUAL CONST DataType GetDataType() const;
		
		VIRTUAL PCBYTE Bytes() const;

		template<typename TVal = Datum>
		CONST TVal GetValue() const;

		template<typename TVal= Datum>
		VOID SetValue(const TVal &);
		
	protected:

		DATUM _Value;
		DataType _DataType;
	};

	template<>
	CONST CHAR Field::GetValue<CHAR>() const;

#pragma endregion


#pragma region TypedField DEFINITION

	template<typename T>
	CLASS TypedField : public virtual IField
	{
	public:

		// CONSTRUYCTORS

		TypedField(RCDATUM value)
		{
		}

		TypedField(CONST T & value)
		{

		}
		

		// OPERATORS

		operator CONST T() const;
		operator SIGNED_TYPE(CONST T)() const;


		//  VIRTUAL HIDING IMPLEMENATIONS

		CSIZE Size() const;
		
		CSIZE ByteSize() const;


		// ACCESSORS/MUTATORS

		CONST DataSize GetDataSize() const;

		CONST DataType GetDataType() const;


		// USER METHODS

		CONST T GetValue() const
		{
			return reinterpret_cast<T>(_Value.ByteVal);
		}

		VOID SetValue(CONST T & value)
		{

		}


		// META-MEMBERS

		typedef T Type;

	protected:

		// INSTANCE VARIABLES

		DATUM _Value;
	};

#pragma endregion
}

using namespace Foxetron;


#endif
