/***********************************************************************************************
* [Foxetron_messages.h]: FOXETRON SERIAL PROTOCOL DATA STRUCTURES
*
* This file is part of the Foxetron program suite
* Copyright � 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
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
		ANGLE_RESPONSE		= RESPONSE_TYPE | 0x1,
		NEWANGLE_RESPONSE	= RESPONSE_TYPE | 0x2,
		STATUS_RESPONSE		= RESPONSE_TYPE | STATUS_TYPE,
		CONTROLLER_STATUS	= STATUS_RESPONSE | 0x1,
		DRIVER_STATUS		= STATUS_RESPONSE | 0x2
	};

	enum Error : BYTE
	{
		SUCCESS = 0
	};

	enum ControllerStatus : BYTE
	{
		NONE
	};

	enum DriverStatus : BYTE
	{
		IDLE
	};

	typedef enum MessageCode * PMESSAGECODE, & RMESSAGECODE, ** PPMESSAGECODE;
	typedef const enum MessageCode * PCMESSAGECODE, & RCMESSAGECODE, ** PPCMESSAGECODE;

	typedef enum Error * PERROR, & RERROR, ** PPERROR;
	typedef const enum Error * PCERROR, & RCERROR, ** PPCERROR;

	typedef enum ControllerStatus * PCONTROLLERSTATUS, & RCONTROLLERSTATUS, ** PPCONTROLLERSTATUS;
	typedef const enum ControllerStatus * PCCONTROLLERSTATUS, & RCCONTROLLERSTATUS, ** PPCCONTROLLERSTATUS;

	typedef enum DriverStatus * PDRIVERSTATUS, & RDRIVERSTATUS, ** PPDRIVERSTATUS;
	typedef const enum DriverStatus * PCDRIVERSTATUS, & RCDRIVERSTATUS, ** PPCDRIVERSTATUS;

#pragma endregion

	
#pragma region FORWARD DECLARATIONS & TYPE ALIASES

	// REQUESTS

	class Request;
	typedef Request REQUEST, * PREQUEST, & RREQUEST;
	typedef const Request CREQUEST, * PCREQUEST, & RCREQUEST;

	class AngleRequest;
	typedef AngleRequest ANGLEREQUEST, * PANGLEREQUEST, & RANGLEREQUEST;
	typedef const AngleRequest CANGLEREQUEST, * CPANGLEREQUEST, & RCANGLEREQUEST;

	class NewAngleRequest;
	typedef NewAngleRequest NEWANGLEREQUEST, * PNEWANGLEREQUEST, & RNEWANGLEREQUEST;
	typedef const NewAngleRequest CNEWANGLEREQUEST, * CPNEWANGLEREQUEST, & CRNEWANGLEREQUEST;

	class StatusRequest;
	typedef StatusRequest STATUSREQUEST, * PSTATUSREQUEST, & RSTATUSREQUEST;
	typedef const StatusRequest CSTATUSREQUEST, * CPSTATUSREQUEST, & CRSTATUSREQUEST;


	// RESPONSES

	class IResponse;
	typedef IResponse IRESPONSE, * PIRESPONSE, & RIRESPONSE;
	typedef const IResponse CIRESPONSE, * PCIRESPONSE, & RCIRESPONSE;

	class IStatusResponse;
	typedef IStatusResponse ISTATUSRESPONSE, * PISTATUSRESPONSE, & RISTATUSRESPONSE;
	typedef const IStatusResponse CISTATUSRESPONSE, * PCISTATUSRESPONSE, & RCISTATUSRESPONSE;

	class Response;
	typedef Response RESPONSE, * PRESPONSE, & RRESPONSE;
	typedef const Response CRESPONSE, * PCRESPONSE, & RCRESPONSE;

	class AngleResponse;
	typedef AngleResponse ANGLERESPONSE, * PANGLERESPONSE, & RANGLERESPONSE;
	typedef const AngleResponse CANGLERESPONSE, * PCANGLERESPONSE, & RCANGLERESPONSE;

	class NewAngleResponse;
	typedef NewAngleResponse NEWANGLERESPONSE, * PNEWANGLERESPONSE, & RNEWANGLERESPONSE;
	typedef const NewAngleResponse CNEWANGLERESPONSE, * PCNEWANGLERESPONSE, & RCNEWANGLERESPONSE;

	class StatusResponse;
	typedef StatusResponse STATUSRESPONSE, * PSTATUSRESPONSE, & RSTATUSRESPONSE;
	typedef const StatusResponse CSTATUSRESPONSE, * PCSTATUSRESPONSE, & RCSTATUSRESPONSE;

	class ControllerStatusResponse;
	typedef ControllerStatusResponse CONTROLLERSTATUSRESPONSE, * PCONTROLLERSTATUSRESPONSE, & RCONTROLLERSTATUSRESPONSE;
	typedef const ControllerStatusResponse CCONTROLLERSTATUSRESPONSE, * PCCONTROLLERSTATUSRESPONSE, & RCCONTROLLERSTATUSRESPONSE;

	class DriverStatusResponse;
	typedef DriverStatusResponse DRIVERSTATUSRESPONSE, * PDRIVERSTATUSRESPONSE, & RDRIVERSTATUSRESPONSE;
	typedef const DriverStatusResponse CDRIVERSTATUSRESPONSE, * PCDRIVERSTATUSRESPONSE, & RCDRIVERSTATUSRESPONSE;

#pragma endregion


#pragma region Request DEFINITIONS

	CLASS Request : public Message
	{
	public:

		Request(MessageCode = MessageCode::REQUEST_TYPE, CBYTE paramCount = 0);
	};


	CLASS AngleRequest : public Request
	{
	public:

		AngleRequest();

		BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};


	CLASS NewAngleRequest : public Request
	{
	public:

		NewAngleRequest(RCWORD);

		RCWORD Degrees() const;
		
		BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};


	CLASS StatusRequest : public Request
	{
	public:

		StatusRequest();

		BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};

#pragma endregion
	

#pragma region Response DEFINITIONS

	INTERFACE IResponse : public IMessage
	{
	public:

		VIRTUAL RCERROR ErrorCode() const = 0;


	protected:

		IResponse() { }
	};
	

	INTERFACE IStatusResponse : public IResponse
	{
	public:

		VIRTUAL PCCHAR StatusMessage() const;


	protected:

		IStatusResponse() { }
	};


	CLASS Response : public Message, public IResponse
	{
	public:

		Response(RCERROR, MessageCode = MessageCode::RESPONSE_TYPE, CBYTE = 1);

		RCERROR ErrorCode() const;

		BOOL Handle(PVOID = NULL, PCVOID = NULL);
	};


	CLASS AngleResponse : public Response
	{
	public:
		
		AngleResponse(RCERROR, RCWORD);

		RCWORD Degrees() const;

		BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};


	CLASS NewAngleResponse : public Response
	{
	public:

		NewAngleResponse(RCERROR);

		BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};

	CLASS StatusResponse : public Response
	{
	public:

		StatusResponse(RCERROR, PCCHAR, MessageCode = MessageCode::RESPONSE_TYPE, CBYTE = 2);

		PCCHAR StatusMessage() const;

		BOOL Handle(PVOID = NULL, PCVOID = NULL);
	};

	
	CLASS ControllerStatusResponse : public StatusResponse
	{
	public:

		ControllerStatusResponse(RCERROR, RCCONTROLLERSTATUS, PCCHAR = NULL);

		RCCONTROLLERSTATUS StatusCode() const;

		BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};


	CLASS DriverStatusResponse : public StatusResponse
	{
	public:

		DriverStatusResponse(RCERROR, RCDRIVERSTATUS, PCCHAR = NULL);
		
		RCDRIVERSTATUS StatusCode() const;

		BOOL Handle(PVOID = NULL, PCVOID = NULL) final;
	};

#pragma endregion
}


#endif
