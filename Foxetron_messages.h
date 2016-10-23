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

	typedef enum MessageCode * PMESSAGECODE, & RMESSAGECODE;
	typedef const enum MessageCode * PCMESSAGECODE, & RCMESSAGECODE;

	typedef enum Error * PERROR, & RERROR;
	typedef const enum Error * PCERROR, & RCERROR;

	typedef enum ControllerStatus * PCONTROLLERSTATUS, & RCONTROLLERSTATUS;
	typedef const enum ControllerStatus * PCCONTROLLERSTATUS, & RCCONTROLLERSTATUS;

	typedef enum DriverStatus * PDRIVERSTATUS, & RDRIVERSTATUS;
	typedef const enum DriverStatus * PCDRIVERSTATUS, & RCDRIVERSTATUS;

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


#pragma region Request DECLARATIONS

	CLASS Request : public GenericMessage<MessageCode::REQUEST_TYPE>
	{
	public:

		typedef GenericMessage<MessageCode::REQUEST_TYPE> TBASE;

		VIRTUAL VOID Handle(...);


	protected:
		
		using TBASE::_Params;
	};


	CLASS AngleRequest : public GenericMessage<MessageCode::ANGLE_REQUEST>
	{
	public:

		typedef GenericMessage<MessageCode::ANGLE_REQUEST> TBASE;

		VIRTUAL VOID Handle(...);


	protected:
		
		using TBASE::_Params;
	};


	CLASS NewAngleRequest : public GenericMessage<MessageCode::NEWANGLE_REQUEST, 1>
	{
	public:

		typedef GenericMessage<MessageCode::NEWANGLE_REQUEST, 1> TBASE;

		NewAngleRequest(RCWORD);

		VIRTUAL RCWORD Degrees() const;

		VIRTUAL VOID Handle(...);

		
	protected:
		
		using TBASE::_Params;
	};


	CLASS StatusRequest : public GenericMessage<MessageCode::STATUS_REQUEST>
	{
	public:

		typedef GenericMessage<MessageCode::STATUS_REQUEST> TBASE;

		VIRTUAL VOID Handle(...);


	protected:
		
		using TBASE::_Params;
	};

#pragma endregion
	

#pragma region Response DECLARATIONS

	INTERFACE IResponse
	{
	public:

		VIRTUAL RCERROR ErrorCode() const = 0;


	protected:

		IResponse() { }
	};
	

	INTERFACE IStatusResponse : public virtual IResponse
	{
	public:

		VIRTUAL PCCHAR StatusMessage() const;


	protected:

		IStatusResponse() { }
	};


	CLASS Response : public GenericMessage<MessageCode::RESPONSE_TYPE, 1>, public virtual IResponse
	{
	public:

		typedef GenericMessage<MessageCode::RESPONSE_TYPE, 1> TBASE;

		Response(RCERROR);

		VIRTUAL RCERROR ErrorCode() const;

		VIRTUAL VOID Handle(...);
		

	protected:
		
		using TBASE::_Params;
	};


	CLASS AngleResponse : public GenericMessage<MessageCode::ANGLE_RESPONSE, 2>, public virtual IResponse
	{
	public:

		typedef GenericMessage<MessageCode::ANGLE_RESPONSE, 2> TBASE;

		AngleResponse(RCERROR, RCWORD);

		VIRTUAL RCERROR ErrorCode() const;

		VIRTUAL RCWORD Degrees() const;

		VIRTUAL VOID Handle(...);

	protected:
		
		using TBASE::_Params;
	};


	CLASS NewAngleResponse : public GenericMessage<MessageCode::NEWANGLE_RESPONSE, 1>, public virtual IResponse
	{
	public:

		typedef GenericMessage<MessageCode::NEWANGLE_RESPONSE, 1> TBASE;

		NewAngleResponse(RCERROR);

		VIRTUAL RCERROR ErrorCode() const;

		VIRTUAL VOID Handle(...);


	protected:
		
		using TBASE::_Params;
	};

	CLASS StatusResponse : public GenericMessage<MessageCode::STATUS_RESPONSE, 2>, public virtual IStatusResponse
	{
	public:

		typedef GenericMessage<MessageCode::STATUS_RESPONSE, 2> TBASE;

		StatusResponse(RCERROR, PCCHAR);

		VIRTUAL RCERROR ErrorCode() const;

		VIRTUAL PCCHAR StatusMessage() const;

		VIRTUAL VOID Handle(...);

		
	protected:
		
		using TBASE::_Params;
	};

	
	CLASS ControllerStatusResponse : public GenericMessage<MessageCode::CONTROLLER_STATUS, 3>, public virtual IStatusResponse
	{
	public:

		typedef GenericMessage<MessageCode::CONTROLLER_STATUS, 3> TBASE;

		ControllerStatusResponse(RCERROR, RCCONTROLLERSTATUS, PCCHAR = NULL);

		VIRTUAL RCERROR ErrorCode() const;

		VIRTUAL PCCHAR StatusMessage() const;

		VIRTUAL RCCONTROLLERSTATUS StatusCode() const;

		VIRTUAL VOID Handle(...);

		
	protected:
		
		using TBASE::_Params;
	};


	CLASS DriverStatusResponse : public GenericMessage<MessageCode::DRIVER_STATUS, 3>, public virtual IStatusResponse
	{
	public:

		typedef GenericMessage<MessageCode::DRIVER_STATUS, 3> TBASE;

		DriverStatusResponse(RCERROR, RCDRIVERSTATUS, PCCHAR = NULL);

		VIRTUAL RCERROR ErrorCode() const;

		VIRTUAL PCCHAR StatusMessage() const;
		
		VIRTUAL RCDRIVERSTATUS StatusCode() const;

		VIRTUAL VOID Handle(...);


	protected:
		
		using TBASE::_Params;
	};

#pragma endregion
}


#endif
