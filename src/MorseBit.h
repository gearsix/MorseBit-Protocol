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
#include "MicroIMG.h"
// defines
#define MorseBit_OFFSET			100		// used to counter physical innacuracies

#define MorseBit_INCOMING		2000	// if Pin is HI for this period, signals incoming message

#define MorseBit_MAX_BUFF		250		//@PLACEHOLDER	// max size of buffers used in protocol

#define MorseBit_DISPLAY_SPEED	35		// speed DOT and DASH scroll across MicroBit display

#define MorseBit_LETTERSPACE	'/'
#define MorseBit_DOT			'.'
#define MorseBit_DASH			'-'
#define MorseBit_TERMINATOR		';'

#define MorseBit_LETTERSPACE_TIME	2000
#define MorseBit_DOT_TIME			500
#define MorseBit_DASH_TIME			1500
#define MorseBit_DEAD_TIME			3500

//===========
// FUNCTIONS
//===========
/*
	Listens to button presses and translates into morse code.
	Only dots, dashes and letter spaces are implemented.

	@param MicroBit *uBit - MicroBit used for systemTime()
	@param MicroBitButton *button - MicroBitButton used to listen for morse code presses
	@param char *buff - buffer where morse code is stored
*/
void MorseBit_getMorseCode(MicroBit *uBit, MicroBitButton *button, char *buff);

/*
	Encrypts a char * of morse code into a uint8_t of encrypted indexes (uses the MorseBit encryption algorithm)

	@param char *morse - a char * of MorseBit_DOT, MorseBit_DASH and MorseBit_LETTERSAPCE
	@param uint8_t *encrypt - a uint8_t * (preferable of size MorseBit_MAX_BUFF) where the encrypted indexes will be stored
*/
void MorseBit_encryptMorse(char *morse, uint8_t *encrypt);

/*
	Decrypts a uint8_t * of encrypted indexes (via the MorseBit encryption algorithm) into a char * of characters

	@param uint8_t *encrypt - a uint8_t * of encrypted indexes
	@param char *buff - a char * where the decrypted characters will be stored (preferably of size MorseBit_MAX_BUFF)
*/
void MorseBit_decryptIndex(uint8_t *encrypt, char *buff);

#endif

