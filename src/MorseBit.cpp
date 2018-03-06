/*-------------------------------------------------------------------------------------------
|	Project:		Challenge2-UFCFVK-15-2
|	File:			MorseBit.cpp
|	Authors:		Alexander Collins, Wei Jun
|	Description:	Implementaiton for the MorseBit protocol
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

#define DEBUG 1

//=========
// GLOBALS
//=========
static char alphabet[64] = {
	0,		//0
	0,		//1
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
	'V',	//17
	'F',	//18
	0,		//19
	'L',	//20
	0,		//21
	'P',	//22
	'J',	//23
	'B',	//24
	'X',	//25
	'C',	//26
	'Y',	//27
	'Z',	//28
	'Q',	//29
	0,		//30
	0,		//31
	'5',	//32
	'4',	//33
	0,		//34
	'3',	//35
	0,		//36
	0,		//37
	0,		//38
	'2',	//39
	0,		//40
	0,		//41
	'+',	//42
	0,		//43
	0,		//44
	0,		//45
	0,		//46
	'1',	//47
	'6',	//48
	'=',	//49
	'/',	//50
	0,		//51
	0,		//52
	0,		//53
	0,		//54
	0,		//55
	'7',	//56
	0,		//57
	0,		//58
	0,		//59
	'8',	//60
	0,		//61
	'9',	//62
	'0'		//63
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
				m = strlen(morse)+1;// end loop
				index[i++] = bit;	// push bit
				bit = 1;			// reset bit
				break;
			case MorseBit_LETTERSPACE:
				index[i] = bit;	// push bit
				i++;
				bit = 1;			// reset bit
				break;
		}
	}

	index[0] = i;	// index[0] = number of letters
}

static void send(uint8_t value, MicroBit *uBit, MicroBitPin *pin)
{
	uint8_t bit;
	unsigned long timestamp;

#if DEBUG
	uBit->serial.printf("%u = ", value);
#endif

	for (int i = MorseBit_PACKET_SIZE-1; i >= 0; i--)
	{
		bit = value >> i;
		timestamp = uBit->systemTime();
		while ((uBit->systemTime() - timestamp) <= MorseBit_BIT_TIME)
			pin->setDigitalValue(bit & 1);

		#if DEBUG
			uBit->serial.printf("%u", bit & 1);
		#endif

		// wait for sync
		uBit->sleep(MorseBit_BIT_TIME);
	}

#if DEBUG
	uBit->serial.printf("\r\n");
#endif
}

static uint8_t recieve(MicroBit *uBit, MicroBitPin *pin)
{
	uint8_t buffer = 0;
	uint8_t packet[MorseBit_PACKET_SIZE];
	unsigned long timestamp;

// recieve bits
	for (int i = MorseBit_PACKET_SIZE-1; i >= 0; i--)
	{
		// recv bit
		timestamp = uBit->systemTime();
		while((uBit->systemTime() - timestamp) <= MorseBit_BIT_TIME)
			packet[i] = pin->getDigitalValue();

	#if DEBUG
		uBit->serial.printf("%u", packet[i]);
	#endif

		uBit->sleep(MorseBit_BIT_TIME);
	}

// convert bits to integers
	for (int position = 0; position < MorseBit_PACKET_SIZE; ++position)
		buffer += packet[position] * pow(2, position);

#if DEBUG
	uBit->serial.printf(" = %u\r\n", buffer);
#endif

	return buffer;
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
		if (duration > MorseBit_OFFSET && (uBit->systemTime() - dead) < MorseBit_TERMINATOR_TIME)
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
		else if ((uBit->systemTime() - dead) >= MorseBit_TERMINATOR_TIME)
		{
			// terminate message
			morse[m] = MorseBit_TERMINATOR;
		#if DEBUG
			uBit->serial.printf("MorseBit_getMorseCode(): %s\r\n", morse);
		#endif

//@TODO have function to check if morse = valid here & print cross if not
			strcpy(buff, morse);

			uBit->display.print(MicroIMG_tick);
			uBit->sleep(2000);
			uBit->display.print(MicroIMG_blank);

			finished = true;
		}
	}
}

void MorseBit_encrypt(char *morse, uint8_t *encrypt)
{
	// get indexes (encrypt[0] = size)
	translateMorseToIndex(morse, encrypt);

	// encrypt from 0 onwards (0 = size & key)
	for (size_t i = 1; i < encrypt[0]/*encrypt[i] != 0*/; i++)
		encrypt[i] = (encrypt[i] ^ encrypt[0]);
}

void MorseBit_decrypt(uint8_t *encrypt, char *buff)
{
	// assumes encrypt[0] == size & key
	for (size_t i = 1; i < encrypt[0]; i++)
		buff[i-1] = alphabet[(encrypt[i] ^ encrypt[0])];
}

void MorseBit_tx(MicroBit *uBit, MicroBitPin *pin, uint8_t *buff)
{
	// tx all data
	for (uint8_t i = 0; i < buff[0]; i++)
	{
		send(buff[i], uBit, pin);
		uBit->sleep(MorseBit_PACKET_TIME);
	}
}

void MorseBit_rx(MicroBit *uBit, MicroBitPin *pin, uint8_t *buff)
{
	// recieve first packet (size & cipher key)
	buff[0] = recieve(uBit, pin);
	uBit->sleep(MorseBit_PACKET_TIME);
	// recieve rest of data
	for (int i = 1; i < buff[0]; i++)
	{
		buff[i] = recieve(uBit, pin);
		uBit->sleep(MorseBit_PACKET_TIME);
	}

	// signal to user RX is done
	uBit->display.print(MicroIMG_tick);
	uBit->sleep(1000);
	uBit->display.clear();
}

