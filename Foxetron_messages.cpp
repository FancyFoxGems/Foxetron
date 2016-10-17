/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_messages.h"


#pragma region MESSAGE DEFINITION

// STATIC MESSAGE "OVERRIDES"

template<class TMessage, MessageCode TCode>
constexpr MessageCode Message<TMessage, TCode>::TYPE()
{
	return TCode;
}

template<class TMessage, MessageCode TCode>
constexpr word Message<TMessage, TCode>::SIZE()
{
	return static_cast<word>(sizeof(TMessage));
}

#pragma endregion MESSAGE DEFINITION
