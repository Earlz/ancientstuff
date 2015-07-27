//See COPYRIGHT.txt for info on copyright
/* TODO (Jordan#3#): Get Everything Event-Driven including the console */
#include <mytypes.h>
#include <event_drive.h>
#include <mouse.h>


volatile EventFunc Events[256]; //just 1k so..

volatile BOOL EventsEnabled=TRUE;
volatile EventFunc EventStack[32]; //This an overrun stack incase an event happens while an event is running
unsigned char EventStackCounter=0;
volatile BOOL event_happening=1;


//returns the value that was in it
void *SetEvent(unsigned char number,void *func){
	void *rety;
	rety=Events[number];
	Events[number]=func;
	return rety;
}
//gets the ptr to the event of number
void *GetEvent(unsigned char number){
     return Events[number];
}


/*Event 0 is a default event and is called if you call a 0'ed event*/
volatile BOOL runninge;
void CallEvent(unsigned char number,unsigned int dat,void *ptr){
	if(EventsEnabled){
	if(event_happening==TRUE){
		runninge=1;
	}
	//if(event_happening==FALSE){
     event_happening=TRUE;
	if(Events[number]!=0){
     Events[number](number,dat,ptr);
	}else{if(Events[0]!=0){
		Events[0](number,dat,ptr);}
	}
	//if(runninge!=1){
		event_happening=FALSE; //reset it on return unless another
	}
}

void DisableEvents(){
	EventsEnabled=FALSE;
}
void EnableEvents(){
	EventsEnabled=TRUE;
}


void InitEventDrive(){ //starts EventDrive with default values; this should be called when a new program runs, or when returning to a program/console
     memset(Events,0,256*4); //0 it all so that any not set goes to default
	SetEvent(0,DefaultEvent);
	SetEvent(KEYBOARD_EVENT,KeyboardEvent);
	SetEvent(MOUSE_CHANGE,MouseEvent);
}



/*Default Event Handlers*/


void DefaultEvent(unsigned char trash1,unsigned int trash2,void *trash3){
	//there is nothing!
}

void KeyboardEvent(unsigned char trash,unsigned int keys,void *trash2){
	put_kbd_buffer((keys&0xFF00)>>8,keys&0x00FF);
}

unsigned char temp_color=0x0F;
unsigned int last_x=0,last_y=0;
void MouseEvent(unsigned char trash,unsigned int len,void *mouse){
	MouseInfo *tmp;
	tmp=mouse;
	//set_color1(temp_color,last_x,last_y); //redraw the cursor
	//temp_color=get_color1(tmp->mouse_x,tmp->mouse_y);
	last_x=tmp->mouse_x;
	last_y=tmp->mouse_y;
	//set_color1(temp_color^1,tmp->mouse_x,tmp->mouse_y);
}



void Com1ReadyWrite(unsigned char trash, unsigned int len, void* mouse){

}



//drawing_event is not used so handled by default





