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

	typedef union Datum DATUM, * PDATUM, & RDATUM, ** PPDATUM, && RRDATUM;
	typedef const union Datum CDATUM, * PCDATUM, & RCDATUM, ** PPCDATUM;

	class IField;
	typedef IField IFIELD, * PIFIELD, & RIFIELD, ** PPIFIELD, && RRIFIELD;
	typedef const IField CIFIELD, *PCIFIELD, & RCIFIELD, ** PPCIFIELD;

	class Field;
	typedef Field FIELD, * PFIELD, & RFIELD, ** PPFIELD, && RRFIELD;
	typedef const Field CFIELD, * PCFIELD, & RCFIELD, ** PPCFIELD;

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

#pragma endregion


#pragma region ENUMS
	
	enum DataSize : BYTE
	{
		WHO_KNOWS	= 0x0,
		ONE_BYTE	= 0x10,
		TWO_BYTES	= 0x20,
		THREE_BYTES	= 0x40,
		FOUR_BYTES	= 0x80,
	};

	enum DataType : BYTE
	{
		BYTES_FIELD		= WHO_KNOWS | 0x0,
		STRING_FIELD	= WHO_KNOWS | 0x1,
		BIT_FIELD		= WHO_KNOWS | 0x2,

		BYTE_FIELD		= ONE_BYTE | 0x1,
		CHAR_FIELD		= ONE_BYTE | 0x2,
		BOOL_FIELD		= ONE_BYTE | 0x4,

		WORD_FIELD		= TWO_BYTES | 0x1,
		SHORT_FIELD		= TWO_BYTES | 0x2,

		DWORD_FIELD		= FOUR_BYTES | 0x1,
		LONG_FIELD		= FOUR_BYTES | 0x2,
		FLOAT_FIELD		= FOUR_BYTES | 0x4
	};

	STATIC CONST BYTE DATA_SIZE_MASK = 0xF0;

#pragma endregion


#pragma region Datum DECLARATION: UNIVERSAL 4-BYTE DATA TYPE UNION
	
	UNION PACKED Datum
	{
		// UNION MEMBERS

		PBYTE Bytes;
		PCHAR String;	
		PBITPACK BitFields;
		
		PCHAR CharPtr;
		PBYTE BytePtr;
		PBOOL BoolPtr;
		
		PSHORT ShortPtr;
		PWORD WordPtr;
		
		PLONG LongPtr;
		PDWORD DWordPtr;
		PFLOAT FloatPtr;


		// CONSTRUCTORS

		Datum();

		Datum(RCDATUM other);
		Datum(RRDATUM other);

		EXPLICIT Datum(PBYTE value);
		EXPLICIT Datum(PCHAR value);		
		EXPLICIT Datum(PBITPACK value);

		EXPLICIT Datum(RCHAR value);
		EXPLICIT Datum(RBYTE value);
		EXPLICIT Datum(RBOOL value);
		EXPLICIT Datum(RSHORT value);
		EXPLICIT Datum(RWORD value);
		EXPLICIT Datum(RLONG value);
		EXPLICIT Datum(RDWORD value);
		EXPLICIT Datum(RFLOAT value);


		//OPERATORS

		RDATUM operator =(RCDATUM other);
		RDATUM operator =(RRDATUM other);

		RDATUM operator =(PBYTE other);
		RDATUM operator =(PCHAR other);
		RDATUM operator =(PBITPACK other);

		RDATUM operator =(RCCHAR other);
		RDATUM operator =(RCBYTE other);
		RDATUM operator =(RCBOOL other);
		RDATUM operator =(RCSHORT other);
		RDATUM operator =(RCWORD other);
		RDATUM operator =(RCLONG other);
		RDATUM operator =(RCDWORD other);
		RDATUM operator =(RCFLOAT other);
		
		operator PCBYTE() const;
		operator PBYTE();
		operator PCCHAR() const;
		operator PCHAR();
		operator PCBITPACK() const;
		operator PBITPACK();

		operator RCCHAR() const;
		operator RCHAR();
		operator RCBYTE() const;
		operator RBYTE();
		operator RCBOOL() const;
		operator RBOOL();
		operator RCSHORT() const;
		operator RSHORT();
		operator RCWORD() const;
		operator RWORD();
		operator RCLONG() const;
		operator RLONG();
		operator RCDWORD() const;
		operator RDWORD();
		operator RCFLOAT() const;
		operator RFLOAT();


		// USER METHODS

		VOID FreePtr();
		VOID FreeData();
	};

#pragma endregion


#pragma region IField INTERFACE

	CLASS IField
	{
	public:

		// INTERFACE METHODS

		VIRTUAL CSIZE Size() const = 0;		
		VIRTUAL CSIZE ByteSize() const = 0;

		VIRTUAL CONST DataSize GetDataSize() const = 0;
		VIRTUAL CONST DataType GetDataType() const = 0;

		VIRTUAL PCBYTE Bytes() const = 0;
		VIRTUAL PCCHAR String() const = 0;
	};

#pragma endregion


#pragma region Field DECLARATION

	CLASS Field : public virtual IField
	{
	public:

		// CONSTRUCTORS/DESTRUCTOR

		Field(DataType = DataType::BYTE_FIELD);

		Field(RCFIELD);
		Field(RRFIELD);

		Field(RCDATUM, DataType = DataType::BYTE_FIELD);

		EXPLICIT Field(PBYTE);
		EXPLICIT Field(PCHAR);
		EXPLICIT Field(PBITPACK);

		EXPLICIT Field(RCHAR);
		EXPLICIT Field(RBYTE);
		EXPLICIT Field(RBOOL);
		EXPLICIT Field(RSHORT);
		EXPLICIT Field(RWORD);
		EXPLICIT Field(RLONG);
		EXPLICIT Field(RDWORD);
		EXPLICIT Field(RFLOAT);

		VIRTUAL ~Field();


		// STATIC FUNCTIONS

		STATIC RFIELD NULL_OBJECT();
		
		
		// OPERATORS

		RFIELD operator =(RCFIELD);
		RFIELD operator =(RRFIELD);

		RFIELD operator =(RCDATUM);
		
		operator PCBYTE() const;
		operator PBYTE();
		operator PCCHAR() const;
		operator PCHAR();
		operator PCBITPACK() const;
		operator PBITPACK();

		operator RCCHAR() const;
		operator RCHAR();
		operator RCBYTE() const;
		operator RBYTE();
		operator RCBOOL() const;
		operator RBOOL();
		operator RCSHORT() const;
		operator RSHORT();
		operator RCWORD() const;
		operator RWORD();
		operator RCLONG() const;
		operator RLONG();
		operator RCDWORD() const;
		operator RDWORD();
		operator RCFLOAT() const;
		operator RFLOAT();


		// IField IMPLEMENTATIONS

		VIRTUAL CSIZE Size() const;		
		VIRTUAL CSIZE ByteSize() const;

		VIRTUAL CONST DataSize GetDataSize() const;
		VIRTUAL CONST DataType GetDataType() const;
		
		VIRTUAL PCBYTE Bytes() const;		
		VIRTUAL PCCHAR String() const;
		

	protected:

		// INSTANCE VARIABLES

		Datum _Value;
		DataType _DataType;
	};

#pragma endregion


#pragma region TypedField DEFINITION

	template<typename T>
	CLASS TypedField : public virtual IField
	{
	public:

		// META-MEMBERS

		typedef T Type;


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


		//  VIRTUAL HIDING IField IMPLEMENATIONS

		CSIZE Size() const;
		
		CSIZE ByteSize() const;


		// ACCESSORS/MUTATORS

		CONST DataSize GetDataSize() const;

		CONST DataType GetDataType() const;


		// USER METHODS

		CONST T GetValue() const
		{
			return reinterpret_cast<T>(*_Value.Bytes);
		}

		VOID SetValue(CONST T & value)
		{

		}


	protected:

		// INSTANCE VARIABLES

		DATUM _Value;
	};

#pragma endregion
}

using namespace Foxetron;


#endif
