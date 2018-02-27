/*-------------------------------------------------------------------------------------------
|	Project:		Challenge2-UFCFVK-15-2
|	File:			main.cpp
|	Authors:		Alexander Collins, Wei Jun
|	Description:	Implement a simple communication protocol that transmits data between 2 \
|					 BBC Micro:bit devices, using GPIOs.
|	Notes:			Hard-tabs (4 spaces)
|	Date:			Feb, 2018
-------------------------------------------------------------------------------------------*/
//========
// MACROS
//========
// includes
#include "MicroBit.h"
#include "img.h"		//IMG_*
#include "MorseBit.h"	//MorseBit_*
// defines
#define MODE_IDLE	0
#define MODE_RX		1
#define MODE_TX		2

#define LO	0
#define HI	1

//=========
// GLOBALS
//=========
// microbit
MicroBit		uBit;
MicroBitPin		ioPin(MICROBIT_ID_IO_P2, MICROBIT_PIN_P2, PIN_CAPABILITY_DIGITAL);	// r/w pin
MicroBitButton	inBtn(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);					// input button
// variables
int mode;
bool main_loop;
char buffer[MorseBit_MAX_BUFFER];

//============
// PROTOTYPES
//============
bool check_pin(MicroBit *uBit, MicroBitPin *pin);
bool check_button(MicroBit *uBit, MicroBitButton *button);

//======
// MAIN
//======
int main()
{
//init
	uBit.init();
	mode = MODE_IDLE;
	main_loop = true;
	uBit.display.scrollAsync("RDY", 75);
	uBit.serial.printf("\r\n---RDY---\r\n");
//----
//main
	while (main_loop)
	{
		uBit.serial.printf("MODE: %i\r\n", mode);
		switch(mode)
		{
			case MODE_RX:
				uBit.display.scroll("RX", 75);
				mode = MODE_IDLE;
				break;
			case MODE_TX:
				uBit.display.scroll("TX", 75);
				MorseBit_getMorseCode(&uBit, &inBtn, buffer);
				uBit.serial.printf("buffer: %s\r\n", buffer);
				mode = MODE_IDLE;
				break;
			default:	//MODE_IDLE
				if (check_pin(&uBit, &ioPin))
					mode = MODE_RX;
				else if (check_button(&uBit, &inBtn))
					mode = MODE_TX;
				break;
		}
		uBit.sleep(1000);
	}
//----
//exit
	uBit.display.scroll("BYE", 75);
	uBit.serial.printf("\r\n---BYE---\r\n");
	release_fiber();
//----
}

bool check_pin(MicroBit *uBit, MicroBitPin *pin)
{
	// get current time
	unsigned long timestamp = uBit->systemTime();
	// wait for pin to go LO
	while(pin->getDigitalValue() == HI)
	{
		// play animation to let the user know partner is about to send data
		IMG_animation_rotation(uBit, 1, 75, true);
	}
	// count how long pin was HI for
	unsigned long duration = uBit->systemTime() - timestamp;

	uBit->serial.printf("check_pin(): %u\r\n", duration);

	// if duration is HI long enough to signal incoming message
	if (duration >= (MorseBit_PIN_INCOMING_MSG - MorseBit_OFFSET) && duration <= (MorseBit_PIN_INCOMING_MSG + MorseBit_OFFSET))
		return true;
	else
		return false;
}

bool check_button(MicroBit *uBit, MicroBitButton *button)
{
	bool triggered = false;
	while (button->isPressed())
	{
		triggered = true;
		// display animation so the user knows function has triggered
		IMG_animation_rotation(uBit, 1, 75, false);
	}

	uBit->serial.printf("check_button(): %i\r\n", triggered);

	return triggered;
}

