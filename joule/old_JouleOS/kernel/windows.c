/* NOTE (<ALL>#1#): TEXTUI HAS BEEN DEPRICATED, NO MORE DEVING FOR IT!!! */



#define MAIN_WINDOW 0xFFFFFFFFF
/*event names*/
/*these are always enabled when there is a window*/
//these are core window events
#define EV_UNSET 0
#define EV_GOT_FOCUS 1
#define EV_LOST_FOCUS 2
#define EV_MINIMIZED 3
#define EV_RESTORED 4
#define EV_MAXIMIZED 5
#define EV_CLOSE 6
//keyboard
#define EV_KEY_RECV 7









unsigned int TextUI;
#include <video.h>

extern volatile video_info *def_console;

typedef struct{
	video_info draw_inf; //tells video info for window
     void *func_ptrs[256]; //data ptr for event interrupts
     void *draw_buffer; //this is where the drawing buffer is
     void *owner;
     char *title; //this is the shown title
     unsigned int x,y; //where on the screen the window is
     // the drawing buffer has all the characters/pixels for the window and is updated to its parent window which is eventually
     // updated to the video memory
	unsigned int attributes; //attributes for everything
     unsigned int rnumber; //what the registered number is, populated by RegisterWindow
}WINDOW;

typedef void (*EvhFunc)(unsigned char,unsigned int);


volatile WINDOW main_wind;
WINDOW **active_windows;
WINDOW *ActiveWindow;

extern video_info k_con;
unsigned int InitTextUI(){
	active_windows=AllocCore(1);
	memset(active_windows,0,4096);
	main_wind.draw_inf=k_con;
	//main_wind.func_ptrs=AllocCore(1);
	main_wind.draw_buffer=AllocCore(1);
	main_wind.owner=0;
	main_wind.title="MAIN";
	main_wind.x=0;
	main_wind.y=0;
	TextUI=1;
}

unsigned int EndTextUI(){
	FreeCore(active_windows,1);
	//FreeCore(main_wind.func_ptrs,1);
	FreeCore(main_wind.draw_buffer,1);
	TextUI=0;
}



WINDOW *RegisterWindow(WINDOW *buffer){
	unsigned int i;
	for(i=0;i<=64;i++){ //64 window limit should be more than enough
		if (i==64){return 0;}
		if (active_windows[i]==0){break;}
	}
	/*
	buffer->func_ptrs=func_ptrs;
	buffer->draw_buffer=draw_buffer;
	set_defaults(&buffer->draw_inf);
	buffer->draw_inf.width=width;
	buffer->draw_inf.width=height;
	buffer->draw_inf.nl_buffer=0;
	buffer->draw_inf.border_type=1;
	buffer->owner=owner;
	buffer->x=x;
	buffer->y=y;
	buffer->attributes=attributes;
	*/
	active_windows[i]=buffer;
	buffer->rnumber=i;
	return buffer;
}


void DrawWindow(unsigned char num){
	unsigned int width[3],height[3],x,y,tmpy,tmpx;char *vptr[3];char *vbuffer;unsigned int nl;unsigned int rl;unsigned int total;unsigned int i;
	if (active_windows[num]==0){return 0;}
	main_wind.draw_inf=k_con;
	width[0]=active_windows[num]->draw_inf.width;
	width[2]=width[0];
	height[0]=active_windows[num]->draw_inf.height;
	height[2]=height[0];
	x=active_windows[num]->x;
	y=active_windows[num]->y;
	vptr[0]=active_windows[num]->draw_inf.vptr;
	vbuffer=active_windows[num]->draw_buffer;
	width[1]=main_wind.draw_inf.width;
	height[1]=main_wind.draw_inf.height;
	vptr[1]=main_wind.draw_inf.vptr;
	nl=width[1]-(x+width[0]);
	rl=width[1]-(nl+width[0]);
	total=nl+rl;

	tvline(height[0]+1,y,x);
	tvline(height[0]+1,y,x+width[2]+1);
     top_line(width[2]+1,x,y);
	bottom_line(width[2]+1,x,y+height[0]+1);
     //by now the window frame is drawn
     tmpx=x;
     for (i=0l;i<width-2;i++){
     	if (active_windows[num]->title[i]==0){break;}
     	x++;
     	_printca(active_windows[num]->title[i],main_wind.draw_inf,x+1,y);
     }
     x=tmpx;
     
     
     
	//writing here is wrong somehow
	
	for (i=0;i<height[0];i++){
          vptr[2]=vptr[1]+(x*2+2)+((y+1)*(width[1])*2);
          vbuffer=vbuffer+width[2]*2;
         memcpy(vptr[2],vbuffer,width[2]*2-1);
         y++;
	}
	
}


//event 0 is used as the unset event. 0 should be used for any event
//that you do not have implemented, or you could use it so that you have a
//message stack or whatever if you don't like interrupt based events
//any evh ptr that is null(0) will use evh 0



//evh stands for event handler
void call_evh(WINDOW *win,unsigned char event,unsigned int parameter){
     EvhFunc tmp;
     tmp=win->func_ptrs[event];
     tmp(event,parameter);
}


	
	
	

	
     






