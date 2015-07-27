//See COPYRIGHT.txt for info on copyright
/******************************
This is the Com port interface
*****************************/
#include <com_ports.h>
#include <k_JouleOS.h>
#include <mytypes.h>
#include <stdio.h>
#include <video.h>
#include <debug.h>
#include <event_drive.h>



volatile ComEvent ReadFunc=0;
volatile unsigned char Com1Error; //this is basically a copy of the error stuff, its a bitmap
volatile unsigned char Com1Buffer[32];
volatile unsigned char Com1Count=0;
volatile BOOL IgnoreReads=TRUE; //ignores reads(lets the pc store it in its internal buffer)
volatile BOOL NoWriting=TRUE; //doesn't write anything
volatile BOOL ReadyWrite=TRUE; //ready to write, only set when IgnoreWrites is set or Com1WriteCounter==Com1WriteLength
volatile BOOL ReadyRead=TRUE; //ready to read, only set when IgnoreReads is set or Com1ReadCounter==Com1ReadLength
volatile unsigned char Com1IgnoredReadBuffer; //done just to avoid errors
volatile unsigned char *Com1ReadBuffer;
volatile unsigned int Com1ReadCounter=0;
volatile unsigned int Com1ReadLength=0;
volatile unsigned char *Com1WriteBuffer;
volatile unsigned int Com1WriteCounter=0; //Where we are at in Com1WriteBuffer
volatile unsigned int Com1WriteLength=0; //length of Com1WriteBuffer
void Init_ComPort1(BOOL on_boot){ //on_boot is 1 if it is called on boot so that it prints the messages
	//ComBuffer=malloc(32);
	//Com1Counter=0;
	//set to address baud
	outportb(COM1_LINE_CONTROL,  LINE_ADDRESS_BAUD);
     //send baud rate
	outportb( COM1_TRANSMIT_RECEIVE_BUFFER,0x0C);
	outportb(COM1_INTERRUPT_ENABLE,0);
	//set the comport up
     outportb(COM1_LINE_CONTROL, LINE_8BIT_1STOP );
     outportb(COM1_INTERRUPT_ENABLE, INT_RECEIVED_DATA|INT_TRANSMITTER_EMPTY|INT_RECEIVER_STATUS);
	outportb(COM1_MODEM_CONTROL,INTERRUPTS_ENABLE);
	//reset the serial port
	inportb(COM1_INTERRUPT_ID);
	inportb(COM1_LINE_STATUS);
	inportb(COM1_MODEM_CONTROL);
	if(on_boot){
	printf("COM1 Hardware:");
     ProcessConsole->curx=60;ProcessConsole->color=GREEN;printf("[OK]");
     ProcessConsole->cury++;ProcessConsole->curx=0;ProcessConsole->color=0x0F;
	}
	wait(500);
	if(on_boot){
          printf("COM1 Line:");
		if(Com1Error==0){
               ProcessConsole->curx=60;ProcessConsole->color=GREEN;printf("[OK]");
               ProcessConsole->cury++;ProcessConsole->curx=0;ProcessConsole->color=0x0F;
		}else{
               ProcessConsole->curx=58;ProcessConsole->color=RED;
               printf("[FAILED]\n");
               ProcessConsole->cury++;ProcessConsole->curx=0;ProcessConsole->color=0x0F;
		}
	}

}

unsigned char SendByteCom1(unsigned char dat){
	//sending without the automatic interrupt
	unsigned int delay;
	delay=timer_ticks+2000; //wait 2s
	while(delay>timer_ticks){
		if(ReadyWrite==TRUE){
			outportb(COM1_TRANSMIT_RECEIVE_BUFFER,dat);
			return 0;
		}
	}
	return 1;
}

unsigned char GetByteCom1(){
	//unsigned int delay;
	unsigned char dat;
	//delay=timer_ticks+2000;
	while(1){ //no timeout for this one....
		if(ReadyRead=TRUE){
			dat=Com1Buffer[Com1Count];
			if(Com1Count==0){
				ReadyRead=0;
			}else{
			Com1Count--;
			}
			return dat;
		}
	}
	return 0;
}




//yea this is a mess with commented out commands
void ComPort1_Handler(){ //This is the interrupt handler
	unsigned char which,tmp;
	unsigned char tmpy[2];
	which=inportb(COM1_INTERRUPT_ID);
	tmp=which&0x06; //isolate the type of interrupt
	switch(tmp){
		case ID_MODEM_STATUS: //ignored
		//do nothing
		break;
		case  ID_TRANSMITTER_EMPTY: //ready to write
			ReadyWrite=TRUE; //sets the status to ready for writing
		break;
		case ID_RECEIVED_DATA: //theirs data in the port so go get it!
		Com1Count++;
		if(Com1Count==32){Com1Count=0;}
		ReadyRead=TRUE;
		Com1Buffer[Com1Count]=inportb(COM1_TRANSMIT_RECEIVE_BUFFER);
          CallEvent(COM1_READ,0,0);
		if(IgnoreReads || Com1ReadLength<=Com1ReadCounter){
			//ReadyRead=TRUE;
		}
		break;
		case ID_RECEIVER_STATUS: //ERROR!!
		which=inportb(COM1_LINE_STATUS);
		which=which&30; //isolates bits 1-4
		switch(which){
			case OVERRUN_ERROR: //buffer overrun
			Com1IgnoredReadBuffer=inportb(COM1_TRANSMIT_RECEIVE_BUFFER);
			Com1Error=Com1Error|OVERRUN_ERROR;
			break;
			case PARITY_ERROR:
			Com1Error=Com1Error|PARITY_ERROR;
			//do nothing I guess
			break;
			case FRAMING_ERROR:
			Com1Error=Com1Error|FRAMING_ERROR;
			//do nothing again
			break;
			case CONNECT_LOST:
			Com1Error=Com1Error|CONNECT_LOST;
			break;
		}
		CallEvent(COM1_ERROR,Com1Error,0);
		break;
	}

}





