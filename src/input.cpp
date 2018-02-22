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
unsigned long duration = 0;

char m[100];
int index = 0;

//============
// MORSE
//============
char alphabet[50] = {
	0,		//
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

	'O',  //15
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
const char *alpha[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z
};

//============
// PROTOTYPES
//============
void compare();
uint8_t temp(const char *str);
void button_listener();
//======
// MAIN
//======
int main()
{
//init
	uBit.init();
	uBit.display.scroll("RDY", 75);
//----
//loop
	// create_fiber(button_listener);
	// while(main_loop)
	// {
	// 	while(!receiving)
	// 		uBit.sleep(1000);
	// 	if (receiving)
	// 	{
	// 		//@TODO
	// 		uBit.serial.printf("received");
	// 	}
	// }
	while (1)
		{
			timestamp = uBit.systemTime();
			while(uBit.buttonA.isPressed())
			{
				// wait for button to be released
			}
			duration = uBit.systemTime() - timestamp;
			//uBit.serial.printf("%u\r\n", duration);

			if (duration > 100 && duration < DEAD_TIMELIMIT)
			{
				dead = -1;
				uBit.serial.printf("%u: ", duration);


				if (duration < DOT_TIMELIMIT)
				{
					uBit.serial.printf(".\r\n");
					uBit.display.scrollAsync(".", DISPLAY_TIME);
					m[index]='.';
					index++;
				}
				else if (duration >= DOT_TIMELIMIT && duration <= DASH_TIMELIMIT)
				{
					uBit.serial.printf("-\r\n");
					uBit.display.scrollAsync("-", DISPLAY_TIME);
					m[index]='-';
					index++;
				}
			}
			else if (dead == -1)
				dead = uBit.systemTime();

			if ((uBit.systemTime() - dead) == DEAD_TIMELIMIT)
			{
				dead = -1;
				// uBit.serial.printf("dead\r\n");
				uBit.serial.printf("buffer: %s\n\r", m);
				//compare();
				uBit.serial.printf("ALPHA: %c\r\n", alphabet[temp(m)]);
				for (int i = 0; i < index; i++) {
					m[i] = NULL;
				}
				index = 0;
				uBit.display.clear();
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

void compare(){
	for (int i = 0; i < sizeof(alpha); i++) {
		if (m == alpha[i]) {
			uBit.serial.printf("ALPHA: %s\n\r", alpha[i]);
		}
	}
}

uint8_t temp(const char *str)
{
	int i = 0;
	uint8_t bit = 1;
	while(i < sizeof(str))
	{
		if (str[i] == '-')
			bit = (bit << 1) | 1;	//bitshift 1
		if (str[i] == '.')
			bit = bit << 1;			//bitshift 0
		else
			uBit.serial.printf("err\r\n");
		i++;
	}
	return bit;
}
