#include <windows.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "../../Open86_Device.h"
#define DLL_EXPORT __declspec(dllexport)

volatile GlobalDevice *GlobStruct;

SDL_Surface *screen;
SDL_Thread *messaging,*drawme;
#define render() SDL_UpdateRect(screen,0,0,640,480)
volatile SDL_Event event;


void PutPixel(unsigned int x,unsigned int y,unsigned char color){
	unsigned int offset;
	unsigned char *buffer=screen->pixels;
	offset=x+(screen->pitch*y);
	buffer[offset]=color;
}


/****FINALLY!!! finally got the stupid check messages thread working properly!*****/

int DrawMe(void *dat){
	printf("&");
    for(;;){
    	PutPixel((unsigned char)rand(),(unsigned char)rand(),(unsigned char)rand());
    	render();
    }
}
unsigned int quit=0;

int CheckMessages(void *dat){ //this is a generic thread that checks for messages and handles them
	printf("*");
	if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
        return 1;
    }
	printf("*");
	screen=SDL_SetVideoMode( 640, 480, 8,0 );
	printf("*");
    if(screen==NULL){
    	printf("error with setting video mode\n");
    }
	printf("*");
	drawme=SDL_CreateThread(DrawMe,0);
	while((volatile)quit==0){
	if(SDL_WaitEvent(&event)==0){printf("x");}
    	if(event.type==SDL_QUIT){
    		(volatile)quit=1;

    }
	}
	SDL_KillThread(drawme);
	SDL_Quit();
}

void do_nothing(void){

}


unsigned int DLL_EXPORT Open86Init(GlobalDevice *_GlobStruct){
    GlobStruct=_GlobStruct; //store the ptr
    if( SDL_Init( SDL_INIT_EVENTTHREAD ) == -1 )
    {
        return 1;
    }

    //system("pause");
    //atexit(SDL_Quit);

   messaging=SDL_CreateThread(CheckMessages,0);
    if(messaging==NULL){printf("\nerror creating thread\n");SDL_Quit();return 1;}
   /*printf("##");
	while(1){
    	SDL_PollEvent(&event);
    	switch(event.type){
			case SDL_QUIT:
			SDL_Quit();
			return(0);
      }
    }
    */
    //sleep(5000);
    //drawing=SDL_CreateThread(DrawScreen,0);
    //SDL_Quit();
	//atexit(do_nothing);
    return 0;

}






























