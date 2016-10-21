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
CONSTEXPR MessageCode Message<TMessage, MsgCode, ParamCnt>::MESSAGE_CODE()
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
Message<TMessage, MsgCode, ParamCnt>::Message() : _Dispose(TRUE)
{
	_Params = new FIELD[ParamCnt];
}

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
Message<TMessage, MsgCode, ParamCnt>::Message(RIFIELD param) : _Dispose(TRUE)
{
	_Params = new FIELD[ParamCnt];
	_Params[0] = param;
}

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
Message<TMessage, MsgCode, ParamCnt>::Message(PIFIELD params) : _Params(params) { }

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
Message<TMessage, MsgCode, ParamCnt>::~Message()
{
	if (_Params != NULL)
		delete[] _Params;
}


// OPERATORS

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
RCIFIELD Message<TMessage, MsgCode, ParamCnt>::operator[](CSIZE i) const
{
	return this->Param(i);
}

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
RIFIELD Message<TMessage, MsgCode, ParamCnt>::operator[](CSIZE i)
{
	return this->Param(i);
}


// USER METHODS

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
RIFIELD Message<TMessage, MsgCode, ParamCnt>::Param(CSIZE i)
{
	if (_Params == NULL)
		return Field::NULL_OBJECT();

	return _Params[i];
}
				

// ISerializable IMPLEMENTATION

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
CSIZE Message<TMessage, MsgCode, ParamCnt>::Size() const
{
	return MESSAGE_MARKER_SIZE + SIZEOF(MessageCode) + SIZEOF(CSIZE) + this->ParamsSize();
}

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
CSIZE Message<TMessage, MsgCode, ParamCnt>::ByteWidth() const
{
	return MESSAGE_MARKER_SIZE + SIZEOF(MessageCode) + SIZEOF(CSIZE) + this->ParamsStringSize();
}
		
template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
PCBYTE Message<TMessage, MsgCode, ParamCnt>::ToBytes() const
{
	CSIZE size = this->Size();

	if (__message_buffer)
		delete[] __message_buffer;

	__message_buffer = new BYTE[size];

	PBYTE bufferPtr = __message_buffer;
		
	memcpy(bufferPtr, &MESSAGE_MARKER, MESSAGE_MARKER_SIZE);
	bufferPtr += MESSAGE_MARKER_SIZE;

	MessageCode msgCode = MsgCode;
	memcpy(bufferPtr++, &msgCode, SIZEOF(MessageCode));
	
	CSIZE paramsSize = this->ParamsSize();
	memcpy(bufferPtr, &paramsSize, SIZEOF(CSIZE));
	bufferPtr += SIZEOF(CSIZE);

	RCIFIELD param = Field::NULL_OBJECT();
	CSIZE paramSize = 0;
	

	for (SIZE i = 0; i < this->PARAM_COUNT(); i++)
	{
		param = _Params[i];
		paramSize = param.Size();

		memcpy(bufferPtr, param.ToBytes(), paramSize);
		bufferPtr += paramSize;
	}

	return __message_buffer;
}

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
PCCHAR Message<TMessage, MsgCode, ParamCnt>::ToString() const
{
	CSIZE size = this->ByteWidth();

	if (__message_buffer)
		delete[] __message_buffer;

	__message_buffer = new BYTE[size];

	PBYTE bufferPtr = __message_buffer;
		
	memcpy(bufferPtr, &MESSAGE_MARKER, MESSAGE_MARKER_SIZE - 1);
	bufferPtr += MESSAGE_MARKER_SIZE - 1;

	MessageCode msgCode = MsgCode;
	memcpy(bufferPtr++, &msgCode, SIZEOF(MessageCode));
	
	CSIZE paramsSize = this->ParamsStringSize();
	memcpy(bufferPtr, &paramsSize, SIZEOF(CSIZE));
	bufferPtr += SIZEOF(CSIZE);

	RCIFIELD param = Field::NULL_OBJECT();
	CSIZE paramSize = 0;
	

	for (SIZE i = 0; i < this->PARAM_COUNT(); i++)
	{
		param = _Params[i];
		paramSize = param.ByteWidth();
		if (_Params[i].GetDataType() == DataType::STRING_FIELD)
			paramSize -= 1;

		memcpy(bufferPtr, param.ToString(), paramSize);
		bufferPtr += paramSize;
	}

	__message_buffer[size] = '\0';

	return reinterpret_cast<PCCHAR>(__message_buffer);
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


template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
SIZE Message<TMessage, MsgCode, ParamCnt>::printTo(Print& printer) const
{
	SIZE printed = printer.print(MESSAGE_MARKER);
	printed += printer.print(MsgCode);
	printed += printer.print(this->ParamsStringSize());
	
	for (SIZE i = 0; i < this->PARAM_COUNT(); i++)
		printed += _Params[i].printTo(printer);

	return printed;
}


// HELPER METHODS

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
CSIZE Message<TMessage, MsgCode, ParamCnt>::ParamsSize() const
{
	SIZE size = 0;

	for (SIZE i = 0; i < this->PARAM_COUNT(); i++)
		size += _Params[i].Size();

	return size;
}

template<class TMessage, MessageCode MsgCode, CSIZE ParamCnt>
CSIZE Message<TMessage, MsgCode, ParamCnt>::ParamsStringSize() const
{
	SIZE size = 0;

	for (SIZE i = 0; i < this->PARAM_COUNT(); i++)
	{
		size += _Params[i].ByteWidth();

		if (_Params[i].GetDataType() == DataType::STRING_FIELD)
			--size;
	}

	return size;
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