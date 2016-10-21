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
#pragma region GLOBAL CONSTANTS & VARIABLES

	CBYTE RX_BUFFER_SIZE = 128;

	EXTERN VCHAR RX[RX_BUFFER_SIZE];

#pragma endregion


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


#pragma region Request DECLARATIONS

	CLASS Request : public Message<Request, MessageCode::REQUEST_TYPE> 
	{
	protected:
		
		using Message<Request, MessageCode::REQUEST_TYPE> ::_Params;

		typedef Message<Request, MessageCode::REQUEST_TYPE> TBASE;
	};


	CLASS AngleRequest : public Message<AngleRequest, MessageCode::ANGLE_REQUEST>, public Request
	{
	protected:
		
		using Request::_Params;

		typedef Message<AngleRequest, MessageCode::ANGLE_REQUEST> TBASE;
	};


	CLASS NewAngleRequest : public Message<NewAngleRequest, MessageCode::NEWANGLE_REQUEST, 1>, public Request
	{
	public:

		NewAngleRequest(CWORD);

		VIRTUAL CWORD Degrees() const;
		
	protected:
		
		using Request::_Params;

		typedef Message<NewAngleRequest, MessageCode::NEWANGLE_REQUEST, 1> TBASE;
	};


	CLASS StatusRequest : public Message<StatusRequest, MessageCode::STATUS_REQUEST>, public Request
	{
	protected:
		
		using Request::_Params;

		typedef Message<StatusRequest, MessageCode::STATUS_REQUEST> TBASE;
	};

#pragma endregion
	

#pragma region Response DECLARATIONS

	CLASS Response : public Message<Response, MessageCode::RESPONSE_TYPE, 1>
	{
	public:

		Response(CONST Error);

		VIRTUAL CONST Error ErrorCode() const;
		
	protected:

		using Message<Response, MessageCode::RESPONSE_TYPE, 1>::_Params;

		typedef Message<Response, MessageCode::RESPONSE_TYPE, 1> TBASE;

		Error _ErrorCode = Error::SUCCESS;
	};


	CLASS AngleResponse : public Message<AngleResponse, MessageCode::ANGLE_RESPONSE, 2>, public Response
	{
	public:

		AngleResponse(CONST Error, CWORD);

		VIRTUAL CWORD Degrees() const;
		
	protected:
		
		using Response::_Params;

		typedef Message<AngleResponse, MessageCode::ANGLE_RESPONSE, 2> TBASE;
	};


	CLASS NewAngleResponse : public Message<NewAngleResponse, MessageCode::NEWANGLE_RESPONSE, 1>, public Response
	{
	protected:
		
		using Response::_Params;

		typedef Message<NewAngleResponse, MessageCode::NEWANGLE_RESPONSE, 1> TBASE;
	};

	CLASS StatusResponse : public Message<StatusResponse, MessageCode::STATUS_RESPONSE, 2>, public Response
	{
	public:

		StatusResponse(CONST Error, PCCHAR);

		VIRTUAL PCCHAR StatusMessage() const;
		
	protected:
		
		using Response::_Params;

		typedef Message<StatusResponse, MessageCode::STATUS_RESPONSE, 2> TBASE;
	};


	CLASS ControllerStatusResponse : public Message<ControllerStatusResponse, MessageCode::CONTROLLER_STATUS, 3>, public StatusResponse
	{
	public:

		ControllerStatusResponse(CONST Error, CONST ControllerStatus, PCCHAR = NULL);

		VIRTUAL CONST ControllerStatus StatusCode() const;
		
	protected:
		
		using StatusResponse::_Params;

		typedef Message<ControllerStatusResponse, MessageCode::CONTROLLER_STATUS, 3> TBASE;
	};


	CLASS DriverStatusResponse : public Message<DriverStatusResponse, MessageCode::DRIVER_STATUS, 3>, public StatusResponse
	{
	public:

		DriverStatusResponse(CONST Error, CONST DriverStatus, PCCHAR = NULL);
		
		VIRTUAL CONST DriverStatus StatusCode() const;

	protected:
		
		using StatusResponse::_Params;

		typedef Message<DriverStatusResponse, MessageCode::DRIVER_STATUS, 3> TBASE;
	};

#pragma endregion
}


#endif
