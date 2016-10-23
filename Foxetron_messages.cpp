/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_messages.h"

using namespace Foxetron;


#pragma region Request DEFINITIONS

// Request

VOID Request::Handle(...)
{
	va_list args;
	va_start(args, 0);
	va_end(args);
}


// AngleRequest

VOID AngleRequest::Handle(...)
{
	va_list args;
	va_start(args, 0);
	va_end(args);
}


// NewAngleRequest

NewAngleRequest::NewAngleRequest(RCWORD degrees) : TBASE()
{
	_Params[0] = new Field(degrees);
}

RCWORD NewAngleRequest::Degrees() const
{
	return (RCWORD)*reinterpret_cast<PFIELD>(_Params[0]);
}

VOID NewAngleRequest::Handle(...)
{
	va_list args;
	va_start(args, 0);
	va_end(args);
}


// StatusRequest

VOID StatusRequest::Handle(...)
{
	va_list args;
	va_start(args, 0);
	va_end(args);
}


#pragma endregion


#pragma region Response DEFINITIONS

// Response

Response::Response(RCERROR error) : TBASE()
{
	_Params[0] = new Field((BYTE)error);
}

RCERROR Response::ErrorCode() const
{
	return reinterpret_cast<RCERROR>((RCBYTE)*reinterpret_cast<PCFIELD>(_Params[0]));
}

VOID Response::Handle(...)
{
	va_list args;
	va_start(args, 0);
	va_end(args);
}


// AngleResponse

AngleResponse::AngleResponse(RCERROR error, RCWORD degrees) : TBASE()
{
	_Params[0] = new Field((BYTE)error);
	_Params[1] = new Field(degrees);
}

RCERROR AngleResponse::ErrorCode() const
{
	return reinterpret_cast<RCERROR>((RCBYTE)*reinterpret_cast<PCFIELD>(_Params[0]));
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

RCERROR NewAngleResponse::ErrorCode() const
{
	return reinterpret_cast<RCERROR>((RCBYTE)*reinterpret_cast<PCFIELD>(_Params[0]));
}

VOID NewAngleResponse::Handle(...)
{
	va_list args;
	va_start(args, 0);
	va_end(args);
}


// StatusResponse

StatusResponse::StatusResponse(RCERROR error, PCCHAR statusMsg) : TBASE()
{
	_Params[0] = new Field((BYTE)error);
	_Params[1] = new VarLengthField(statusMsg);
}

RCERROR StatusResponse::ErrorCode() const
{
	return reinterpret_cast<RCERROR>((RCBYTE)*reinterpret_cast<PCFIELD>(_Params[0]));
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

ControllerStatusResponse::ControllerStatusResponse(RCERROR error, RCCONTROLLERSTATUS controllerStatus, PCCHAR statusMsg) : TBASE()
{
	_Params[0] = new Field((BYTE)error);
	_Params[1] = new VarLengthField(statusMsg);
	_Params[2] = new Field((BYTE)controllerStatus);
}

RCERROR ControllerStatusResponse::ErrorCode() const
{
	return reinterpret_cast<RCERROR>((RCBYTE)*reinterpret_cast<PCFIELD>(_Params[0]));
}

PCCHAR ControllerStatusResponse::StatusMessage() const
{
	return (PCCHAR)*reinterpret_cast<PCVARLENGTHFIELD>(_Params[1]);
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

DriverStatusResponse::DriverStatusResponse(RCERROR error, RCDRIVERSTATUS driverStatus, PCCHAR statusMsg) : TBASE()
{
	_Params[0] = new Field((BYTE)error);
	_Params[1] = new VarLengthField(statusMsg);
	_Params[2] = new Field((BYTE)driverStatus);
}

RCERROR DriverStatusResponse::ErrorCode() const
{
	return reinterpret_cast<RCERROR>((RCBYTE)*reinterpret_cast<PCFIELD>(_Params[0]));
}

PCCHAR DriverStatusResponse::StatusMessage() const
{
	return (PCCHAR)*reinterpret_cast<PCVARLENGTHFIELD>(_Params[1]);
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
