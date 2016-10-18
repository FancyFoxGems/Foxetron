/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_messages.h"


#pragma region MESSAGE DEFINITION

// STATIC MESSAGE "OVERRIDES"

template<CLASS TMessage, MessageCode TCode>
CONSTEXPR MessageCode Message<TMessage, TCode>::TYPE()
{
	return TCode;
}

template<CLASS TMessage, MessageCode TCode>
CONSTEXPR CSIZE Message<TMessage, TCode>::SIZE()
{
	return SIZEOF(TMessage);
}


// CONSTRUCTORS/DESTRUCTOR

template<CLASS TMessage, MessageCode TCode>
Message<TMessage, TCode>::Message()
{
}

template<CLASS TMessage, MessageCode TCode>
Message<TMessage, TCode>::Message(RFIELD)
{
}

template<CLASS TMessage, MessageCode TCode>
Message<TMessage, TCode>::Message(PFIELD)
{
}

template<CLASS TMessage, MessageCode TCode>
Message<TMessage, TCode>::~Message()
{
}


// OPERATORS

template<CLASS TMessage, MessageCode TCode>
RFIELD Message<TMessage, TCode>::operator[](CSIZE i)
{
	return this->Param(i);
}


// PUBLIC METHODS

template<CLASS TMessage, MessageCode TCode>
CSIZE Message<TMessage, TCode>::ParamCount() CONST
{
	return _Params == NULL ? 0 : static_cast<CSIZE>(COUNT(_Params));
}

template<CLASS TMessage, MessageCode TCode>
RFIELD Message<TMessage, TCode>::Param(CSIZE i) CONST
{
	if (_Params == NULL)
	{
		STATIC DATUM NULL_DATUM;
		STATIC FIELD NULL_FIELD(NULL_DATUM);
		return NULL_FIELD;
	}

	return _Params[i];
}


// PROTECTED METHODS


#pragma endregion MESSAGE DEFINITION



#pragma region REQUEST DEFINITIONS

// NewAngleRequest

NewAngleRequest::NewAngleRequest(CWORD degrees)
{
}

CWORD NewAngleRequest::Degrees() CONST
{
	return 0;
}

#pragma endregion REQUEST DEFINITIONS



#pragma region RESPONSE DEFINITIONS

// Response

Response::Response(CONST Error)
{
}

CONST Error Response::ErrorCode() CONST
{
	return Error();
}


// AngleResponse

AngleResponse::AngleResponse(CWORD)
{
}

CONST WORD AngleResponse::Degrees() CONST
{
	return 0;
}


// StatusResponse

StatusResponse::StatusResponse(PCCHAR statusMsg) : _StatusMessage(statusMsg)
{
}

PCCHAR StatusResponse::StatusMessage() CONST
{
	return nullptr;
}


// ControllerStatusResponse

ControllerStatusResponse::ControllerStatusResponse(ControllerStatus controllerStatus, PCCHAR statusMsg) : StatusResponse(statusMsg)
{
}

CONST ControllerStatus ControllerStatusResponse::StatusCode() CONST
{
	return static_cast<CONST ControllerStatus>((BYTE)(StatusResponse::Param(0)));
}


// DriverStatusResponse

DriverStatusResponse::DriverStatusResponse(DriverStatus driverStatus, PCCHAR statusMsg) : StatusResponse(statusMsg)
{
}

CONST DriverStatus DriverStatusResponse::StatusCode() CONST
{
	return static_cast<CONST DriverStatus>((BYTE)(StatusResponse::Param(0)));
}

#pragma endregion RESPONSE DEFINITIONS
