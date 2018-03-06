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
#define MorseBit_OFFSET				100		// used to counter physical innacuracies

#define MorseBit_INCOMING			2000	// if Pin is HI for this period, signals incoming message

#define MorseBit_MAX_BUFF			250		// max size of buffers used in protocol

#define MorseBit_DISPLAY_SPEED		35		// speed DOT and DASH scroll across MicroBit display

#define MorseBit_LETTERSPACE		'/'		// char used for LETTER SPACES
#define MorseBit_DOT				'.'		// char used for DOT
#define MorseBit_DASH				'-'		// char used for DASH
#define MorseBit_TERMINATOR			';'		// char used for TERMINATOR (end of sentence)

#define MorseBit_LETTERSPACE_TIME	2000	// time between presses to signal LETTER SPACE
#define MorseBit_DOT_TIME			500		// button hold time to signal DOT
#define MorseBit_DASH_TIME			1500	// button hold time to signal DASH
#define MorseBit_TERMINATOR_TIME	3500	// time between presses to signal TERMINATION

// see protocol docs for explaination
#define MorseBit_BIT_TIME			250		// timing used to signal a single bit during tx/rx
#define MorseBit_PACKET_SIZE		6		// max number of bits in each packet
#define MorseBit_PACKET_TIME		600		// timing used to wait between packets

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

	@note - assumes morse is a string of MorseBit_* characters
	@note - outputs a uint8_t * following the MorseBit protocol
*/
void MorseBit_encrypt(char *morse, uint8_t *encrypt);

/*
	Decrypts a uint8_t * of encrypted indexes (via the MorseBit encryption algorithm) into a char * of characters

	@param uint8_t *encrypt - a uint8_t * of encrypted indexes
	@param char *buff - a char * where the decrypted characters will be stored (preferably of size MorseBit_MAX_BUFF)

	@note - assumes encrypt is a uint8_t * that's a result of the MorseBit protocol encryption
*/
void MorseBit_decrypt(uint8_t *encrypt, char *buff);

/*
	Sends a set of integers (buffer) in binary over pin following the MorseBit protocol.

	@param MicroBit *uBit - MicroBit used for systemTime()
	@param MicroBitPin *pin - MicroBitPin used to set HI/LO value on
	@param uint8_t *buff - buffer that data is read from

	@note - assumes that buffer is an encrypted message (following the MorseBit protocol)
*/
void MorseBit_tx(MicroBit *uBit, MicroBitPin *pin, uint8_t *buffer);

/*
	Listens for MorseBit packets being recieved on pin.

	@param MicroBit *uBit - MicroBit used for systemTime()
	@param MicroBitPin *pin - MicroBitPin used to set HI/LO value on
	@param uint8_t *buff - buffer that data is stored in

	@note - buffer should be an encrypted message (following the MorseBit protocol)
*/
void MorseBit_rx(MicroBit *uBit, MicroBitPin *pin, uint8_t *buffer);

#endif

