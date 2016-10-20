/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_messages.h"


#pragma region Message DEFINITION

// STATIC CONSTEXPR METHODS

template<class TMessage, MessageCode TCode>
CONSTEXPR MessageCode Message<TMessage, TCode>::TYPE()
{
	return TCode;
}


// CONSTRUCTORS/DESTRUCTOR

template<class TMessage, MessageCode TCode>
Message<TMessage, TCode>::Message()
{
}

template<class TMessage, MessageCode TCode>
Message<TMessage, TCode>::Message(RIFIELD)
{
}

template<class TMessage, MessageCode TCode>
Message<TMessage, TCode>::Message(PIFIELD)
{
}

template<class TMessage, MessageCode TCode>
Message<TMessage, TCode>::~Message()
{
	if (_Params != NULL)
		delete[] _Params;
}


// OPERATORS

template<class TMessage, MessageCode TCode>
RIFIELD Message<TMessage, TCode>::operator[](CSIZE i)
{
	return this->Param(i);
}


// USER METHODS

template<class TMessage, MessageCode TCode>
CSIZE Message<TMessage, TCode>::ParamCount() const
{
	return _Params == NULL ? 0 : static_cast<CSIZE>(COUNT(_Params));
}

template<class TMessage, MessageCode TCode>
RIFIELD Message<TMessage, TCode>::Param(CSIZE i) const
{
	if (_Params == NULL)
		return Field::NULL_OBJECT();

	return _Params[i];
}
				

// ISerializable IMPLEMENTATION

template<class TMessage, MessageCode TCode>
CSIZE Message<TMessage, TCode>::Size() const
{
	SIZE size = SIZEOF(MESSAGE_MARKER) + SIZEOF(MessageCode);

	for (SIZE i = 0; i < this->ParamCount(); i++)
		size += _Params[i].Size();

	return size;
}
		
template<class TMessage, MessageCode TCode>
PCBYTE Message<TMessage, TCode>::ToBytes() const
{
	return 0;
}

template<class TMessage, MessageCode TCode>
PCCHAR Message<TMessage, TCode>::ToString() const
{
	CSIZE size = this->Size();
	
	if (__field_buffer == NULL || COUNT(__field_buffer) <= size)
	{
		if (__field_buffer)
			delete[] __field_buffer;

		__field_buffer = new CHAR[size + 1];
	}

	memcpy(__field_buffer, "", size);
	__field_buffer[size] = '\0';

	return __field_buffer;
}

template<class TMessage, MessageCode TCode>
VOID Message<TMessage, TCode>::LoadFromBytes(PCBYTE data)
{
	//_DataType = static_cast<DataType>(*data++);
}

template<class TMessage, MessageCode TCode>
VOID Message<TMessage, TCode>::LoadFromString(PCCHAR data)
{
	LoadFromBytes(reinterpret_cast<PCBYTE>(data));
}

#pragma endregion
/*

#pragma region Request DEFINITIONS

// NewAngleRequest

NewAngleRequest::NewAngleRequest(CWORD degrees)
{
}

CWORD NewAngleRequest::Degrees() const
{
	return 0;
}

#pragma endregion


#pragma region Response DEFINITIONS

// Response

Response::Response(CONST Error)
{

}

CONST Error Response::ErrorCode() const
{
	return Error();
}


// AngleResponse

AngleResponse::AngleResponse(CWORD)
{
}

CONST WORD AngleResponse::Degrees() const
{
	return 0;
}


// StatusResponse

StatusResponse::StatusResponse(PCCHAR statusMsg) : _StatusMessage(statusMsg)
{
}

PCCHAR StatusResponse::StatusMessage() const
{
	return nullptr;
}


// ControllerStatusResponse

ControllerStatusResponse::ControllerStatusResponse(ControllerStatus controllerStatus, PCCHAR statusMsg) : StatusResponse(statusMsg)
{
}

CONST ControllerStatus ControllerStatusResponse::StatusCode() const
{
	return static_cast<CONST ControllerStatus>((BYTE)*reinterpret_cast<PFIELD>(&StatusResponse::Param(0)));
}


// DriverStatusResponse

DriverStatusResponse::DriverStatusResponse(DriverStatus driverStatus, PCCHAR statusMsg) : StatusResponse(statusMsg)
{
}

CONST DriverStatus DriverStatusResponse::StatusCode() const
{
	return static_cast<CONST DriverStatus>((BYTE)*reinterpret_cast<PFIELD>(&StatusResponse::Param(0)));
}

#pragma endregion
*/