/*------------------------------------------------------------------------------------------\
|	Project:		Challenge2-UFCFVK-15-2
|	File:			main.cpp
|	Authors:		Alexander Collins, Wei Jun
|	Description:	Implement a simple communication protocol that transmits data between 2 \
|					 BBC Micro:bit devices, using GPIOs.
|	Notes:			Hard-tabs (4 spaces)
|	Date:			Feb, 2018
\------------------------------------------------------------------------------------------*/
//========
// MACROS
//========
// includes
#include "MicroBit.h"
#include "MicroIMG.h"	//MicroIMG_*
#include "MorseBit.h"	//MorseBit_*
// defines
#define MODE_IDLE	0
#define MODE_RX		1
#define MODE_TX		2

#define LO	0
#define HI	1

#define DEBUG 1

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
char buffer[MorseBit_MAX_BUFF];

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
	uBit.display.scroll("RDY", 75);
#if DEBUG
	uBit.serial.printf("\r\n---RDY---\r\n");
#endif
//----
//main
	while (main_loop)
	{
	#if DEBUG
		uBit.serial.printf("MODE: %i\r\n", mode);
	#endif
		switch(mode)
		{
			case MODE_RX:
				uBit.display.scrollAsync("RX", 75);
//				MorseBit_rx(&uBit, &ioPin, buffer);
				uBit.display.scroll(buffer);
				mode = MODE_IDLE;
				break;
			case MODE_TX:
				uBit.display.scroll("TX", 75);
				ioPin.setDigitalValue(HI);	// signal incoming message
				MorseBit_getMorseCode(&uBit, &inBtn, buffer);
				ioPin.setDigitalValue(LO);
//				MorseBit_tx(&uBit, &ioPin, buffer);
				mode = MODE_IDLE;
				break;
			default:	//MODE_IDLE
				uBit.display.scrollAsync("IDLE", 75);
				if (check_pin(&uBit, &ioPin))
					mode = MODE_RX;
				else if (check_button(&uBit, &inBtn))
					mode = MODE_TX;
				break;
		}
	}
//----
//exit
	uBit.display.scroll("BYE", 75);
#if DEBUG
	uBit.serial.printf("\r\n---BYE---\r\n");
#endif
	release_fiber();
//----
}

/*
	Checks if pin is HI long enough to signal an incoming message

	@param MicroBit *uBit - MicroBit used to get systemTime() for timestamps
	@param MicroBitPin *pin - MicroBitPin used to check for incoming message signal

	@return true - incoming message signal detected on pin
	@return false - no incoming message signal detected on pin
*/
bool check_pin(MicroBit *uBit, MicroBitPin *pin)
{
	// get current time
	unsigned long timestamp = uBit->systemTime();
	// wait for pin to go LO
	while(pin->getDigitalValue() == HI)
	{
		// play animation to let the user know partner is about to send data
		MicroIMG_animation_rotation(uBit, 1, 75, true);
	}
	// count how long pin was HI for
	unsigned long duration = uBit->systemTime() - timestamp;

#if DEBUG
	uBit->serial.printf("check_pin(): %u\r\n", duration);
#endif

	// return if duration is HI long enough to signal incoming message
	if (duration >= MorseBit_INCOMING - MorseBit_OFFSET)
		return true;
	else
		return false;
}

/*
	Check if button is held

	@param MicroBit *uBit - MicroBit used to get systemTime() for timestamps
	@param MicroBitButton *button - MicroBitButton used to check for hold

	@return true - button was held when checked
	@return false - button was not held when checked
*/
bool check_button(MicroBit *uBit, MicroBitButton *button)
{
	bool triggered = false;
	while (button->isPressed())
	{
		triggered = true;
		// display animation so the user knows function has triggered
		MicroIMG_animation_rotation(uBit, 1, 75, false);
	}

#if DEBUG
	uBit->serial.printf("check_button(): %i\r\n", triggered);
#endif

	return triggered;
}

