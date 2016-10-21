/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright � 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_messages.h"


#pragma region GLOBAL VARIABLES

//IttyBitty::MESSAGE_MARKER = "FOX";

VCHAR Foxetron::RX[RX_BUFFER_SIZE];

#pragma endregion


#pragma region Request DEFINITIONS

// NewAngleRequest

NewAngleRequest::NewAngleRequest(CWORD degrees) : Request()
{
	_Params[0] = Field(UNCONST(degrees));
}

CWORD NewAngleRequest::Degrees() const
{
	return 0;
}

#pragma endregion


#pragma region Response DEFINITIONS

// Response

Response::Response(CONST Error error) : Message()
{
	_Params[0] = Field((RBYTE)UNCONST(error));
}

CONST Error Response::ErrorCode() const
{
	return Error();
}


// AngleResponse

AngleResponse::AngleResponse(CONST Error error, CWORD degrees) : Response(error)
{
	_Params[1] = Field(UNCONST(degrees));
}

CONST WORD AngleResponse::Degrees() const
{
	return 0;
}


// StatusResponse

StatusResponse::StatusResponse(CONST Error error, PCCHAR statusMsg) : Response(error)
{
	_Params[1] = VarLengthField(UNCONST(statusMsg));
}

PCCHAR StatusResponse::StatusMessage() const
{
	return nullptr;
}


// ControllerStatusResponse

ControllerStatusResponse::ControllerStatusResponse(CONST Error error, CONST ControllerStatus controllerStatus, PCCHAR statusMsg) : StatusResponse(error, statusMsg)
{
	_Params[2] = Field((RBYTE)UNCONST(controllerStatus));
}

CONST ControllerStatus ControllerStatusResponse::StatusCode() const
{
	return (ControllerStatus)0;//static_cast<CONST ControllerStatus>((BYTE)*reinterpret_cast<PFIELD>(&StatusResponse::Param(0)));
}


// DriverStatusResponse

DriverStatusResponse::DriverStatusResponse(CONST Error error, CONST DriverStatus driverStatus, PCCHAR statusMsg) : StatusResponse(error, statusMsg)
{
	_Params[2] = Field((RBYTE)UNCONST(driverStatus));
}

CONST DriverStatus DriverStatusResponse::StatusCode() const
{
	return (DriverStatus)0;//static_cast<CONST DriverStatus>((BYTE)*reinterpret_cast<PCFIELD>(&StatusResponse::Param(0)));
}

#pragma endregion
