/* INCLUDES */



/* DEFINES */

// PROGRAM OPTIONS

#define DEBUG_INPUTS			0
#define SERIAL_ENABLE
#define SERIAL_BAUD_RATE		115200

#if defined(DEBUG_INPUTS)
	#if DEBUG_INPUTS != 1
		#undef DEBUG_INPUTS
	#elif !defined(SERIAL_ENABLE)
		#define SERIAL_ENABLE
	#endif
#endif


// INPUT PINS

#define PIN_ANGLE_ENCODER_A		2	// Pin 2 / PD2 (INT0)
#define PIN_ANGLE_ENCODER_B		3	// Pin 3 / PD3 (INT1)


// OUTPUT PINS



/* FLASH DATA */



/* EEPROM VARIABLES */



/* PROGRAM CONSTANTS */



/* GLOBAL VARIABLES */

// INPUTS

volatile bool _AngleEncoderA = 0;	// Pin 2 / PD2 (INT0)
volatile bool _AngleEncoderB = 0;	// Pin 3 / PD3 (INT1)

volatile bool _angleUp			= 0;
volatile uint32_t _angleReading	= 0;
word _angleDelta				= 0;
word _angleVelocity				= 0;


// OUTPUTS



/* PROGRAM CODE */

// PROGRAM OUTLINE: ENTRY POINT & LOOP

void setup()
{
#ifdef SERIAL_ENABLE
	Serial.begin(SERIAL_BAUD_RATE);
#endif

	initializePins();
	initializeInterrupts();
}

void loop()
{
#ifdef DEBUG_INPUTS

	_DEBUG_printInputValues();

#endif
}


/* INTERRUPT VECTORS and SUPPORTING MACROS & INLINED FUNCTIONS */

// INT0/INT1: ANGLE ENCODER

#define _ISR_ANGLE_ENCODER_READ_CHANNEL(channel, other_channel, increment_comparison)					\
	_AngleEncoder ## channel = !_AngleEncoder ## channel;												\
	_angleUp = (_AngleEncoder ## channel increment_comparison _AngleEncoder ## other_channel);			\
	_ISR_AngleEncoder_updateAngleReading();

static inline void _ISR_AngleEncoder_updateAngleReading() __attribute__((always_inline));

static inline void _ISR_AngleEncoder_updateAngleReading()
{
	if (_angleUp)
		++_angleReading;
	else
		--_angleReading;
}

// CHANNEL A
ISR(INT0_vect)
{
	_ISR_ANGLE_ENCODER_READ_CHANNEL(A, B, ==);
}

// CHANNEL B
ISR(INT1_vect)
{
	_ISR_ANGLE_ENCODER_READ_CHANNEL(B, A, != );}// TIMER EVENTS// TIMER 2 OVERFLOWISR(TIMER2_OVF_vect, ISR_NOBLOCK){}// SERIAL EVENTS// USART RECEIVE/*ISR(USART_RX_vect, ISR_NOBLOCK){}*/

// PROGRAM FUNCTIONS

void initializePins()
{
	// REAR PINS

	// [Pins 0 (RX) & 1 (TX): USART serial communication --[angle feedback, commands, etc.]--> AVR uC stepper motor angle controller]

	// Angle encoder
	pinMode(PIN_ANGLE_ENCODER_A, INPUT);			// Channel A
	pinMode(PIN_ANGLE_ENCODER_B, INPUT);			// Channel B
}

void initializeInterrupts()
{
	// External interrupts: Angle encoder
	EIMSK |= 0b00000011;
	EICRA &= 0b11110101;
	EICRA |= 0b00000101;
}


// DEBUG UTILITY FUNCTIONS


void _DEBUG_printInputValues()
{
	static char valStr[2];

	_AngleEncoderA = digitalRead(2);
	_AngleEncoderB = digitalRead(3);

	Serial.print(itoa(_AngleEncoderA, valStr, 2));
	Serial.print(" ");
	Serial.print(itoa(_AngleEncoderB, valStr, 2));

	Serial.println();
}
