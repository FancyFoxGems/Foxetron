/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_messages.h"

using namespace Foxetron;


#pragma region Request DEFINITIONS

// Request

Request::Request(MessageCode msgCode, CBYTE paramCount) : Message((CBYTE)msgCode, paramCount) { }

VOID Request::Handle(...) { }


// AngleRequest

AngleRequest::AngleRequest() : Request(MessageCode::ANGLE_REQUEST, 0) { }

VOID AngleRequest::Handle(...) { }


// NewAngleRequest

NewAngleRequest::NewAngleRequest(RCWORD degrees) : Request(MessageCode::NEWANGLE_REQUEST, 1)
{
	_Params[0] = new Field(degrees);
}

RCWORD NewAngleRequest::Degrees() const
{
	return (RCWORD)*reinterpret_cast<PFIELD>(_Params[0]);
}

VOID NewAngleRequest::Handle(...)
{

	Serial.println("NEWANGLE");
	Serial.flush();
}


// StatusRequest

StatusRequest::StatusRequest() : Request(MessageCode::STATUS_REQUEST, 0) { }

VOID StatusRequest::Handle(...) { }


#pragma endregion


#pragma region Response DEFINITIONS

// Response


Response::Response(RCERROR error, MessageCode msgCode, CBYTE paramCount) : Message((CBYTE)msgCode, paramCount) { }

RCERROR Response::ErrorCode() const
{
	return reinterpret_cast<RCERROR>((RCBYTE)*reinterpret_cast<PCFIELD>(_Params[0]));
}

VOID Response::Handle(...) { }


// AngleResponse

AngleResponse::AngleResponse(RCERROR error, RCWORD degrees) : Response(error, MessageCode::ANGLE_RESPONSE, 2)
{
	_Params[1] = new Field(degrees);
}
RCWORD AngleResponse::Degrees() const
{
	return (RCWORD)*reinterpret_cast<PCFIELD>(_Params[1]);
}

VOID AngleResponse::Handle(...)
{
	va_list args;
	va_start(args, 0);
	va_end(args);
}


// NewAngleResponse

NewAngleResponse::NewAngleResponse(RCERROR error) : Response(error, MessageCode::NEWANGLE_RESPONSE) { }

VOID NewAngleResponse::Handle(...)
{
	va_list args;
	va_start(args, 0);
	va_end(args);

	this->ErrorCode();
}


// StatusResponse

StatusResponse::StatusResponse(RCERROR error, PCCHAR statusMsg, MessageCode msgCode, CBYTE paramCount) : Response(error, msgCode, paramCount)
{
	_Params[1] = new VarLengthField(statusMsg);
}

PCCHAR StatusResponse::StatusMessage() const
{
	return (PCCHAR)*reinterpret_cast<PCVARLENGTHFIELD>(_Params[1]);
}

VOID StatusResponse::Handle(...)
{
	va_list args;
	va_start(args, 0);
	va_end(args);
}


// ControllerStatusResponse

ControllerStatusResponse::ControllerStatusResponse(RCERROR error, RCCONTROLLERSTATUS controllerStatus, PCCHAR statusMsg) 
	: StatusResponse(error, statusMsg, MessageCode::CONTROLLER_STATUS, 3)
{
	_Params[2] = new Field((BYTE)controllerStatus);
}

RCCONTROLLERSTATUS ControllerStatusResponse::StatusCode() const
{
	return reinterpret_cast<RCCONTROLLERSTATUS>((RCBYTE)*reinterpret_cast<PCFIELD>(_Params[2]));
}

VOID ControllerStatusResponse::Handle(...)
{
	va_list args;
	va_start(args, 0);
	va_end(args);
}


// DriverStatusResponse

DriverStatusResponse::DriverStatusResponse(RCERROR error, RCDRIVERSTATUS driverStatus, PCCHAR statusMsg) 
	: StatusResponse(error, statusMsg, MessageCode::DRIVER_STATUS, 3)
{
	_Params[2] = new Field((BYTE)driverStatus);
}

RCDRIVERSTATUS DriverStatusResponse::StatusCode() const
{
	return reinterpret_cast<RCDRIVERSTATUS>((RCBYTE)*reinterpret_cast<PCFIELD>(_Params[2]));
}

VOID DriverStatusResponse::Handle(...)
{
	va_list args;
	va_start(args, 0);
	va_end(args);
}

#pragma endregion
