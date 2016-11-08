/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_messages.h"

using namespace IttyBitty;
using namespace Foxetron;


#pragma region Request IMPLEMENTATIONS

// Request

Request::Request(MessageCode messageCode, CBYTE paramCount) : Message((CBYTE)messageCode, paramCount) { }

BOOL Request::Handle(PVOID results, PCVOID state)
{
#ifdef DEBUG_MESSAGES
	PrintLine(F("Request::Handle"));
#endif
	
#ifdef DEBUG_MESSAGES
	PrintLine();
#endif

	return TRUE;
}


// AngleRequest

AngleRequest::AngleRequest() : Request(MessageCode::ANGLE_REQUEST, 0) { }

BOOL AngleRequest::Handle(PVOID results, PCVOID state)
{
#ifdef DEBUG_MESSAGES
	PrintLine(F("AngleRequest::Handle"));
#endif

	CERROR driverError = *((PPCERROR)state)[0];
	CWORD degrees = *((PPCWORD)state)[1];

#ifdef DEBUG_MESSAGES
	PrintLine((BYTE)driverError);
	PrintLine(degrees);
#endif

	if (!Request::Handle(results, state))
		return FALSE;

	AngleResponse(driverError, degrees).Transmit();

	return TRUE;
}


// NewAngleRequest

NewAngleRequest::NewAngleRequest(CWORD degrees) : Request(MessageCode::NEWANGLE_REQUEST, 1)
{
	_Params[0] = new PARAM(degrees);
}

CWORD NewAngleRequest::Degrees() const
{
	return (CWORD)*reinterpret_cast<PCPARAM>(_Params[0]);
}

BOOL NewAngleRequest::Handle(PVOID results, PCVOID state)
{
#ifdef DEBUG_MESSAGES
	PrintLine(F("NewAngleRequest::Handle"));
#endif
	
	*((PPWORD)results)[0] = this->Degrees();
	
	CERROR driverError = *((PPCERROR)state)[0];

#ifdef DEBUG_MESSAGES
	PrintLine((BYTE)driverError);
#endif

	if (!Request::Handle(results, state))
		return FALSE;
	
	NewAngleResponse(driverError).Transmit();

	return TRUE;
}


// StatusRequest

StatusRequest::StatusRequest() : Request(MessageCode::STATUS_REQUEST, 0) { }

BOOL StatusRequest::Handle(PVOID results, PCVOID state)
{
#ifdef DEBUG_MESSAGES
	PrintLine(F("StatusRequest::Handle"));
#endif
	
	CERROR error = *((PPCERROR)state)[0];	
	PCCHAR statusMsg = *((PPCCHAR *)state)[1];	
	CBYTE statusCode = *((PPCBYTE)state)[2];
	BOOL isDriverStatus = (((PPBYTE)state)[3] != NULL);

#ifdef DEBUG_MESSAGES
	PrintLine((BYTE)error);
	PrintLine(statusMsg);
	PrintLine(statusCode);
	PrintLine(isDriverStatus);
#endif

	if (!Request::Handle(results, state))
		return FALSE;

	if (isDriverStatus)
		DriverStatusResponse(error, (CDRIVERSTATUS)statusCode, statusMsg).Transmit();
	else
		ControllerStatusResponse(error, (CCONTROLLERSTATUS)statusCode, statusMsg).Transmit();
	
	return TRUE;
}


#pragma endregion


#pragma region Response IMPLEMENTATIONS

// Response

Response::Response(CERROR error, MessageCode msgCode, CBYTE paramCount) : Message((CBYTE)msgCode, paramCount)
{
	_Params[0] = new PARAM((CBYTE)error);
}

CERROR Response::ErrorCode() const
{
	return (CERROR)(CBYTE)*reinterpret_cast<PCPARAM>(_Params[0]);
}

BOOL Response::Handle(PVOID results, PCVOID state)
{
#ifdef DEBUG_MESSAGES
	PrintLine(F("Response::Handle"));
#endif
	
	*((PPERROR)results)[0] = this->ErrorCode();

	return TRUE;
}


// AngleResponse

AngleResponse::AngleResponse(CERROR error, CWORD degrees) : Response(error, MessageCode::ANGLE_RESPONSE, 2)
{
	_Params[1] = new PARAM(degrees);
}
CWORD AngleResponse::Degrees() const
{
	return (CWORD)*reinterpret_cast<PCPARAM>(_Params[1]);
}

BOOL AngleResponse::Handle(PVOID results, PCVOID state)
{
#ifdef DEBUG_MESSAGES
	PrintLine(F("AngleResponse::Handle"));
#endif

	if (!Response::Handle(results, state))
		return FALSE;
	
	*((PPWORD)results)[1] = this->Degrees();

	return TRUE;
}


// NewAngleResponse

NewAngleResponse::NewAngleResponse(CERROR error) : Response(error, MessageCode::NEWANGLE_RESPONSE) { }

BOOL NewAngleResponse::Handle(PVOID results, PCVOID state)
{
#ifdef DEBUG_MESSAGES
	PrintLine(F("NewAngleResponse::Handle"));
#endif
	
	if (!Response::Handle(results, state))
		return FALSE;

	return TRUE;
}


// StatusResponse

StatusResponse::StatusResponse(CERROR error, PCCHAR statusMsg, MessageCode msgCode, CBYTE paramCount) : Response(error, msgCode, paramCount)
{
	_Params[1] = new VarLengthParam(statusMsg);
}

PCCHAR StatusResponse::StatusMessage() const
{
	return (PCCHAR)*reinterpret_cast<PCVARLENGTHPARAM>(_Params[1]);
}

BOOL StatusResponse::Handle(PVOID results, PCVOID state)
{
#ifdef DEBUG_MESSAGES
	PrintLine(F("StatusResponse::Handle"));
#endif
	
	if (!Response::Handle(results, state))
		return FALSE;
	
	*((PPCCHAR *)results)[1] = this->StatusMessage();

	return TRUE;
}


// ControllerStatusResponse

ControllerStatusResponse::ControllerStatusResponse(CERROR error, CCONTROLLERSTATUS controllerStatus, PCCHAR statusMsg) 
	: StatusResponse(error, statusMsg, MessageCode::CONTROLLER_STATUS, 3)
{
	_Params[2] = new PARAM((CBYTE)controllerStatus);
}

CCONTROLLERSTATUS ControllerStatusResponse::StatusCode() const
{
	return reinterpret_cast<RCCONTROLLERSTATUS>((RCBYTE)*reinterpret_cast<PCPARAM>(_Params[2]));
}

BOOL ControllerStatusResponse::Handle(PVOID results, PCVOID state)
{
#ifdef DEBUG_MESSAGES
	PrintLine(F("ControllerStatusResponse::Handle"));
#endif

	if (!StatusResponse::Handle(results, state))
		return FALSE;
	
	*((PPCONTROLLERSTATUS)results)[2] = this->StatusCode();

	return TRUE;
}


// DriverStatusResponse

DriverStatusResponse::DriverStatusResponse(CERROR error, CDRIVERSTATUS driverStatus, PCCHAR statusMsg) 
	: StatusResponse(error, statusMsg, MessageCode::DRIVER_STATUS, 3)
{
	_Params[2] = new PARAM((CBYTE)driverStatus);
}

CDRIVERSTATUS DriverStatusResponse::StatusCode() const
{
	return reinterpret_cast<RCDRIVERSTATUS>((RCBYTE)*reinterpret_cast<PCPARAM>(_Params[2]));
}

BOOL DriverStatusResponse::Handle(PVOID results, PCVOID state)
{
#ifdef DEBUG_MESSAGES
	PrintLine(F("DriverStatusResponse::Handle"));
#endif
	
	if (!StatusResponse::Handle(results, state))
		return FALSE;
	
	*((PPDRIVERSTATUS)results)[2] = this->StatusCode();

	return TRUE;
}

#pragma endregion
