/**********************************************************************************************
* This file is part of the Itty Bitty Arduino library.                                        *
* Copyright � 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.            *
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS. *
***********************************************************************************************/

#include "IttyBitty_bits.h"

using namespace IttyBitty;


/* [BITPACK] DEFINITION */

#define __BIT(i) (this->b##i)


_BitPack::_BitPack() { }

_BitPack::_BitPack(_BitPack const & other)
{
	*(reinterpret_cast<PBYTE>(this)) = reinterpret_cast<RCBYTE>(other);
}

_BitPack::_BitPack(_BitPack && other)
{
	new (this) _BitPack(reinterpret_cast<RBYTE>(other));
}

_BitPack::_BitPack(RCBYTE value)
{
	*(reinterpret_cast<PBYTE>(this)) = value;
}


_BitPack & _BitPack::NULL_OBJECT()
{
	STATIC _BitPack NULL_BITPACK;
	return NULL_BITPACK;
}

CSIZE _BitPack::BitSize()
{
	return BIT_SIZE(_BitPack);
}


_BitPack & _BitPack::operator =(_BitPack const & rValue)
{
	*this = _BitPack(rValue);
	return *this;
}

_BitPack & _BitPack::operator =(_BitPack && rValue)
{
	*this = _BitPack(rValue);
	return *this;
}

_BitPack & _BitPack::operator =(RCBYTE rValue)
{
	*this = _BitPack(rValue);
	return *this;
}

BIT _BitPack::operator[](SIZE i) const
{
	return this->Bit(i);
}


BIT _BitPack::Bit(SIZE i) const
{
	if (i >= this->BitSize())
		return NULL;

	switch (i)
	{
	case 0:
		return __BIT(0);
	case 1:
		return __BIT(1);
	case 2:
		return __BIT(2);
	case 3:
		return __BIT(3);
	case 4:
		return __BIT(4);
	case 5:
		return __BIT(5);
	case 6:
		return __BIT(6);
	case 7:
		return __BIT(7);
	default:
		return NULL;
	}
}
