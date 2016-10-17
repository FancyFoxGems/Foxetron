/**********************************************************************************************
* This file is part of the Foxetron program suite.                                            *
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/


#include "Foxetron_messages.h"


#pragma region MESSAGE DEFINITION

// STATIC MESSAGE "OVERRIDES"

template<class T, MessageType TMsg>
constexpr MessageType Message<T, TMsg>::TYPE()
{
	return TMsg;
}

template<class T, MessageType TMsg>
constexpr word Message<T, TMsg>::SIZE()
{
	return static_cast<word>(sizeof(T));
}

#pragma endregion MESSAGE DEFINITION
