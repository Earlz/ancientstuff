
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "../../Open86_Device.h"
#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

volatile GlobalDevice *GlobStruct;

SDL_Surface *screen;
unsigned int swidth=640,sheight=200;
SDL_Thread *messaging,*drawme;
#define render() SDL_UpdateRect(screen,0,0,swidth,sheight)
volatile SDL_Event event;


void PutPixel(unsigned int x,unsigned int y,unsigned char color){
	unsigned int offset;
	unsigned char *buffer=screen->pixels;
	offset=x+(screen->pitch*y);
	buffer[offset]=color;
}


/****FINALLY!!! finally got the stupid check messages thread working properly!*****/
int DrawMe2(void *dat){
	for(;;){
		PutPixel((unsigned char)rand(),(unsigned char)rand(),(unsigned char)rand());
		render();
	}
}




void MessageRecv(Open86Message *msg){
	unsigned int ptr;
	unsigned char *tmp=msg->extra_ptr;
	unsigned int x,y;
		//GlobStruct->LockCPU();
		ptr=0;

    for(y=0;y<sheight;y++){
    	for(x=0;x<swidth;x++){
			PutPixel(x,y,tmp[ptr]);

			ptr++;


			}

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
	screen=SDL_SetVideoMode( swidth, sheight, 8,0 );
	SDL_WM_SetCaption("Open86 CGA Display","");
	printf("*");
    if(screen==NULL){
    	printf("error with setting video mode\n");
    }
	printf("*");
	GlobStruct->InstallMessageEvent(GUI,&MessageRecv);
	while((volatile)quit==0){
	if(SDL_WaitEvent(&event)==0){printf("x");}
    	switch(event.type){
    		case SDL_QUIT:
    		quit=1;
    		break;
    		case SDL_KEYDOWN:
    		GlobStruct->PutMessage(KEYBOARD,KEY_DOWN,event.key.keysym.scancode,NULL,NULL,NULL,NULL);
    		printf("t");
    		break;
    		case SDL_KEYUP:
    		GlobStruct->PutMessage(KEYBOARD,KEY_UP,event.key.keysym.scancode,NULL,NULL,NULL,NULL);
    		break;
		}
	}
	GlobStruct->UninstallMessageEvent(GUI);
	sleep(500); //wait for that to take a bit of effect..
	SDL_Quit();
	GlobStruct->PutMessage(OPEN86_CORE,KILL_OPEN86,0,NULL,NULL,NULL,NULL); //shut it down

}


void do_nothing(void){

}


unsigned int EXPORT Open86Init(GlobalDevice *_GlobStruct){
    GlobStruct=_GlobStruct; //store the ptr
    if( SDL_Init( SDL_INIT_EVENTTHREAD ) == -1 )
    {
        return 1;
    }

    //system("pause");
    //atexit(SDL_Quit);

   messaging=SDL_CreateThread(CheckMessages,0);
    if(messaging==NULL){printf("\nerror creating thread\n");SDL_Quit();return 1;}
    sleep(500); //give it time to start..
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






























