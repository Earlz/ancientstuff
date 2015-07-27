#include <windows.h>
#include <stdio.h>
#include "../../Open86_Device.h"
#define DLL_EXPORT __declspec(dllexport)
#include <sdl/sdl.h>

// a sample exported function

SDL_TimerID my_time;

volatile GlobalDevice *GlobStruct;
void *core;
extern unsigned char RomFont[];
unsigned char color_translation[16]={ //currently only black and white are translated from 4bit to 8bit
	0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0xFF
};

unsigned char VideoBuffer[640*480];

unsigned int MyTimer(unsigned int interval,void *param){
	unsigned char *ptr=core;
	unsigned char tmp_char,tmp_color;
	unsigned int curx,cury;
	ptr=ptr+0xB8000; //set it to video memory
	tmp_char=*ptr;
	ptr++;
	tmp_color=*ptr;
	curx=0;
	cury=0;





}



unsigned int DLL_EXPORT Open86Init(GlobalDevice *_GlobStruct){
    GlobStruct=_GlobStruct; //store the ptr
    core=GlobStruct->_core;
    if( SDL_Init( SDL_INIT_TIMER ) == -1 )
    {
        return 1;
    }
   my_time=SDL_AddTimer(100,MyTimer,0);
    if(my_time==NULL){printf("\nerror creating timer\n");SDL_Quit();return 1;}
    return 0;

}






















