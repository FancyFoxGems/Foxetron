/***********************************************************************************************
* [Foxetron_messages.h]: FOXETRON SERIAL PROTOCOL DATA STRUCTURES
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_MESSAGES_H
#define _FOXETRON_MESSAGES_H


#include "IttyBitty_messages.h"

using namespace IttyBitty;


namespace Foxetron
{
#pragma region ENUMS

	enum MessageCode : BYTE
	{
		// Primary types
		REQUEST_TYPE		= 0x0,
		RESPONSE_TYPE		= 0x80,


		// Categories
		ANGLE_TYPE			= 0x10,
		NEWANGLE_TYPE		= 0x20,
		STATUS_TYPE			= 0x40,


		// Request types
		ANGLE_REQUEST		= REQUEST_TYPE | ANGLE_TYPE,
		NEWANGLE_REQUEST	= REQUEST_TYPE | NEWANGLE_TYPE,
		STATUS_REQUEST		= REQUEST_TYPE | STATUS_TYPE,

		// Response types
		ANGLE_RESPONSE		= RESPONSE_TYPE | ANGLE_TYPE,
		NEWANGLE_RESPONSE	= RESPONSE_TYPE | NEWANGLE_TYPE,
		STATUS_RESPONSE		= RESPONSE_TYPE | STATUS_TYPE,
		CONTROLLER_STATUS	= STATUS_RESPONSE | 0x1,
		DRIVER_STATUS		= STATUS_RESPONSE | 0x2
	};

	TYPEDEF_ENUM_ALIASES(MessageCode, MESSAGECODE);


	ENUM Error : BYTE
	{
		SUCCESS = 0
	};

	TYPEDEF_ENUM_ALIASES(Error, ERROR);


	ENUM ControllerStatus : BYTE
	{
		NONE
	};

	TYPEDEF_ENUM_ALIASES(ControllerStatus, CONTROLLERSTATUS);


	ENUM DriverStatus : BYTE
	{
		IDLE
	};

	TYPEDEF_ENUM_ALIASES(DriverStatus, DRIVERSTATUS);

#pragma endregion


#pragma region FORWARD DECLARATIONS & TYPE ALIASES

	// REQUESTS

	class Request;
	TYPEDEF_CLASS_ALIASES(Request, REQUEST);

	class AngleRequest;
	TYPEDEF_CLASS_ALIASES(AngleRequest, ANGLEREQUEST);

	class NewAngleRequest;
	TYPEDEF_CLASS_ALIASES(NewAngleRequest, NEWANGLEREQUEST);

	class StatusRequest;
	TYPEDEF_CLASS_ALIASES(StatusRequest, STATUSREQUEST);


	// RESPONSES

	class IResponse;
	TYPEDEF_CLASS_ALIASES(IResponse, IRESPONSE);

	class IStatusResponse;
	TYPEDEF_CLASS_ALIASES(IStatusResponse, ISTATUSRESPONSE);

	class Response;
	TYPEDEF_CLASS_ALIASES(Response, RESPONSE);

	class AngleResponse;
	TYPEDEF_CLASS_ALIASES(AngleResponse, ANGLERESPONSE);

	class NewAngleResponse;
	TYPEDEF_CLASS_ALIASES(NewAngleResponse, NEWANGLERESPONSE);

	class StatusResponse;
	TYPEDEF_CLASS_ALIASES(StatusResponse, STATUSRESPONSE);

	class ControllerStatusResponse;
	TYPEDEF_CLASS_ALIASES(ControllerStatusResponse, CONTROLLERSTATUSRESPONSE);

	class DriverStatusResponse;
	TYPEDEF_CLASS_ALIASES(DriverStatusResponse, DRIVERSTATUSRESPONSE);

#pragma endregion


#pragma region Request DEFINITIONS

	CLASS Request : public Message
	{
	public:

		Request(MessageCode = MessageCode::REQUEST_TYPE, CBYTE paramCount = 0);

		VIRTUAL BOOL Handle(PVOID = NULL, PCVOID = NULL);
	};


	CLASS AngleRequest : public Request
	{
	public:

		AngleRequest();

		VIRTUAL BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};


	CLASS NewAngleRequest : public Request
	{
	public:

		NewAngleRequest(CWORD);

		CWORD Degrees() const;

		VIRTUAL BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};


	CLASS StatusRequest : public Request
	{
	public:

		StatusRequest();

		VIRTUAL BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};

#pragma endregion


#pragma region Response DEFINITIONS

	INTERFACE IResponse
	{
	public:

		VIRTUAL CERROR ErrorCode() const = 0;


	protected:

		IResponse() { }
	};


	INTERFACE IStatusResponse
	{
	public:

		VIRTUAL PCCHAR StatusMessage() const;


	protected:

		IStatusResponse() { }
	};


	CLASS Response : public Message, public IResponse
	{
	public:

		Response(CERROR, MessageCode = MessageCode::RESPONSE_TYPE, CBYTE = 1);

		CERROR ErrorCode() const final;

		VIRTUAL BOOL Handle(PVOID = NULL, PCVOID = NULL);
	};


	CLASS AngleResponse : public Response
	{
	public:

		AngleResponse(CERROR, CWORD);

		CWORD Degrees() const;

		VIRTUAL BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};


	CLASS NewAngleResponse : public Response
	{
	public:

		NewAngleResponse(CERROR);

		VIRTUAL BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};

	CLASS StatusResponse : public Response, public IStatusResponse
	{
	public:

		StatusResponse(CERROR, PCCHAR, MessageCode = MessageCode::STATUS_RESPONSE, CBYTE = 2);

		PCCHAR StatusMessage() const final;

		VIRTUAL BOOL Handle(PVOID = NULL, PCVOID = NULL);
	};


	CLASS ControllerStatusResponse : public StatusResponse
	{
	public:

		ControllerStatusResponse(CERROR, CCONTROLLERSTATUS, PCCHAR = NULL);

		CCONTROLLERSTATUS StatusCode() const;

		VIRTUAL BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};


	CLASS DriverStatusResponse : public StatusResponse
	{
	public:

		DriverStatusResponse(CERROR, CDRIVERSTATUS, PCCHAR = NULL);

		CDRIVERSTATUS StatusCode() const;

		VIRTUAL BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};

#pragma endregion
}


#endif
