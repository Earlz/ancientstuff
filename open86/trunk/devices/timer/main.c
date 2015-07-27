
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

SDL_TimerID timer;





unsigned int timer_callback(unsigned int ticks,void *dat){
    printf("t");


	GlobStruct->PutMessage(PIC_DEVICE,SEND_INTERRUPT,0,NULL,NULL,NULL,NULL);
}





unsigned int EXPORT Open86Init(GlobalDevice *_GlobStruct){
    GlobStruct=_GlobStruct; //store the ptr
    if( SDL_Init( SDL_INIT_TIMER ) == -1 )
    {
        return 1;
    }

    //system("pause");
    //atexit(SDL_Quit);

   timer=SDL_AddTimer(55,timer_callback,0);
    if(timer==NULL){printf("\nerror creating thread\n");SDL_Quit();return 1;}
    sleep(500);
   // sleep(500); //give it time to start..


}






























