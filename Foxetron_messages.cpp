/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_messages.h"


#pragma region Message DEFINITION

// GLOBAL VARIABLES

VCHAR Foxetron::RX[RX_BUFFER_SIZE];

PBYTE Foxetron::__message_buffer;


// STATIC CONSTEXPR METHODS

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
CONSTEXPR MessageCode Message<TMessage, MsgCode, ParamCnt>::TYPE()
{
	return MsgCode;
}

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
CONSTEXPR CSIZE Message<TMessage, MsgCode, ParamCnt>::PARAM_COUNT()
{
	return ParamCnt;
}


// CONSTRUCTORS/DESTRUCTOR

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
Message<TMessage, MsgCode, ParamCnt>::Message()
{
}

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
Message<TMessage, MsgCode, ParamCnt>::Message(RIFIELD)
{
}

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
Message<TMessage, MsgCode, ParamCnt>::Message(PIFIELD)
{
}

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
Message<TMessage, MsgCode, ParamCnt>::~Message()
{
	if (_Params != NULL)
		delete[] _Params;
}


// OPERATORS

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
RIFIELD Message<TMessage, MsgCode, ParamCnt>::operator[](CSIZE i)
{
	return this->Param(i);
}


// USER METHODS

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
RIFIELD Message<TMessage, MsgCode, ParamCnt>::Param(CSIZE i) const
{
	if (_Params == NULL)
		return Field::NULL_OBJECT();

	return _Params[i];
}
				

// ISerializable IMPLEMENTATION

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
CSIZE Message<TMessage, MsgCode, ParamCnt>::Size() const
{
	SIZE size = SIZEOF(MESSAGE_MARKER) + SIZEOF(MessageCode);

	for (SIZE i = 0; i < this->PARAM_COUNT(); i++)
		size += _Params[i].Size();

	return size;
}
		
template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
PCBYTE Message<TMessage, MsgCode, ParamCnt>::ToBytes() const
{
	return 0;
}

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
PCCHAR Message<TMessage, MsgCode, ParamCnt>::ToString() const
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

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
VOID Message<TMessage, MsgCode, ParamCnt>::LoadFromBytes(PCBYTE data)
{
	//_DataType = static_cast<DataType>(*data++);
}

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
VOID Message<TMessage, MsgCode, ParamCnt>::LoadFromString(PCCHAR data)
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