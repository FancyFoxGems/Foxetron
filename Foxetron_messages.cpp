/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright � 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_messages.h"


#pragma region Message DEFINITION

// STATIC MESSAGE "OVERRIDES"

template<class TMessage, MessageCode TCode>
CONSTEXPR MessageCode Message<TMessage, TCode>::TYPE()
{
	return TCode;
}

template<class TMessage, MessageCode TCode>
CONSTEXPR CSIZE Message<TMessage, TCode>::SIZE()
{
	return SIZEOF(TMessage);
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
}


// OPERATORS

template<class TMessage, MessageCode TCode>
RIFIELD Message<TMessage, TCode>::operator[](CSIZE i)
{
	return this->Param(i);
}


// PUBLIC METHODS

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


// PROTECTED METHODS


#pragma endregion


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
