/*-------------------------------------------------------------------------------------------
|	Project:		Challenge2-UFCFVK-15-2
|	File:			MorseBit.c
|	Authors:		Alexander Collins, Wei Jun
|	Description:	Header for the MorseBit protocol
|	Notes:			Hard-tabs (4 spaces)
|	Date:			Feb, 2018
-------------------------------------------------------------------------------------------*/
//========
// MACROS
//========
// includes
#include "MorseBit.h"

//=========
// GLOBALS
//=========
static char alphabet[50] = {
	' ',	//space
	0,		//*
	'E',	//2
	'T',	//3
	'I',	//4
	'A',	//5
	'N',	//6
	'M',	//7
	'S',	//8
	'U',	//9
	'R',	//10
	'W',	//11
	'D',	//12
	'K',	//13
	'G',	//14
	'O',	//15
	'H',	//16
	'V',	//16
	'F',	//17
	0,
	'L',	//19
	0,
	'P',	//21
	'J',	//22
	'B',	//23
	'X',	//24
	'C',	//25
	'Y',	//26
	'Z',	//27
	'Q',	//28
};

//===========
// FUNCTIONS
//===========
// private
static void translateMorse(char *morse_code, char *buffer)
{
	uint8_t bit = 1;	// bit used to traverse binary tree

	for (uint8_t i = 0; i < strlen(morse_code); i++)
	{
		switch(morse_code[i])
		{
			case MorseBit_DOT:
				bit = (bit << 1);		// bitshift 0
				break;
			case MorseBit_DASH:
				bit = (bit << 1) | 1;	// bitshift 1
				break;
			case MorseBit_TERMINATOR:
				i = strlen(morse_code)+1;
				// fall-through
			case MorseBit_LETTERSPACE:
				*buffer++ = alphabet[bit];
				bit = 1;
				break;
		}
	}
}

// public
//see MorseBit.h for doc
bool MorseBit_getMorseCode(MicroBit *uBit, MicroBitButton *button, char *buff)
{
	bool finished = false;
	char morse_code[MorseBit_MAX_BUFFER] = { 0 };
	int m = 0;	//morse_code index
	long dead_counter = -1;
	unsigned long timestamp, duration;

	while (!finished)
	{
		// get button input time
		timestamp = uBit->systemTime();
		while (button->isPressed())
		{
			// wait for button to be released
		}
		duration = uBit->systemTime() - timestamp;

		// check button input time
		if (duration > MorseBit_OFFSET && (uBit->systemTime() - dead_counter) < MorseBit_DEAD_TIME)
		{
			//uBit->serial.printf("press: %u\tdead: %u\r\n", duration, uBit->systemTime() - dead_counter);
			// check dead_counter for LETTER SPACE
			if ((uBit->systemTime() - dead_counter) >= MorseBit_LETTERSPACE_GAP)// && (uBit->systemTime() - dead_counter) < MorseBit_WORDSPACE_GAP)
			{
			//	uBit->display.scrollAsync(MorseBit_LETTERSPACE, 75);
				morse_code[m++] = MorseBit_LETTERSPACE;
			}

			dead_counter = -1;	// reset dead_counter

			// if DOT
			if (duration <= MorseBit_DOT_HOLD)
			{
				uBit->display.scrollAsync(MorseBit_DOT, MorseBit_DISPLAY_SPEED);
				morse_code[m++] = MorseBit_DOT;
			}
			// if DASH
			else if (duration > MorseBit_DOT_HOLD && duration <= MorseBit_DASH_HOLD)
			{
				uBit->display.scrollAsync(MorseBit_DASH, MorseBit_DISPLAY_SPEED);
				morse_code[m++] = MorseBit_DASH;
			}
		}
		// if no input detected, start dead_counter
		else if (dead_counter == -1)
			dead_counter = uBit->systemTime();

		// end of input
		if ((uBit->systemTime() - dead_counter) == MorseBit_DEAD_TIME)
		{
			morse_code[m] = MorseBit_TERMINATOR;
			/* ENCRYPT */
			/* SEND OVER PIN */
//			uBit->serial.printf("buff: %s\r\n", morse_code);
//			translateMorse(morse_code, buff);
			finished = true;
		}
	}

	return true;
}

