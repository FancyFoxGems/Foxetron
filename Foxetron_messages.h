/***********************************************************************************************
* [Foxetron_messages.h]: FOXETRON SERIAL PROTOCOL DATA STRUCTURES
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_MESSAGES_H
#define _FOXETRON_MESSAGES_H


#include <Arduino.h>


#pragma region ENUMS

enum DataType : byte
{
	BYTES,
	BOOL,
	BYTE,
	CHAR,
	WORD,
	SHORT,
	DWORD,
	LONG,
	FLOAT
};

enum FoxetronError : byte
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



#pragma region TYPEDEFS

typedef uint32_t dword;

typedef union _MESSAGE
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
	} Bits;

	bool BoolVal;
	byte ByteVal;
	char CharVal;
	word WordVal;
	short IntVal;
	dword DWordVal;
	long LongVal;
	float FloatVal;
}
Datum, DATUM, * PDATUM, & RDATUM;

typedef const struct _Datum CDATUM, *CPDATUM, &CRDATUM;


typedef struct _Message
{
	DataType DatumType;
	Datum Data;
}
Message, MESSAGE, * PMESSAGE, & RMESSAGE;

typedef const struct _Message CMESSAGE, *CPMESSAGE, &CRMESSAGE;


// REQUESTS

typedef struct _Request : public Message
{

}
Request, REQUEST, * PREQUEST, & RREQUEST;

typedef const struct _Request CREQUEST, * CPREQUEST, & CRREQUEST;


typedef struct _StatusRequest : public Request
{

}
StatusRequest, STATUSREQUEST, *PSTATUSREQUEST, &RSTATUSREQUEST;

typedef const struct _StatusRequest CSTATUSREQUEST, *CPSTATUSREQUEST, &CRSTATUSREQUEST;


typedef struct _AngleRequest : public Request
{

}
AngleRequest, ANGLEREQUEST, * PANGLEREQUEST, & RANGLEREQUEST;

typedef const struct _AngleRequest CANGLEREQUEST, * CPANGLEREQUEST, & CRANGLEREQUEST;


typedef struct _MotorRequest : public Request
{

}
MotorRequest, MOTORREQUEST, *PMOTORREQUEST, &RMOTORREQUEST;

typedef const struct _MotorRequest CMOTORREQUEST, *CPMOTORREQUEST, &CRMOTORREQUEST;



// RESPONSES

typedef struct _Response : public Message
{
	FoxetronError ErrorCode = FoxetronError::SUCCESS;
}
Response, RESPONSE, *PRESPONSE, &RRESPONSE;

typedef const struct _Response CRESPONSE, * CPRESPONSE, & CRRESPONSE;


typedef struct _StatusResponse : public Response
{
	const char * StatusMessage;
}
StatusResponse, STATUSRESPONSE, *PSTATUSRESPONSE, &RSTATUSRESPONSE;

typedef const struct _StatusResponse CSTATUSRESPONSE, *CPSTATUSRESPONSE, &CRSTATUSRESPONSE;


typedef struct _ControllerStatusResponse : public StatusResponse
{
	ControllerStatus StatusCode = ControllerStatus::NONE;
}
ControllerStatusResponse, CONTROLLERSTATUSRESPONSE, *PCONTROLLERSTATUSRESPONSE, &RCONTROLLERSTATUSRESPONSE;

typedef const struct _ControllerStatusResponse CCONTROLLERSTATUSRESPONSE, *CPCONTROLLERSTATUSRESPONSE, &CRCONTROLLERSTATUSRESPONSE;


typedef struct _DriverStatusResponse : public StatusResponse
{
	DriverStatus StatusCode = DriverStatus::IDLE;
}
DriverStatusResponse, DRIVERSTATUSRESPONSE, *PDRIVERSTATUSRESPONSE, &RDRIVERSTATUSRESPONSE;

typedef const struct _DriverStatusResponse CDRIVERSTATUSRESPONSE, *CPDRIVERSTATUSRESPONSE, &CRDRIVERSTATUSRESPONSE;


typedef struct _AngleResponse : public Response
{

}
AngleResponse, ANGLERESPONSE, *PANGLERESPONSE, &RANGLERESPONSE;

typedef const struct _AngleResponse CAngleResponse, *CPAngleResponse, &CRAngleResponse;


typedef struct _MotorResponse : public Response
{

}
MotorResponse, MOTORRESPONSE, *PMOTORRESPONSE, &RMOTORRESPONSE;

typedef const struct _MotorResponse CMOTORRESPONSE, *CPMOTORRESPONSE, &CRMOTORRESPONSE;


#pragma endregion TYPEDEFS





#endif
