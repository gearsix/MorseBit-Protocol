# Challenge 2 - MorseBit Protocol
This challenge implements a simple communication protocol that transmits data between 2 BBC Micro:bit devices, using one of the GPIO pins.

## Contents
1. [Overview](##Overview)
2. [Morse:ßit Protocol](##Morse:ßit Protocol)
3. [Instructions](##Instructions)
4. [Prerequisites](##Prerequisites)
5. [Authors](##Authors)
6. [Extras](##Extras)

## Overview

This is an implementation of a custom single-wire protocol designed for the BBC Micro:bits. We'd hope it could be adapted to any single-wire platform though.

In a basic overview of the protocol a TX user inputs data via button presses in Morse Code and the RX user recieves this data. In-between the TX and RX users, the data is converted from button presses into indexes on our morse code lookup table (see [Extras](##Extras)) and then encrypted using an XOR cipher. These indexes are then sent to the RX user in binary (uing HI/LO on wire), which the RX user recieves, decrypts and reads.

There are several intricacies in the timing that can be customised in MorseBit.h (basically things like timing for button presses).

Also check out the [Extras](##Extras) to see how this protocol reads Morse Code.


## Morse:ßit Protocol

The Morse:ßit Protocol is a simple, single-wire communication protocol that trasmits data between 2 devices. This implementation has done it using GPIO pins on BBC Micro:bit devices. It uses Morse Code and a Morse Code lookup table (see [Extras](##Extras)) to tx/rx data from user to user.

### MorseBit Messages
A MorseBit message is simply a series of MorseBit packets. The first packet in a MorseBit message will always be the number of packets in it's message and the XOR cipher key to decrypting the following packets.

### MorseBit Package
A MorseBit packet is a simple 6-bit integer. It's only 6-bits because the lookup table has 63 indexes and 111111 = 63. This could be expanded in the future.


### TX over MorseBit:
1. User inputs message via Morse Code presses (set GPIO pin HI whilst this is going on to signal incoming message to the RX user)
2. Morse code is then converted to the Morse Code character's corresponding indexes in the Morse Code lookup table [Extras](##Extras)
3. These indexes are then encrypted using an XOR cipher. They key to this cipher wil lbe the number of indexes being encrypted
4. When the RX user is ready, the TX user sends the number of characters to be recieved followed by the indexes over the GPIO pin
	1. Convert the integer to be sent into binary
	2. Set the GPIO pin HI/LO for _X_ ms (HI for 1, LO for 0) **(Note: send MSB first)**
	3. Then sleep for _X_ ms
	4. Repeat until all packets have been sent

### RX over MorseBit:
1. Wait for an incoming message signal to be detected
2. Listen for incoming MorseBit packets:
	1. Read the GPIO pin for _X_ ms
	2. If the GPIO pin is HI push 1 onto the packet integer, else push 0 **(Note: recieve MSB first)**
	3. After 6 bits, convert to an integer
3. The first packet recieved should always be the number of packets to follow. This will also be used as the key for the XOR cipher if the packet is encrypted
4. Decrypt the packet using an XOR cipher. The key sohuld be the first packet recieved (message size)
5. You should now have indexes to the Morse Code lookup table [Extras](##Extras) to convert to characters


### Protocol Finite State Machine
![](images/morseState.png)


## Instructions

When micro:bit is flashed, **RDY** will scroll on the LED to indicate 'Ready'. It will stay in **IDLE**, waiting for input (either on pin or button).
**To enter TX mode**
* *HOLD* Button A until a spinning animation plays (away from button A).
* Then release and **TX** will scroll across the display.
* After **TX** has scrolled off the screen, start inputting Morse Code using Button A.
* When finished, a **TICK** symbol will be shown.

**To enter RX mode**
* wait in **IDLE** until an `incoming message` signal is recieved on the GPIO pin
* When this happens a spinning animation will play (towards button A)
* Then **RX** with scroll across the screen and the device will start recieving data.
* Then a **TICK** will show to note that it's finished recieving data
* Then the message will scroll across the screen following by a small animation to show it's finished.

Do | Button/Action | Timing
---|---| ---|
**DOT** | **PRESS** Button A | 500 _ms_
**DASH** | **HOLD** Button A | 1500 _ms_
**SPACE** | wait | 2000 _ms_
**END** | wait | 3500 _ms_


## Prerequisites
**These instructions assume you're using yotta**
Set `yotta` target before building application. Then copy executable to micro:bit. [Learn more](https://lancaster-university.github.io/microbit-docs/offline-toolchains "Yotta Offline Build System")

```
yt target bbc-microbit-classic-gcc
yt build
```
Executable .hex file in directory:
```
build/bbc-microbit-classic-gcc/source/
```

### Installation
At _MorseBit_ directory, simply build application and flash executable to micro:bit.

```
cp build/bbc-microbit-classic-gcc/source/morsebit-combined.hex /media/[username]/MICROBIT

```


## Authors
- **Alexander Collins** - [14029202]
- **WeiJun Chai** - [16031162]


## Extras
### Morse Code Chart
** Note: PROSIGNS require spaces inbetween their letters (e.g. SOS goes from ...---... to .../---/...)**

![](images/morseTable.png)

### Morse Code Lookup Table
The charts below shows the lookup table and tree structure of morse code. Timing of the dots and dashes are listed under Instructions section.

![](images/morseTree.png)

| INDEX	| VALUE	|	MORSE CODE	|
| ---|---| ---|
|	0	|		||
|	1	|		||
|	2	|	E	|.-|
|	3	|	T	|-..|
|	4 | I |..|
| 5 | A |.-|
|	6 | N |-.|
| 7 | M |--|
|	8 | S |...|
| 9 | U |..-|
|	10 | R |.-.|
| 11 | W |.--|
| 12 | D |-..|
| 13 | K |-.-|
|	14 | G |--.|
| 15 | O |---|
|	16 | H |....|
| 17 | V |...-|
|	18 | F |..-.|
| 19 |   ||
|	20 | L |.-..|
| 21 |   ||
|	22 | P |.--.|
| 23 | J |.---|
|	24 | B |-...|
| 25 | X |-..-|
|	26 | C |-.-.|
| 27 | Y |-.--|
|	28 | Z |--..|
| 29 | Q |--.-|
| 30 |   ||
| 31 |   ||
| 32 | 5 |.....|
| 33 | 4 |....-|
| 34 |   ||
| 35 | 3 |...--|
| 36 |   ||
| 37 |   ||
| 38 |   ||
| 39 | 2 |..---|
| 40 |   ||
| 41 |   ||
| 42 | + |.-.-.|
| 43 |   ||
| 44 |   ||
| 45 |   ||
| 46 |   ||
| 47 | 1 |.----|
| 48 | 6 |-....|
| 49 | = |-...-|
| 50 | / |-..-.|
| 51 |   ||
| 52 |   ||
| 53 |   ||
| 54 |   ||
| 55 |   ||
| 56 | 7 |--...|
| 57 |   ||
| 58 |   ||
| 59 |   ||
| 60 | 8 |---..|
| 61 |   ||
| 62 | 9 |----.|
| 63 | 0 |-----|
