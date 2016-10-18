/***********************************************************************************************
* [Foxetron_fields.h]: UNIVERSAL DATA STRUCTURES FOR STORING SETTINGS & MESSAGE FIELDS
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_FIELDS_H
#define _FOXETRON_FIELDS_H


#include "IttyBitty_util.h"


namespace Foxetron
{
	
#pragma region ENUMS
	
	enum DataSize : byte
	{
		ONE_BYTE	= 0x10,
		TWO_BYTES	= 0x20,
		THREE_BYTES	= 0x40,
		FOUR_BYTES	= 0x80,
	};

	enum DataType : byte
	{
		BYTES	= 0x0,

		BYTE	= ONE_BYTE | 0x1,
		CHAR	= ONE_BYTE | 0x2,
		BOOL	= ONE_BYTE | 0x4,

		WORD	= TWO_BYTES | 0x1,
		SHORT	= TWO_BYTES | 0x2,
		BITS	= TWO_BYTES | 0x4,

		DWORD	= FOUR_BYTES | 0x1,
		LONG	= FOUR_BYTES | 0x2,
		FLOAT	= FOUR_BYTES | 0x4
	};

#pragma endregion ENUMS



#pragma region FORWARD DECLARATIONS & TYPE ALIASES


	//  TEMPLATED TYPE FORWARD DECLARATIONS & ALIASES

	typedef union _Datum Datum, DATUM, *PDATUM, &RDATUM;
	typedef const union _Datum CDATUM, *CPDATUM, &CRDATUM;

	template<typename T = Datum>
	class TypedField;
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
	using CTYPEDFIELD = const TypedField<T>;
	template<typename T = Datum>
	using PCTYPEDFIELD = const TypedField<T> *;
	template<typename T = Datum>
	using RCTYPEDFIELD = const TypedField<T> &;
	template<typename T = Datum>
	using PPCTYPEDFIELD = const TypedField<T> **;

#pragma endregion FORWARD DECLARATIONS & TYPE ALIASES



#pragma region TYPE DECLARATIONS

	// Datum: UNIVERSAL 4-BYTE DATA TYPE UNION

	union _Datum
	{
		byte Bytes[4];

		struct
		{
			bool b0 : 1;
			bool b1 : 1;
			bool b2 : 1;
			bool b3 : 1;
			bool b4 : 1;
			bool b5 : 1;
			bool b6 : 1;
			bool b7 : 1;
			bool b8 : 1;
			bool b9 : 1;
			bool bA : 1;
			bool bB : 1;
			bool bC : 1;
			bool bD : 1;
			bool bE : 1;
			bool bF : 1;
		}
		Bits;

		byte ByteVal;
		char CharVal;
		bool BoolVal;

		word WordVal;
		short ShortVal;

		::DWORD DWordVal;
		long LongVal;
		float FloatVal;
	};


	// IField INTERFACE

	class IField
	{
	public:

		//operator byte()

		template<typename TVal = Datum>
		const TVal GetValue() const;

		template<typename TVal = Datum>
		void SetValue(TVal &);

	protected:

		Datum _Value;
		DataType _Type;
	};

	typedef IField IFIELD, *PIFIELD, &RIFIELD;
	typedef const IField CIFIELD, *CPIFIELD, &CRIFIELD;


	// Field

	class Field : public virtual IField
	{
	public:

		Field(Datum &, DataType = DataType::BYTES);

		Field(byte *, DataType = DataType::BYTES);

		Field(char &, DataType = DataType::CHAR);
		Field(short &, DataType = DataType::SHORT);
		Field(long &, DataType = DataType::LONG);
		Field(float &, DataType = DataType::FLOAT);

		//operator byte()

		template<typename TVal = Datum>
		const TVal GetValue() const;

		template<typename TVal= Datum>
		void SetValue(TVal &);
		
	protected:

		Datum _Value;
		DataType _Type;
	};

	template<>
	const char Field::GetValue<char>() const;

	typedef Field FIELD, *PFIELD, &RFIELD;
	typedef const Field CFIELD, *CPFIELD, &CRFIELD;


	// TypedField

	template<typename T>
	class TypedField : public virtual IField
	{
	public:

		TypedField(const Datum &)
		{
		}

		TypedField(const T &)
		{

		}

		typedef T Type;

		const T Value() const
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
