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
// defines
#define LO 0
#define HI 1

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
static void translateMorseToAlphabet(char *morse_code, char *buffer)
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
static void translateMorseToIndexes(MicroBit *uBit, char *morse_code, uint8_t indexes[])
{
	int i = 1;
	uint8_t bit = 1;	// bit used to traverse binary tree

	for (unsigned int m = 0; m < strlen(morse_code); m++)
	{
		switch(morse_code[m])
		{
			case MorseBit_DOT:
				bit = (bit << 1);		//bitshift 0
				break;
			case MorseBit_DASH:
				bit = (bit << 1) | 1;	//bitshift 1
				break;
			case MorseBit_TERMINATOR:
				m = strlen(morse_code)+1;
				indexes[i] = bit;
				i++;
				bit = 1;
				break;
			case MorseBit_LETTERSPACE:
				indexes[i] = bit;
				i++;
				bit = 1;
				break;
		}
	}
	
	indexes[0] = i-1;
}

// public
//see MorseBit.h for doc
void MorseBit_getMorseCode(MicroBit *uBit, MicroBitButton *button, MicroBitPin *pin, char *buff)
{
	bool finished = false;
	char morse_code[MorseBit_MAX_BUFFER] = { 0 };
	int m = 0;	//morse_code index
	long dead_counter = -1;
	unsigned long timestamp, duration;

	// signal to partner msg is incoming
		pin->setDigitalValue(HI);

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
			// print to show finished
			uBit->display.print("DONE", 75);
				// tick if input == valid mcode
				// cross if not ?
			morse_code[m] = MorseBit_TERMINATOR;
			uBit->serial.printf("getMorseCode(): %s\r\n", morse_code);
			strcpy(buff, morse_code);
			// check for prosign, return value ?
			finished = true;
		}
	}
	pin->setDigitalValue(LO);
}

void MorseBit_encrypt(MicroBit *uBit, char *morse_code, uint8_t *encryption)
{
	uint8_t indexes[MorseBit_MAX_BUFFER] = {0};

	translateMorseToIndexes(uBit, morse_code, indexes);

	//cp size
	encryption[0] = indexes[0];
	// start at 1 so we don't overwrite size
	for (size_t i = 1; indexes[i] != 0; i++) {
		uint8_t num = 0;
		uint8_t encNum = 0;

		// get number from array and DIVIDED by 2 then - 1
		num = indexes[i]/2;
		encNum = num - 1;

		// send to encryptedIndex
		encryption[i] = encNum;
	}
}
 

static void tx(uint8_t value, MicroBit *uBit, MicroBitPin *pin)
{
	uint8_t bit;
	unsigned long timestamp; 
	for (int i = 7; i >= 0; i--)
	{
		bit = value >> i;

		timestamp = uBit->systemTime();
		while ((uBit->systemTime() - timestamp) != MorseBit_BIT_MS)
			pin->setDigitalValue(bit & 1);
		pin->setDigitalValue(0);
	}
}

bool MorseBit_tx(MicroBit *uBit, MicroBitPin *pin, char *buffer)
{
	unsigned long timestamp;
	uint8_t encryption[MorseBit_MAX_BUFFER] = {0};

	MorseBit_encrypt(uBit, buffer, encryption);

	uBit->serial.printf("e: %i\r\n", encryption[0]);

	uBit->sleep(1000);
	// note: encryption[0] = num letters
	for (uint8_t i = 0; i < encryption[0]+1; i++)
	{
		uBit->sleep(1000);
		uBit->serial.printf("v: %u\r\n", encryption[i]);
		tx(encryption[i], uBit, pin);
	}
}


/*
bool MorseBit_recieveMorseBit(MicroBit *uBit, MicroBitPin *pin, char *recv)
{
	bool finished = false;
	unsigned long lo_time, hi_time;
	unsigned long timestamp;
	uint8_t bit = 1;
	while (!finished)
	{
		// each packet = 6 bits
		for (int i = 0; i < 6; i++)
		{
			// get current time
			timestamp = uBit->systemTime();
			// wait for pin to go LO
			while (pin->getDigitalValue() == HI)
			{
			}
			// count how long pin was HI
			hi_time = uBit->systemTime() - timestamp;

			// if HI == BIT_MS
			if (hi_time >= (MorseBit_BIT_MS - MorseBit_OFFSET) && hi_time <= (MorseBit_BIT_MS + MorseBit_OFFSET))
			{
				bit = (bit << 1) | 1;	// bitshift 1
			}
			// else start counting LO
			else (lo_time == -1)
				lo_time = uBit->systemTime();
			// if LO == BIT_MS
			else if (lo_time >= (MorseBit_BIT_MS - MorseBit_OFFSET) && lo_time <= (MorseBit_BIT_MS + MorseBit_OFFSET))
			{
				bit = (bit << 1)		// bitshift 0
			}
		}

		uBit->serial.printf("SIZE: %u\r\n", bit);
		uBit->sleep(1000);
	}

	return true;
}
*/

