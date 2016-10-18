/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_messages.h"


#pragma region FIELD DEFINITION

// CONSTRUCTORS

Field::Field(const Datum & value, DataType type) : _Value(value), _Type(type) { }

Field::Field(byte * value, DataType type) : _Type(type)
{
	for (byte i = 0; i < sizeof(value); i++)
		_Value.Bytes[i] = value[i];
}

Field::Field(const char & value, DataType type) : _Type(type)
{
	_Value.CharVal = value;
}

Field::Field(const short & value, DataType type) : _Type(type)
{
	_Value.ShortVal = value;
}

Field::Field(const long & value, DataType type) : _Type(type)
{
	_Value.LongVal = value;
}

Field::Field(const float & value, DataType type) : _Type(type)
{
	_Value.FloatVal = value;
}


// METHODS

template<typename T>
T & Field::Value()
{
	return _Value;
}

template<>
char & Field::Value<char>()
{
	return _Value.CharVal;
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
		static DATUM NULL_DATUM;
		static FIELD NULL_FIELD(NULL_DATUM);
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

// ANGLE REQUEST

NewAngleRequest::NewAngleRequest(const word degrees)
{
}

const word NewAngleRequest::Degrees() const
{
	return 0;
}

#pragma endregion REQUEST DEFINITIONS



#pragma region RESPONSE DEFINITIONS

Response::Response(const Error)
{
}

const Error Response::ErrorCode() const
{
	return Error();
}

const word AngleResponse::Degrees() const
{
	return 0;
}

StatusResponse::StatusResponse(const char *)
{
}

const char * StatusResponse::StatusMessage() const
{
	return nullptr;
}

ControllerStatusResponse::ControllerStatusResponse(ControllerStatus, const char *)
{
}

const ControllerStatus ControllerStatusResponse::StatusCode() const
{
	return ControllerStatus();
}

DriverStatusResponse::DriverStatusResponse(DriverStatus, const char *)
{
}

const DriverStatus DriverStatusResponse::StatusCode() const
{
	return DriverStatus();
}

#pragma endregion RESPONSE DEFINITIONS
