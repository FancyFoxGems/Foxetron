/***********************************************************************************************
* [Foxetron_messages.h]: FOXETRON SERIAL PROTOCOL DATA STRUCTURES
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_MESSAGES_H
#define _FOXETRON_MESSAGES_H


#include "Foxetron_fields.h"


namespace Foxetron
{

#pragma region GLOBAL CONSTANTS & VARIABLES

	STATIC CCHAR MESSAGE_MARKER[4] = "FOX";

	STATIC CWORD BUFFER_SIZE = 200;

	EXTERN VCHAR RX[BUFFER_SIZE];

#pragma endregion GLOBAL CONSTANTS & VARIABLES
	


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

#pragma endregion ENUMS



#pragma region FORWARD DECLARATIONS & TYPE ALIASES


	//  TEMPLATED TYPE FORWARD DECLARATIONS & ALIASES

	template<CLASS TMessage, MessageCode TCode>
	CLASS Message;
	template<CLASS TMessage, MessageCode TCode>
	using MESSAGE = Message<TMessage, TCode>;
	template<CLASS TMessage, MessageCode TCode>
	using PMESSAGE = Message<TMessage, TCode> *;
	template<CLASS TMessage, MessageCode TCode>
	using RMESSAGE = Message<TMessage, TCode> &;
	template<CLASS TMessage, MessageCode TCode>
	using PPMESSAGE = Message<TMessage, TCode> **;
	template<CLASS TMessage, MessageCode TCode>
	using RRMESSAGE = Message<TMessage, TCode> &&;
	template<CLASS TMessage, MessageCode TCode>
	using CMESSAGE = CONST Message<TMessage, TCode>;
	template<CLASS TMessage, MessageCode TCode>
	using PCMESSAGE = CONST Message<TMessage, TCode> *;
	template<CLASS TMessage, MessageCode TCode>
	using RCMESSAGE = CONST Message<TMessage, TCode> &;
	template<CLASS TMessage, MessageCode TCode>
	using PPCMESSAGE = CONST Message<TMessage, TCode> **;


	// REQUESTS

	CLASS Request;
	typedef Request REQUEST, *PREQUEST, &RREQUEST;
	typedef CONST Request CREQUEST, *CPREQUEST, &CRREQUEST;

	CLASS AngleRequest;
	typedef AngleRequest ANGLEREQUEST, *PANGLEREQUEST, &RANGLEREQUEST;
	typedef CONST AngleRequest CANGLEREQUEST, *CPANGLEREQUEST, &CRANGLEREQUEST;

	CLASS NewAngleRequest;
	typedef NewAngleRequest NEWANGLEREQUEST, *PNEWANGLEREQUEST, &RNEWANGLEREQUEST;
	typedef CONST NewAngleRequest CNEWANGLEREQUEST, *CPNEWANGLEREQUEST, &CRNEWANGLEREQUEST;

	CLASS StatusRequest;
	typedef StatusRequest STATUSREQUEST, *PSTATUSREQUEST, &RSTATUSREQUEST;
	typedef CONST StatusRequest CSTATUSREQUEST, *CPSTATUSREQUEST, &CRSTATUSREQUEST;


	// RESPONSES

	CLASS Response;
	typedef Response RESPONSE, *PRESPONSE, &RRESPONSE;
	typedef CONST Response CRESPONSE, *CPRESPONSE, &CRRESPONSE;

	CLASS AngleResponse;
	typedef AngleResponse ANGLERESPONSE, *PANGLERESPONSE, &RANGLERESPONSE;
	typedef CONST AngleResponse CANGLERESPONSE, *CPANGLERESPONSE, &CRANGLERESPONSE;

	CLASS NewAngleResponse;
	typedef NewAngleResponse NEWANGLERESPONSE, *PNEWANGLERESPONSE, &RNEWANGLERESPONSE;
	typedef CONST NewAngleResponse CNEWANGLERESPONSE, *CPNEWANGLERESPONSE, &CRNEWANGLERESPONSE;

	CLASS StatusResponse;
	typedef StatusResponse STATUSRESPONSE, *PSTATUSRESPONSE, &RSTATUSRESPONSE;
	typedef CONST StatusResponse CSTATUSRESPONSE, *CPSTATUSRESPONSE, &CRSTATUSRESPONSE;

	CLASS ControllerStatusResponse;
	typedef ControllerStatusResponse CONTROLLERSTATUSRESPONSE, *PCONTROLLERSTATUSRESPONSE, &RCONTROLLERSTATUSRESPONSE;
	typedef CONST ControllerStatusResponse CCONTROLLERSTATUSRESPONSE, *CPCONTROLLERSTATUSRESPONSE, &CRCONTROLLERSTATUSRESPONSE;

	CLASS DriverStatusResponse;
	typedef DriverStatusResponse DRIVERSTATUSRESPONSE, *PDRIVERSTATUSRESPONSE, &RDRIVERSTATUSRESPONSE;
	typedef CONST DriverStatusResponse CDRIVERSTATUSRESPONSE, *CPDRIVERSTATUSRESPONSE, &CRDRIVERSTATUSRESPONSE;

#pragma endregion FORWARD DECLARATIONS & TYPE ALIASES



#pragma region TYPE DECLARATIONS

	// Message

	template<CLASS TMessage, MessageCode TCode>
	CLASS Message
	{
	public:

		STATIC CONSTEXPR MessageCode TYPE();
		STATIC CONSTEXPR CWORD SIZE();

		Message();
		Message(RFIELD);
		Message(PFIELD);

		VIRTUAL ~Message();

		VIRTUAL RFIELD operator[](CSIZE);

		VIRTUAL CSIZE ParamCount() CONST;

		VIRTUAL RFIELD Param(CSIZE = 0) CONST;

	protected:

		PFIELD _Params;
	};


	// REQUESTS

	CLASS Request : public Message<Request, MessageCode::REQUEST_TYPE> { };


	CLASS AngleRequest : public Message<AngleRequest, MessageCode::ANGLE_REQUEST> { };


	CLASS NewAngleRequest : public Message<NewAngleRequest, MessageCode::NEWANGLE_REQUEST>
	{
	public:

		NewAngleRequest(CWORD);

		CWORD Degrees() CONST;
	};


	CLASS StatusRequest : public Message<StatusRequest, MessageCode::STATUS_REQUEST> { };



	// RESPONSES

	CLASS Response : public Message<Response, MessageCode::RESPONSE_TYPE>
	{
	public:

		Response(CONST Error);

		VIRTUAL CONST Error ErrorCode() CONST;

	protected:

		Error _ErrorCode = Error::SUCCESS;
	};


	CLASS AngleResponse : public Message<AngleResponse, MessageCode::ANGLE_RESPONSE>
	{
	public:

		AngleResponse(CONST WORD);

		CWORD Degrees() CONST;
	};


	CLASS NewAngleResponse : public Message<NewAngleResponse, MessageCode::NEWANGLE_RESPONSE> { };

	CLASS StatusResponse : public Message<StatusResponse, MessageCode::STATUS_RESPONSE>
	{
	public:

		StatusResponse(PCCHAR);

		PCCHAR StatusMessage() CONST;

	protected:

		PCCHAR _StatusMessage;
	};


	CLASS ControllerStatusResponse : public Message<ControllerStatusResponse, MessageCode::CONTROLLER_STATUS>
	{
	public:

		ControllerStatusResponse(ControllerStatus, PCCHAR = NULL);

		CONST ControllerStatus StatusCode() CONST;

	protected:

		ControllerStatus _StatusCode = ControllerStatus::NONE;
	};


	CLASS DriverStatusResponse : public Message<DriverStatusResponse, MessageCode::DRIVER_STATUS>
	{
	public:

		DriverStatusResponse(DriverStatus, PCCHAR = NULL);

		CONST DriverStatus StatusCode() CONST;

	protected:

		DriverStatus _StatusCode = DriverStatus::IDLE;
	};

#pragma endregion TYPE DECLARATIONS

}


#endif
