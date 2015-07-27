//See COPYRIGHT.txt for info on copyright
/*terminal.c  --the terminal application*/
#include <mytypes.h>
#include <event_drive.h>
#include <video.h>
#include <stdio.h>


volatile BOOL terminal_end=FALSE;

void TerminalKeyboard(unsigned char trash,unsigned int key,void *trash2);
void TerminalComRead(unsigned char trash,unsigned int dat,void *trash2);

void _Terminal(unsigned char *argp){
	void *preserve[2];

     printf("JouleTerminal 0.6.2");
     preserve[0]=SetEvent(KEYBOARD_EVENT,TerminalKeyboard);
     preserve[1]=SetEvent(COM1_READ,TerminalComRead); //save and set the event handlers
     printf(" .........Ready!\n\n");
     while(1){
     //do nothing!
     if(terminal_end==1){
     	DisableEvents(); //do this quickly so we have time to uninstall all of the events
     	printf("\nThis session has been terminated!\n\n");
     	break;
     }
     }
     SetEvent(KEYBOARD_EVENT,preserve[0]);
     SetEvent(COM1_READ,preserve[1]); //restore event handlers
     EnableEvents();
     terminal_end=FALSE; //for future
     return;
}




unsigned char tmp_buffer_com[2]; //just stores a null terminated character
void TerminalComRead(unsigned char trash,unsigned int dat,void *trash2){ //event-based system, this is called when data is available in comport
	//This is called when a byte is sent to us
	tmp_buffer_com[1]=0;
	if(dat=='\r'){dat='\n';}
	tmp_buffer_com[0]=GetByteCom1();
	printf(tmp_buffer_com);
	//if(dat=27){
	//	terminal_end=1;
	//
}


void TerminalKeyboard(unsigned char trash,unsigned int key,void *trash2){ //event-based system, this is called when anyone types something at the keyboard
	key=key&0xFF; //just get the ascii code
	switch(key){
		case '\n':
          SendByteCom1('\r'); //add that since its the ascii standard
          //SendByteCom1('\n');
          break;
          case 27: //escape key
          terminal_end=1;
          SendByteCom1(27);
          break;
          default:
          SendByteCom1(key);
          break;
	}
	tmp_buffer_com[1]=0;
	tmp_buffer_com[0]=key;
	printf(tmp_buffer_com); //printf is the only thing up-to-date with some escape chars so..
}









