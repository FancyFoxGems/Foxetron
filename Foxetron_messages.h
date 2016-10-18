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


#define countof(var) (sizeof(var) / sizeof(0[var]))


namespace Foxetron
{

#pragma region GLOBAL CONSTANTS & VARIABLES

	static const char MESSAGE_MARKER[4] = "FOX";

	static const word BUFFER_SIZE = 200;

	static volatile char RX[BUFFER_SIZE];

#pragma endregion GLOBAL CONSTANTS & VARIABLES
	


#pragma region ENUMS

	enum MessageCode : byte
	{
		// Primary types
		REQUEST_TYPE		= 0x0,
		RESPONSE_TYPE		= 0x80,


		// Categories
		ANGLE_TYPE			= 0x10,
		NEWANGLE_TYPE			= 0x20,
		STATUS_TYPE			= 0x40,


		// Request types
		ANGLE_REQUEST		= REQUEST_TYPE | ANGLE_TYPE,
		NEWANGLE_REQUEST		= REQUEST_TYPE | NEWANGLE_TYPE,
		STATUS_REQUEST		= REQUEST_TYPE | STATUS_TYPE,

		// Response types
		ANGLE_RESPONSE		= RESPONSE_TYPE | 0x1,
		NEWANGLE_RESPONSE		= RESPONSE_TYPE | 0x2,
		STATUS_RESPONSE		= RESPONSE_TYPE | STATUS_TYPE,
		CONTROLLER_STATUS	= STATUS_RESPONSE | 0x1,
		DRIVER_STATUS		= STATUS_RESPONSE | 0x2
	};

	enum Error : byte
	{
		SUCCESS = 0
	};

	enum ControllerStatus : byte
	{
		NONE
	};

	enum DriverStatus : byte
	{
		IDLE
	};

#pragma endregion ENUMS



#pragma region FORWARD DECLARATIONS & TYPE ALIASES


	//  TEMPLATED TYPE FORWARD DECLARATIONS & ALIASES

	template<class TMessage, MessageCode TCode>
	class Message;
	template<class TMessage, MessageCode TCode>
	using MESSAGE = Message<TMessage, TCode>;
	template<class TMessage, MessageCode TCode>
	using PMESSAGE = Message<TMessage, TCode> *;
	template<class TMessage, MessageCode TCode>
	using RMESSAGE = Message<TMessage, TCode> &;
	template<class TMessage, MessageCode TCode>
	using PPMESSAGE = Message<TMessage, TCode> **;
	template<class TMessage, MessageCode TCode>
	using RRMESSAGE = Message<TMessage, TCode> &&;
	template<class TMessage, MessageCode TCode>
	using CMESSAGE = const Message<TMessage, TCode>;
	template<class TMessage, MessageCode TCode>
	using PCMESSAGE = const Message<TMessage, TCode> *;
	template<class TMessage, MessageCode TCode>
	using RCMESSAGE = const Message<TMessage, TCode> &;
	template<class TMessage, MessageCode TCode>
	using PPCMESSAGE = const Message<TMessage, TCode> **;


	// REQUESTS

	class Request;
	typedef Request REQUEST, *PREQUEST, &RREQUEST;
	typedef const Request CREQUEST, *CPREQUEST, &CRREQUEST;

	class AngleRequest;
	typedef AngleRequest ANGLEREQUEST, *PANGLEREQUEST, &RANGLEREQUEST;
	typedef const AngleRequest CANGLEREQUEST, *CPANGLEREQUEST, &CRANGLEREQUEST;

	class NewAngleRequest;
	typedef NewAngleRequest NEWANGLEREQUEST, *PNEWANGLEREQUEST, &RNEWANGLEREQUEST;
	typedef const NewAngleRequest CNEWANGLEREQUEST, *CPNEWANGLEREQUEST, &CRNEWANGLEREQUEST;

	class StatusRequest;
	typedef StatusRequest STATUSREQUEST, *PSTATUSREQUEST, &RSTATUSREQUEST;
	typedef const StatusRequest CSTATUSREQUEST, *CPSTATUSREQUEST, &CRSTATUSREQUEST;


	// RESPONSES

	class Response;
	typedef Response RESPONSE, *PRESPONSE, &RRESPONSE;
	typedef const Response CRESPONSE, *CPRESPONSE, &CRRESPONSE;

	class AngleResponse;
	typedef AngleResponse ANGLERESPONSE, *PANGLERESPONSE, &RANGLERESPONSE;
	typedef const AngleResponse CANGLERESPONSE, *CPANGLERESPONSE, &CRANGLERESPONSE;

	class NewAngleResponse;
	typedef NewAngleResponse NEWANGLERESPONSE, *PNEWANGLERESPONSE, &RNEWANGLERESPONSE;
	typedef const NewAngleResponse CNEWANGLERESPONSE, *CPNEWANGLERESPONSE, &CRNEWANGLERESPONSE;

	class StatusResponse;
	typedef StatusResponse STATUSRESPONSE, *PSTATUSRESPONSE, &RSTATUSRESPONSE;
	typedef const StatusResponse CSTATUSRESPONSE, *CPSTATUSRESPONSE, &CRSTATUSRESPONSE;

	class ControllerStatusResponse;
	typedef ControllerStatusResponse CONTROLLERSTATUSRESPONSE, *PCONTROLLERSTATUSRESPONSE, &RCONTROLLERSTATUSRESPONSE;
	typedef const ControllerStatusResponse CCONTROLLERSTATUSRESPONSE, *CPCONTROLLERSTATUSRESPONSE, &CRCONTROLLERSTATUSRESPONSE;

	class DriverStatusResponse;
	typedef DriverStatusResponse DRIVERSTATUSRESPONSE, *PDRIVERSTATUSRESPONSE, &RDRIVERSTATUSRESPONSE;
	typedef const DriverStatusResponse CDRIVERSTATUSRESPONSE, *CPDRIVERSTATUSRESPONSE, &CRDRIVERSTATUSRESPONSE;

#pragma endregion FORWARD DECLARATIONS & TYPE ALIASES



#pragma region TYPE DECLARATIONS

	// Message

	template<class TMessage, MessageCode TCode>
	class Message
	{
	public:

		static constexpr MessageCode TYPE();
		static constexpr word SIZE();

		Message();
		Message(RFIELD);
		Message(PFIELD);

		VIRTUAL ~Message();

		virtual RFIELD operator[](size_t);

		virtual size_t ParamCount() const;
		virtual RFIELD GetParam(size_t = 0) const;

	protected:

		void RetrieveParamValue(PVOID, byte = 0);

		PFIELD _Params;
	};


	// REQUESTS

	class Request : public Message<Request, MessageCode::REQUEST_TYPE> { };


	class AngleRequest : public Message<AngleRequest, MessageCode::ANGLE_REQUEST> { };


	class NewAngleRequest : public Message<NewAngleRequest, MessageCode::NEWANGLE_REQUEST>
	{
	public:

		NewAngleRequest(const word);

		const word Degrees() const;
	};


	class StatusRequest : public Message<StatusRequest, MessageCode::STATUS_REQUEST> { };



	// RESPONSES

	class Response : public Message<Response, MessageCode::RESPONSE_TYPE>
	{
	public:

		Response(const Error);

		virtual const Error ErrorCode() const;

	protected:

		Error _ErrorCode = Error::SUCCESS;
	};


	class AngleResponse : public Message<AngleResponse, MessageCode::ANGLE_RESPONSE>
	{
	public:

		AngleResponse(const word);

		const word Degrees() const;
	};


	class NewAngleResponse : public Message<NewAngleResponse, MessageCode::NEWANGLE_RESPONSE> { };

	class StatusResponse : public Message<StatusResponse, MessageCode::STATUS_RESPONSE>
	{
	public:

		StatusResponse(const char *);

		const char * StatusMessage() const;

	protected:

		const char * _StatusMessage;
	};


	class ControllerStatusResponse : public Message<ControllerStatusResponse, MessageCode::CONTROLLER_STATUS>
	{
	public:

		ControllerStatusResponse(ControllerStatus, const char * = NULL);

		const ControllerStatus StatusCode() const;

	protected:

		ControllerStatus _StatusCode = ControllerStatus::NONE;
	};


	class DriverStatusResponse : public Message<DriverStatusResponse, MessageCode::DRIVER_STATUS>
	{
	public:

		DriverStatusResponse(DriverStatus, const char * = NULL);

		const DriverStatus StatusCode() const;

	protected:

		DriverStatus _StatusCode = DriverStatus::IDLE;
	};

#pragma endregion TYPE DECLARATIONS

}


#endif
