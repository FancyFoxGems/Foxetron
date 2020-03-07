/***********************************************************************************************
* [Foxetron_common.h]: SHARED INCLUDES, DEFINES, ETC. FOR FOXETRON PROGRAMS
*
* This file is part of the Foxetron program suite
* Copyright © 2016 Thomas J. Biuso III  ALL RIGHTS RESERVED...WHATEVER THAT MEANS.
* RELEASED UNDER THE GPL v3.0 LICENSE; SEE <LICENSE> FILE WITHIN DISTRIBUTION ROOT FOR TERMS.
***********************************************************************************************/

#ifndef _FOXETRON_COMMON_H
#define _FOXETRON_COMMON_H


#pragma region INCLUDES

// PROJECT MODULES
#include "Foxetron_messages.h"

using namespace Foxetron;

// ITTY BITTY
#include "IttyBitty.h"

using namespace IttyBitty;

#pragma endregion


#pragma region GCC WARNING SUPPRESSIONS

IGNORE_WARNING(unknown-pragmas)
IGNORE_WARNING(unused-function)
IGNORE_WARNING(unused-variable)
IGNORE_WARNING(unused-but-set-variable)
IGNORE_WARNING(unused-value)
IGNORE_WARNING(parentheses)
IGNORE_WARNING(return-type)
IGNORE_WARNING(conversion-null)
IGNORE_WARNING(char-subscripts)
IGNORE_WARNING(reorder)
IGNORE_WARNING(sequence-point)
IGNORE_WARNING(sign-compare)
IGNORE_WARNING(strict-aliasing)
IGNORE_WARNING(pointer-arith)
IGNORE_WARNING(virtual-move-assign)

#pragma endregion


#pragma region DEFINES

// PROGRAM CONSTANTS

#define uS_PER_SECOND							1000000UL

#define PROCESS_TIMER_PRESCALER_FLAGS			B(CS22) | B(CS21) | B(CS20)
#define PROCESS_TIMER_PRESCALE_FACTOR			1024
#define PROCESS_TIMER_OVERFLOW_uS				((CDWORD)PROCESS_TIMER_PRESCALE_FACTOR * uS_PER_SECOND / F_CPU)

#define CALIBRATION_ANGLE_DEGREES				4500


// PROGRAM OPTIONS

#define SERIAL_BAUD_RATE						115200			// UART baud rate

#define ANGLE_DEGREE_PRECISION_FACTOR			100				// Scaling factor of °s for angle measurement
#define ANGLE_ACTUAL_DEGRESS_PER_DEGREE_VALUE	ANGLE_DEGREE_PRECISION_FACTOR


#ifdef _DEBUG

#define DEBUG_MEMORY							0				// Whether to print available RAM
#define DEBUG_MEMORY_INTERVAL_MS				3000			// Period by which available RAM should be printed when debugging

#if defined(DEBUG_MEMORY) AND DEBUG_MEMORY != 1
	#undef DEBUG_MEMORY
#endif

#if defined(DEBUG_MEMORY_INTERVAL_MS) AND (!defined(DEBUG_MEMORY) OR defined(DEBUG_MEMORY_INTERVAL_MS) AND DEBUG_MEMORY_INTERVAL_MS == 0)
	#undef DEBUG_MEMORY_INTERVAL_MS
#endif

#define DEBUG_INPUTS							0				// Whether to print values of pin input signals
#define DEBUG_INPUTS_INTERVAL_MS				500				// Period by which input signal values should be printed when debugging

#if defined(DEBUG_INPUTS) AND DEBUG_INPUTS != 1
	#undef DEBUG_INPUTS
#endif

#if defined(DEBUG_INPUTS_INTERVAL_MS) AND (!defined(DEBUG_INPUTS) OR defined(DEBUG_INPUTS_INTERVAL_MS) AND DEBUG_INPUTS_INTERVAL_MS == 0)
	#undef DEBUG_INPUTS_INTERVAL_MS
#endif

#define DEBUG_STATE								0				// Whether to print values of state variables
#define DEBUG_STATE_INTERVAL_MS					3000			// Period by which state variable values should be printed when debugging

#if defined(DEBUG_STATE) AND DEBUG_STATE != 1
	#undef DEBUG_STATE
#endif

#if defined(DEBUG_STATE_INTERVAL_MS) AND (!defined(DEBUG_STATE) OR defined(DEBUG_STATE_INTERVAL_MS) AND DEBUG_STATE_INTERVAL_MS == 0)
	#undef DEBUG_STATE_INTERVAL_MS
#endif

#define DEBUG_MESSAGES							0				// Whether to print messages received

#if defined(DEBUG_MESSAGES) AND DEBUG_MESSAGES != 1
#undef DEBUG_MESSAGES
#endif

#endif

#pragma endregion


#pragma region PROGRAM FUNCTION DECLARATIONS

VOID OnError(PCCHAR = NULL) WEAK;

#pragma endregion

#endif
