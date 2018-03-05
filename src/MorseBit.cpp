/*-------------------------------------------------------------------------------------------
|	Project:		Challenge2-UFCFVK-15-2
|	File:			MorseBit.cpp
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

#define RESET -1

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
static unsigned long getButtonPressTime(MicroBit *uBit, MicroBitButton *button)
{
	unsigned long timestamp = uBit->systemTime();
	while (button->isPressed());	// wait
	return uBit->systemTime() - timestamp;
}
static void translateMorseToIndex(char *morse, uint8_t *index)
{
	int i = 1;			// index for index (hah)
	uint8_t bit = 1;	// bit used to traverse binary tree (as bits are pushed onto it)

	for (size_t m = 0; m < strlen(morse); m++)
	{
		switch(morse[m])
		{
			case MorseBit_DOT:			bit = (bit << 1);	break;	// bitshift left 0
			case MorseBit_DASH:			bit = (bit << 1)|1;	break;	// bitshift left 1
			case MorseBit_TERMINATOR:
				m = strlen(morse);	// end loop
				index[i++] = bit;	// push bit
				bit = 1;			// reset bit
				break;
			case MorseBit_LETTERSPACE:
				index[i++] = bit;	// push bit
				bit = 1;			// reset bit
				break;
		}
	}

	index[0] = i - 1;	// index[0] = number of letters
}
// public
void MorseBit_getMorseCode(MicroBit *uBit, MicroBitButton *button, char *buff)
{
	bool finished = false;
	char morse[MorseBit_MAX_BUFF] = { 0 };
	int m = 0;					// index for morse[]
	long dead = RESET;	// counts dead(no user input) time
	unsigned long duration;

	while (!finished)
	{
		duration = getButtonPressTime(uBit, button);

		// if input detected
		if (duration > MorseBit_OFFSET && (uBit->systemTime() - dead) < MorseBit_DEAD_TIME)
		{
			// check for LETTERSPACE in dead
			if ((uBit->systemTime() - dead) >= MorseBit_LETTERSPACE_TIME)
				morse[m++] = MorseBit_LETTERSPACE;

			// reset dead counter
			dead = -1;
			
			// check for DOT in duration
			if (duration <= MorseBit_DOT_TIME)
			{
				uBit->display.scrollAsync(MorseBit_DOT, MorseBit_DISPLAY_SPEED);
				morse[m++] = MorseBit_DOT;
			}
			// check for DASH in duration
			else if (duration <= MorseBit_DASH_TIME)
			{
				uBit->display.scrollAsync(MorseBit_DASH, MorseBit_DISPLAY_SPEED);
				morse[m++] = MorseBit_DASH;
			}
		}
		// if no input detected
		else if (dead == RESET)
			dead = uBit->systemTime();	// start dead counter
		// if end of input
		else if ((uBit->systemTime() - dead) >= MorseBit_DEAD_TIME)
		{
			morse[m] = MorseBit_TERMINATOR;
		#if DEBUG
			uBit->serial.printf("MorseBit_getMorseCode(): %s\r\n", morse);
		#endif
			strcpy(buff, morse);
			uBit->display.print(MicroIMG_tick);
			uBit->sleep(2000);
		}
	}
}

void MorseBit_encryptMorse(char *morse, uint8_t *encrypt)
{
	for (size_t i = 0; i < sizeof(encrypt); i++)
		encrypt[i] = 0;

	translateMorseToIndex(morse, encrypt);

	for (size_t i = 1; encrypt[i] != 0; i++)
		encrypt[i] = (encrypt[i] / 2) - 1;
}

void MorseBit_decryptIndex(uint8_t *encrypt, char *buff)
{
	// assumes encrypt[0] contains number of indexes to decrypt
	for (int i = 1; i < encrypt[0]; i++)
		buff[i-1] = alphabet[(encrypt[i] + 1) * 2];
}

void MorseBit_tx(MicroBit *uBit, MicroBitPin *pin, char *buff)
{
}

