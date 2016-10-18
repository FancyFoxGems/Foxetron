/***********************************************************************************************
* [Foxetron_messages.h]: FOXETRON SERIAL PROTOCOL DATA classURES
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_MESSAGES_H
#define _FOXETRON_MESSAGES_H


#include <Arduino.h>


#define countof(var) (sizeof(var) / sizeof(0[var]))


namespace FoxetronMessaging
{

#pragma region GLOBAL CONSTANTS & VARIABLES

	static const char MESSAGE_MARKER[4] = "FOX";

	static const word BUFFER_SIZE = 200;

	static volatile char RX[BUFFER_SIZE];

#pragma endregion GLOBAL CONSTANTS & VARIABLES
	


#pragma region ENUMS
	
	enum DataSize : byte
	{
		ONE_BYTE	= 0x10,
		TWO_BYTES	= 0x20,
		THREE_BYTES	= 0x40,
		FOUR_BYTES	= 0x80,
	};

	enum DataType : byte
	{
		BYTES	= 0x0,

		BYTE	= ONE_BYTE | 0x1,
		CHAR	= ONE_BYTE | 0x2,
		BOOL	= ONE_BYTE | 0x4,

		WORD	= TWO_BYTES | 0x1,
		SHORT	= TWO_BYTES | 0x2,
		BITS	= TWO_BYTES | 0x4,

		DWORD	= FOUR_BYTES | 0x1,
		LONG	= FOUR_BYTES | 0x2,
		FLOAT	= FOUR_BYTES | 0x4
	};

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

	// FUNDAMENTAL TYPE ALIASES

	typedef uint32_t dword;
	typedef void * pvoid;


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

	// UNIVERSAL 4-BYTE DATA TYPE UNION
	typedef union _Datum
	{
		byte Bytes[4];

		struct
		{
			bool b0 : 1;
			bool b1 : 1;
			bool b2 : 1;
			bool b3 : 1;
			bool b4 : 1;
			bool b5 : 1;
			bool b6 : 1;
			bool b7 : 1;
			bool b8 : 1;
			bool b9 : 1;
			bool bA : 1;
			bool bB : 1;
			bool bC : 1;
			bool bD : 1;
			bool bE : 1;
			bool bF : 1;
		}
		Bits;

		byte ByteVal;
		char CharVal;
		bool BoolVal;

		word WordVal;
		short ShortVal;

		dword DWordVal;
		long LongVal;
		float FloatVal;
	}
	Datum, DATUM, * PDATUM, & RDATUM;

	typedef const union _Datum CDATUM, *CPDATUM, &CRDATUM;


	struct Field
	{
		Field(DataType, Datum);

		virtual ~Field();

		template <typename T = Datum>
		const T Value() const;

	protected:

		DataType _Type;
		Datum _Value;
	};

	typedef Field FIELD, *PFIELD, &RFIELD;
	typedef const Field CFIELD, *CPFIELD, &CRFIELD;


	template<class TMessage, MessageCode TCode>
	class Message
	{
	public:

		static constexpr MessageCode TYPE();
		static constexpr word SIZE();

		Message();
		Message(RFIELD);
		Message(PFIELD);

		virtual ~Message();

		virtual RFIELD operator[](size_t);

		virtual size_t ParamCount() const;
		virtual RFIELD GetParam(size_t = 0);

	protected:

		void RetrieveParamValue(pvoid, byte = 0);

		PFIELD _Params;
	};


	// REQUESTS

	class Request : public Message<Request, MessageCode::REQUEST_TYPE> { };


	class AngleRequest : public Message<AngleRequest, MessageCode::ANGLE_REQUEST>
	{
	public:

		AngleRequest();

		virtual ~AngleRequest();
	};


	class NewAngleRequest : public Message<NewAngleRequest, MessageCode::NEWANGLE_REQUEST>
	{
	public:

		NewAngleRequest(const word degrees);

		virtual ~NewAngleRequest();

		const word Degrees() const;
	};


	class StatusRequest : public Message<StatusRequest, MessageCode::STATUS_REQUEST>
	{
	public:

		StatusRequest();

		virtual ~StatusRequest();
	};



	// RESPONSES

	class Response : public Message<Response, MessageCode::RESPONSE_TYPE>
	{
	public:

		const Error ErrorCode() const;

	protected:

		Error _ErrorCode = Error::SUCCESS;
	};


	class AngleResponse : public Message<AngleResponse, MessageCode::ANGLE_RESPONSE>
	{
	public:

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

using namespace FoxetronMessaging;


#endif
