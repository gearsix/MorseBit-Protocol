/*------------------------------------------
|	Proj: Challenge1-UFCFVK-15-2 
|	File: main.cpp
|	Auth: Alexander Collins
|	Desc: Implement a simple game on the BBC Microbit that uses the LED matrix and at least two \
|			buttons or other sensors on the device.
|		  Game that queues several actions and awards the user points for acting them out within \
|			a certain time period (depending upon difficulty)
|	Date: Jan-Feb, 2018
|-----------------------------------------*/
//========
// MACROS
//========
// includes
#include "MicroBit.h"
#include "img.h"
#include "morsebit.h"
// defines
#define MODE_IDLE	0
#define	MODE_RX		1
#define	MODE_TX		2
#define LO	0
#define HI	1

//=========
// GLOBALS
//=========
// microbit
MicroBit uBit;
MicroBitPin		ioPin(MICROBIT_ID_IO_P2, MICROBIT_PIN_P2, PIN_CAPABILITY_DIGITAL);	// r/w pin
MicroBitButton	inBtn(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);					// input button
// variables
int mode;
char buffer[MORSEBIT_MAX_BUFFER_SIZE];
bool main_loop;

//============
// PROTOTYPES
//============
bool check_pin(MicroBit *uBit);
bool check_btn(MicroBit *uBit);

//======
// MAIN
//======
int main()
{
//init
	uBit.init();
	main_loop = true;
	mode = MODE_IDLE;
	uBit.display.scroll("RDY", 75);
	uBit.serial.printf("\r\n---RDY---\r\n");
//----
//loop
	while(main_loop)
	{
		uBit.serial.printf("MODE: %i\r\n", mode);
		switch(mode)
		{
			case MODE_RX:
				uBit.display.scrollAsync("RX", 75);
				mode = MODE_IDLE;
				break;
			case MODE_TX:
				uBit.display.scrollAsync("TX", 75);
				MORSEBIT_get_morsecode(&uBit, &inBtn, buffer);
				//
				break;
			default:	// IDLE
				uBit.display.scrollAsync("IDLE", 75);
				if (check_pin(&uBit))
					mode = MODE_RX;
				else if (check_btn(&uBit))
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

//===========
// FUNCTIONS
//===========
bool check_pin(MicroBit *uBit)
{
	unsigned long timestamp = uBit->systemTime();
	while (ioPin.getDigitalValue() == HI)
	{
		// wait for pin to go LO
		IMG_animation_rotation(uBit, 1, 50, true);
	}
	unsigned long duration = uBit->systemTime() - timestamp;

	uBit->serial.printf("check_pin(): %u\r\n", duration);

	if (duration >= (MORSEBIT_PIN_MSG_INCOMING - MORSEBIT_OFFSET) && duration <= (MORSEBIT_PIN_MSG_INCOMING + MORSEBIT_OFFSET))
		return true;
	else
		return false;
}

bool check_btn(MicroBit *uBit)
{
	bool triggered = false;
	while (inBtn.isPressed())
	{
		triggered = true;
		// display animation so user knows to release button
		IMG_animation_rotation(uBit, 1, 50, false);
	}

	uBit->serial.printf("check_btn(): %i\r\n", triggered);

	if (triggered)
		return true;
	else
		return false;
}

