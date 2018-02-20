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
// defines
#define DOT_TIMELIMIT	500
#define DASH_TIMELIMIT	2000
#define DEAD_TIMELIMIT	3000
#define DISPLAY_TIME	30

//=========
// GLOBALS
//=========
// microbit
MicroBit uBit;
// variables
bool main_loop = true;
bool receiving = false;
int dead = 0;
unsigned long timestamp = 0;
unsigned long duration = 0;;

//============
// PROTOTYPES
//============
void button_listener();
void event_handler(MicroBitEvent evt)
{
	if(evt.source == MICROBIT_ID_BUTTON_B)
		receiving = true;
}
void register_listeners()
{
	uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY, event_handler, MESSAGE_BUS_LISTENER_IMMEDIATE);
}

//======
// MAIN
//======
int main()
{
//init
	uBit.init();
	register_listeners();
	uBit.display.scroll("RDY", 75);
//----
//loop
	create_fiber(button_listener);
	while(main_loop)
	{
		while(!receiving)
			uBit.sleep(1000);
		if (receiving)
		{
			//@TODO
			uBit.serial.printf("received");
		}
	}
//----
//exit
	uBit.display.scroll("BYE", 75);
	release_fiber();
//----
}

//===========
// FUNCTIONS
//===========
void button_listener()
{
	while (1)
	{
		timestamp = uBit.systemTime();
		while(uBit.buttonA.isPressed())
		{
			// wait for button to be released
		}
		duration = uBit.systemTime() - timestamp;

		if (duration > 100 && duration < DEAD_TIMELIMIT)
		{
			dead = -1;
			uBit.serial.printf("%u: ", duration);
			if (duration < DOT_TIMELIMIT)
			{
				uBit.serial.printf(".\r\n");
				uBit.display.scrollAsync(".", DISPLAY_TIME);
			}
			else if (duration >= DOT_TIMELIMIT && duration <= DASH_TIMELIMIT)
			{
				uBit.serial.printf("-\r\n");
				uBit.display.scrollAsync("-", DISPLAY_TIME);
			}
		}
		else if (dead == -1)
			dead = uBit.systemTime();

		if ((uBit.systemTime() - dead) == DEAD_TIMELIMIT)
		{
			dead = -1;
			uBit.serial.printf("dead\r\n");
			uBit.display.clear();
		}
	}
}

