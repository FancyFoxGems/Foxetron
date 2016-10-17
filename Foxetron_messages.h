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

namespace FoxetronMessaging
{

#pragma region GLOBAL CONSTANTS
#pragma endregion GLOBAL CONSTANTS
	


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

	enum MessageType : byte
	{
		// Primary types
		REQUEST_TYPE		= 0x0,
		RESPONSE_TYPE		= 0x80,


		// Categories
		ANGLE_TYPE			= 0x10,
		MOTOR_TYPE			= 0x20,
		STATUS_TYPE			= 0x40,


		// Request types
		ANGLE_REQUEST		= REQUEST_TYPE | ANGLE_TYPE,
		MOTOR_REQUEST		= REQUEST_TYPE | MOTOR_TYPE,
		STATUS_REQUEST		= REQUEST_TYPE | STATUS_TYPE,

		// Response types
		ANGLE_RESPONSE		= RESPONSE_TYPE | 0x1,
		MOTOR_RESPONSE		= RESPONSE_TYPE | 0x2,
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


	//  TEMPLATED TYPE FORWARD DECLARATIONS & ALIASES

	template<class T, MessageType TMsg>
	class Message;
	template<class T, MessageType TMsg>
	using MESSAGE = Message<T, TMsg>;
	template<class T, MessageType TMsg>
	using PMESSAGE = Message<T, TMsg> *;
	template<class T, MessageType TMsg>
	using RMESSAGE = Message<T, TMsg> &;
	template<class T, MessageType TMsg>
	using PPMESSAGE = Message<T, TMsg> **;
	template<class T, MessageType TMsg>
	using RRMESSAGE = Message<T, TMsg> &&;
	template<class T, MessageType TMsg>
	using CMESSAGE = const Message<T, TMsg>;
	template<class T, MessageType TMsg>
	using PCMESSAGE = const Message<T, TMsg> *;
	template<class T, MessageType TMsg>
	using RCMESSAGE = const Message<T, TMsg> &;
	template<class T, MessageType TMsg>
	using PPCMESSAGE = const Message<T, TMsg> **;


	// REQUESTS

	class Request;
	typedef Request REQUEST, *PREQUEST, &RREQUEST;
	typedef const Request CREQUEST, *CPREQUEST, &CRREQUEST;

	class AngleRequest;
	typedef AngleRequest ANGLEREQUEST, *PANGLEREQUEST, &RANGLEREQUEST;
	typedef const AngleRequest CANGLEREQUEST, *CPANGLEREQUEST, &CRANGLEREQUEST;

	class MotorRequest;
	typedef MotorRequest MOTORREQUEST, *PMOTORREQUEST, &RMOTORREQUEST;
	typedef const MotorRequest CMOTORREQUEST, *CPMOTORREQUEST, &CRMOTORREQUEST;

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

	class MotorResponse;
	typedef MotorResponse MOTORRESPONSE, *PMOTORRESPONSE, &RMOTORRESPONSE;
	typedef const MotorResponse CMOTORRESPONSE, *CPMOTORRESPONSE, &CRMOTORRESPONSE;

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


	typedef struct _Field
	{
		DataType Type;
		Datum Value;
	}
	Field, FIELD, *PFIELD, &RFIELD;

	typedef const union _FIELD CFIELD, *CPFIELD, &CRFIELD;


	template<class T, MessageType TMsg>
	class Message
	{
		static constexpr MessageType TYPE();
		static constexpr word SIZE();
	};


	// REQUESTS

	class Request : public Message<Request, MessageType::REQUEST_TYPE>
	{
		Field Param;
	};


	class AngleRequest : public Message<AngleRequest, MessageType::ANGLE_REQUEST>
	{

	};


	class MotorRequest : public Message<MotorRequest, MessageType::MOTOR_REQUEST>
	{

	};


	class StatusRequest : public Message<StatusRequest, MessageType::STATUS_REQUEST>
	{

	};



	// RESPONSES

	class Response : public Message<Response, MessageType::RESPONSE_TYPE>
	{
		Error ErrorCode = Error::SUCCESS;
	};


	class AngleResponse : public Message<AngleResponse, MessageType::ANGLE_RESPONSE>
	{

	};


	class MotorResponse : public Message<MotorResponse, MessageType::ANGLE_RESPONSE>
	{

	};


	class StatusResponse : public Message<StatusResponse, MessageType::STATUS_RESPONSE>
	{
		const char * StatusMessage;
	};


	class ControllerStatusResponse : public Message<ControllerStatusResponse, MessageType::CONTROLLER_STATUS>
	{
		ControllerStatus StatusCode = ControllerStatus::NONE;
	};


	class DriverStatusResponse : public Message<DriverStatusResponse, MessageType::DRIVER_STATUS>
	{
		DriverStatus StatusCode = DriverStatus::IDLE;
	};

#pragma endregion TYPE DECLARATIONS

}

using namespace FoxetronMessaging;


#endif
