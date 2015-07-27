//See COPYRIGHT.txt for info on copyright
#ifndef COM_PORTS_H
#define COM_PORTS_H
#include <mytypes.h>

//ports
#define COM1_TRANSMIT_RECEIVE_BUFFER 0x3F8 //also is baud rate divisor LSB
#define COM1_INTERRUPT_ENABLE 0x3F9 //also is baud rate divisor MSB
#define COM1_INTERRUPT_ID 0x3FA
#define COM1_LINE_CONTROL 0x3FB
#define COM1_MODEM_CONTROL 0x3FC
#define COM1_LINE_STATUS 0x3FD
#define COM1_MODEM_STATUS 0x3FE
#define COM1_SCRATCH_PAD 0x3FF






//interrupt enable register macros
#define INT_RECEIVED_DATA 1 //ready to read
#define INT_TRANSMITTER_EMPTY 2 //ready to send
#define INT_RECEIVER_STATUS 4 //error occured
#define INT_MODEM_STATUS 8  //just a change, useless

//interrupt identification
#define ID_MODEM_STATUS 0 //means it has changed, read xxC
#define ID_TRANSMITTER_EMPTY 2 //ready to write
#define ID_RECEIVED_DATA 4 //a byte is in the buffer(ready to read)
#define ID_RECEIVER_STATUS 6 //an error occured, read xxD(line status)
#define NO_INTERRUPTS_PENDING 1

//Line Control
#define LINE_5BIT_1STOP 0
#define LINE_6BIT_1STOP 1
#define LINE_7BIT_1STOP 2
#define LINE_8BIT_1STOP 3
#define LINE_5BIT_1_5STOP 4
#define LINE_6BIT_2STOP 5
#define LINE_7BIT_2STOP 6
#define LINE_8BIT_2STOP 7

#define LINE_PARITY_ON 8
#define LINE_ODD_PARITY 0
#define LINE_EVEN_PARITY 16
#define LINE_ALWAYS0_PARITY 32
#define LINE_ALWAYS1_PARITY 48

#define LINE_BREAK_ENABLED 64
#define LINE_ADDRESS_BAUD 128


//Modem Control

#define ACTIVATE_DTR 1
#define ACTIVATE_RTS 2
#define INTERRUPTS_ENABLE 8
#define LOOPBACK_MODE 16



//9600 is 0x3F9:0 0x3F8:0x0C


//line status
//errors
#define OVERRUN_ERROR 2
#define PARITY_ERROR 4
#define FRAMING_ERROR 8
#define CONNECT_LOST 16 //actually referred to break interrupt



//global vars
typedef  unsigned char(*ComEvent)(unsigned char);

extern volatile ComEvent ReadFunc;
extern volatile unsigned char Com1Error; //this is basically a copy of the error stuff, its a bitmap

extern volatile BOOL IgnoreReads; //ignores reads(lets the pc store it in its internal buffer)
extern volatile BOOL NoWriting; //doesn't write anything
extern volatile BOOL ReadyWrite; //ready to write, only set when IgnoreWrites is set or Com1WriteCounter==Com1WriteLength
extern volatile BOOL ReadyRead; //ready to read, only set when IgnoreReads is set or Com1ReadCounter==Com1ReadLength
extern volatile unsigned char Com1IgnoredReadBuffer; //done just to avoid errors
extern volatile unsigned char *Com1ReadBuffer;
extern volatile unsigned int Com1ReadCounter;
extern volatile unsigned int Com1ReadLength;
extern volatile unsigned char *Com1WriteBuffer;
extern volatile unsigned int Com1WriteCounter; //Where we are at in Com1WriteBuffer
extern volatile unsigned int Com1WriteLength; //length of Com1WriteBuffer

//functions
void Init_ComPort1(BOOL on_boot);
unsigned char SendByteCom1(unsigned char dat);
void ComPort1_Handler();

unsigned char GetByteCom1();

















#endif

