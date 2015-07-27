//See COPYRIGHT.txt for info on copyright
#ifndef EVENT_DRIVE_H
#define EVENT_DRIVE_H
#include <mytypes.h>
//event numbers
#define DEFAULT_EVENT 0 //for other events set to 0
#define KEYBOARD_EVENT 1 //for the keyboard
#define DRAWING_EVENT 2 //called when the backbuffer is redrawn
#define COM1_READ 3 //called when com1 has data
#define MOUSE_CHANGE 4
#define NETWORK_READ 6 //called when the network has data
#define COM1_ERROR 7
#define TIMER1 8 //these are timers, they are called when they reach 0
#define TIMER2 9
#define TIMER3 10
#define VIDEO_CHANGE 11 //a change to the video has been made such as a resolution change


//standard events
void DefaultEvent(unsigned char trash1,unsigned int trash2,void *trash3);
void KeyboardEvent(unsigned char trash,unsigned int keys,void *trash2);
void MouseEvent(unsigned char trash,unsigned int len,void *mouse);



//functions
void InitEventDrive();
void EnableEvents();
void DisableEvents();
void CallEvent(unsigned char number,unsigned int dat,void *ptr);
void *GetEvent(unsigned char number);
void *SetEvent(unsigned char number,void *func);



typedef void (*EventFunc)(unsigned char,unsigned int,void *);
//2st parameter is used for those functions that just need 1 int, or optionally used to describe how big the void* is
//3nd parameter is used for extra data
//the unsigned char tells what event and i used internally

extern volatile EventFunc Events[256]; //just 1k so..

extern volatile BOOL EventsEnabled;
extern volatile EventFunc EventStack[32]; //This an overrun stack incase an event happens while an event is running
extern unsigned char EventStackCounter;
extern volatile BOOL event_happening;

















#endif



