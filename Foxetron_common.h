/***********************************************************************************************
* [Foxetron_common.h]: SHARED INCLUDES, DEFINES, ETC. FOR FOXETRON PROGRAMS
*
* This file is part of the Foxetron program suite
* Copyright � 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_COMMON_H
#define _FOXETRON_COMMON_H


// GCC WARNING SUPPRESSIONS

#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wconversion-null"
#pragma GCC diagnostic ignored "-Wchar-subscripts"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wsequence-point"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#pragma GCC diagnostic ignored "-Wpointer-arith"
#pragma GCC diagnostic ignored "-Wvirtual-move-assign"


#pragma region INCLUDES

// PROJECT INCLUDES
#include "Foxetron_common.h"

// PROJECT MODULES
#include "Foxetron_messages.h"

using namespace Foxetron;

// ITTY BITTY
#include "IttyBitty.h"

using namespace IttyBitty;

#pragma endregion


#pragma region DEFINES

// PROGRAM OPTIONS

#define SERIAL_BAUD_RATE						115200			// (Debugging) UART baud rate
#define SERIAL_DELAY_MS							1				// Delay for waiting on serial buffer to flush when printing debug statements

#define DEBUG_MEMORY_INFO_INTERVAL_MS			3000			// Period by which available RAM should be printed when debugging

#define DEBUG_INPUTS							0				// Whether to print values of pin input signals
#define DEBUG_INPUTS_INTERVAL_MS				500				// Period by which input signal values should be printed when debugging

#if defined(DEBUG_INPUTS) && DEBUG_INPUTS != 1
	#undef DEBUG_INPUTS
#endif

#define ANGLE_DEGREE_PRECISION_FACTOR			100				// Scaling factor of �s for angle measurement
#define ANGLE_ACTUAL_DEGRESS_PER_DEGREE_VALUE	ANGLE_DEGREE_PRECISION_FACTOR


// PROGRAM CONSTANTS

#define uS_PER_SECOND								1000000

#define PROCESS_TIMER_PRESCALE_FACTOR				1024
#define PROCESS_TIMER_OVERFLOW_uS					((CDWORD)PROCESS_TIMER_PRESCALE_FACTOR * uS_PER_SECOND / F_CPU)

#define CALIBRATION_ANGLE_DEGREES					4500

#pragma endregion

#endif