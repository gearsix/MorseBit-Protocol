# Challenge 2 - MorseBit
This challenge implements a simple communication protocol that transmits data between 2 BBC Micro:bit devices, using GPIOs.

## Prerequisites
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

## Morse:ßit Protocol
A simple communication protocol that transmits data between 2 BBC Micro:bit devices using GPIOs, it is implemented to transmit/receive and decode Morse code. There is only a kind of data being transmitted to the receiver. The morse code is translated by the transmitter to the character and allocated into its buffer, then, encrypts and sends the data by dividing each index on its buffer by 2 then deduct result by 1. The receiver will decrypt each index of the buffer by reversing the mathematical order and gets the character from the lookup table with its index. However, there is a limitation on the size of the buffer, it can only take up to 32 characters because it uses 6 bits packages to transmit the data, as Binary 111 111 is 63 in Decimal, and it has 63 indexes in the morse code lookup table.

## Finite State Machine
![](images/morseState.png)


## Morse Code
The charts below shows the lookup table and tree structure of morse code. Timing of the dots and dashes are listed under Instructions section.

![](images/morseTable.png)

![](images/morseTree.png)

## Instructions
Do | Button/Action | Timing
---|---| ---|
**•** | **PRESS** Button A | 500 _ms_
**-** | **HOLD** Button A | 1500 _ms_
**Space** | wait | 2000 _ms_
**End** | wait | 4000 _ms_
Reset | **Button Reset** behind micro:bit |





### Author
- **Alexander Collins** - [14029202]
- **WeiJun Chai** - [16031162]
