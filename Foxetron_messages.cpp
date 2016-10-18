/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_messages.h"


#pragma region FIELD DEFINITION

// CONSTRUCTOR/DESTRUCTOR

Field::Field(DataType, Datum)
{
}

Field::~Field()
{
}


// METHODS

template<typename T>
const T Field::Value() const
{
	return 0;
}

#pragma endregion FIELD DEFINITION


#pragma region MESSAGE DEFINITION

// STATIC MESSAGE "OVERRIDES"

template<class TMessage, MessageCode TCode>
constexpr MessageCode Message<TMessage, TCode>::TYPE()
{
	return TCode;
}

template<class TMessage, MessageCode TCode>
constexpr word Message<TMessage, TCode>::SIZE()
{
	return static_cast<word>(sizeof(TMessage));
}


// CONSTRUCTORS/DESTRUCTOR

template<class TMessage, MessageCode TCode>
Message<TMessage, TCode>::Message()
{
}

template<class TMessage, MessageCode TCode>
Message<TMessage, TCode>::Message(RFIELD)
{
}

template<class TMessage, MessageCode TCode>
Message<TMessage, TCode>::Message(PFIELD)
{
}

template<class TMessage, MessageCode TCode>
Message<TMessage, TCode>::~Message()
{
}


// OPERATORS

template<class TMessage, MessageCode TCode>
RFIELD Message<TMessage, TCode>::operator[](size_t i)
{
	return GetParam(i);
}


// PUBLIC METHODS

template<class TMessage, MessageCode TCode>
size_t Message<TMessage, TCode>::ParamCount() const
{
	return _Params == NULL ? 0 : static_cast<size_t>(countof(_Params));
}

template<class TMessage, MessageCode TCode>
RFIELD Message<TMessage, TCode>::GetParam(size_t i)
{
	if (_Params == NULL)
	{
		static FIELD NULL_FIELD;
		return NULL_FIELD;
	}

	return _Params[i];
}


// PROTECTED METHODS

template<class TMessage, MessageCode TCode>
void Message<TMessage, TCode>::RetrieveParamValue(pvoid, byte)
{
}

#pragma endregion MESSAGE DEFINITION


#pragma region REQUEST DEFINITIONS

#pragma endregion REQUEST DEFINITIONS

