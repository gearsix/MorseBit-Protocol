/*-------------------------------------------------------------------------------------------
|	Project:		Challenge2-UFCFVK-15-2
|	File:			MorseBit.h
|	Authors:		Alexander Collins, Wei Jun
|	Description:	Header for the MorseBit protocol
|	Notes:			Hard-tabs (4 spaces)
|	Date:			Feb, 2018
-------------------------------------------------------------------------------------------*/
#ifndef _MORSEBIT_
#define _MORSEBIT_

//========
// MACROS
//========
// includes
#include "MicroBit.h"
// defines
#define MorseBit_OFFSET					100		// used to counter physical innacuracies
#define MorseBit_PIN_INCOMING_MSG		5100

#define MorseBit_DOT_HOLD			500		// user holds button for .5 secs = DOT
#define MorseBit_DASH_HOLD			1500	// user holds button for 1.5 secs = DASH
#define MorseBit_LETTERSPACE_GAP	2000	// user releases button for 1 sec = LETTER SPACE
#define MorseBit_DEAD_TIME			4000	// user releases button for 4 secs = FINISHED (aka DEAD)

#define MorseBit_DOT			'.'	// symbol used to characterise DOT
#define MorseBit_DASH			'-'	// symbol used to characterise DASH
#define MorseBit_LETTERSPACE	'/'	// symbol used to characterise LETTER SPACES
#define MorseBit_TERMINATOR		';'	// doc

#define MorseBit_DISPLAY_SPEED	75	// speed the symbols scroll across the display

#define MorseBit_MAX_BUFFER		250		//@NOTE: 250 = placeholder

#define MorseBit_BIT_MS		200

//===========
// FUNCTIONS
//===========
/*
	@TODO doc
*/
void MorseBit_getMorseCode(MicroBit *uBit, MicroBitButton *btn, MicroBitPin *pin, char *buff);
/*
	@TODO doc
*/
void MorseBit_encrypt(char *morse_code, int *encryption);

bool MorseBit_tx(MicroBit *uBit, MicroBitPin *pin, char *buffer);

#endif	//_MORSEBIT_

