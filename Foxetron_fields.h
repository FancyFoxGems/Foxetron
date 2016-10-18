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

		WORD_FIELD	= TWO_BYTES | 0x1,
		SHORT_FIELD	= TWO_BYTES | 0x2,
		BIT_FIELD	= TWO_BYTES | 0x4,

		DWORD_FIELD	= FOUR_BYTES | 0x1,
		LONG_FIELD	= FOUR_BYTES | 0x2,
		FLOAT_FIELD	= FOUR_BYTES | 0x4
	};

#pragma endregion ENUMS



#pragma region FORWARD DECLARATIONS & TYPE ALIASES


	//  TEMPLATED TYPE FORWARD DECLARATIONS & ALIASES

	typedef VOLATILE UNION _Datum Datum, DATUM, *PDATUM, &RDATUM;
	typedef CONST UNION _Datum CDATUM, *CPDATUM, &CRDATUM;

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

#pragma endregion FORWARD DECLARATIONS & TYPE ALIASES



#pragma region TYPE DECLARATIONS

	// Datum: UNIVERSAL 4-BYTE DATA TYPE UNION
	
	UNION PACKED _Datum
	{
		BYTE Bytes[4];

		BITPACK	Bits;

		BYTE ByteVal;
		CHAR CharVal;
		BOOL BoolVal;

		WORD WordVal;
		SHORT ShortVal;

		DWORD DWordVal;
		LONG LongVal;
		FLOAT FloatVal;

		_Datum() { }

		_Datum(RDATUM other)
		{
			this->DWordVal = other.DWordVal;
		}
	};


	// IField INTERFACE

	CLASS IField
	{
	public:

		VIRTUAL CSIZE Size() const = 0;
		
		VIRTUAL CSIZE ByteSize() const = 0;

		VIRTUAL CONST DataSize GetDataSize() const = 0;

		VIRTUAL CONST DataType GetDataType() const = 0;
	};

	typedef IField IFIELD, *PIFIELD, &RIFIELD;
	typedef CONST IField CIFIELD, *CPIFIELD, &CRIFIELD;


	// Field

	CLASS Field : public VIRTUAL IField
	{
	public:

		Field(RDATUM, DataType = DataType::BYTES_FIELD);

		Field(PBYTE, DataType = DataType::BYTES_FIELD);

		Field(RCHAR, DataType = DataType::CHAR_FIELD);
		Field(RSHORT, DataType = DataType::SHORT_FIELD);
		Field(RLONG, DataType = DataType::LONG_FIELD);
		Field(RFLOAT, DataType = DataType::FLOAT_FIELD);

		operator BYTE() const;

		VIRTUAL CSIZE Size() const;
		
		VIRTUAL CSIZE ByteSize() const;

		VIRTUAL CONST DataSize GetDataSize() const;

		VIRTUAL CONST DataType GetDataType() const;

		template<typename TVal = Datum>
		CONST TVal GetValue() const;

		template<typename TVal= Datum>
		VOID SetValue(TVal &);
		
	protected:

		DATUM _Value;
		DataType _DataType;
	};

	template<>
	CONST CHAR Field::GetValue<CHAR>() const;

	typedef Field FIELD, *PFIELD, &RFIELD;
	typedef CONST Field CFIELD, *CPFIELD, &CRFIELD;


	// TypedField

	template<typename T>
	CLASS TypedField : public VIRTUAL IField
	{
	public:

		TypedField(CONST Datum &)
		{
		}

		TypedField(CONST T &)
		{

		}

		typedef T Type;

		operator CONST T() const;
		operator SIGNED_TYPE(CONST T)() const;

		CSIZE Size() const;
		
		CSIZE ByteSize() const;

		CONST DataSize GetDataSize() const;

		CONST DataType GetDataType() const;

		CONST T Value() const
		{
			return reinterpret_cast<T>(_Value.ByteVal);
		}

	protected:

		Datum _Value;
	};

#pragma endregion TYPE DECLARATIONS

}

using namespace Foxetron;


#endif
