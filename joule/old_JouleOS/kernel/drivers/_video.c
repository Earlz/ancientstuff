//See COPYRIGHT.txt for info on copyright
//this is for conversion from one video driver to another
#include <mytypes.h>
#include <video.h>
//text modes
extern unsigned char g_40x25_text[];
extern unsigned char g_40x50_text[];
extern unsigned char g_80x25_text[];
extern unsigned char g_80x50_text[];
extern unsigned char g_90x30_text[];
extern unsigned char g_90x60_text[];
//graphic modes
extern unsigned char g_640x480x2[];
extern unsigned char g_320x200x4[];
extern unsigned char g_640x480x16[];
extern unsigned char g_720x480x16[];
extern unsigned char g_320x200x256[];
extern unsigned char g_320x200x256_modex[];
//fonts
extern unsigned char g_8x8_font[2048];
extern unsigned char g_8x16_font[4096];
//functions
void write_font(unsigned char *buf, unsigned font_height);
void set_plane(unsigned p);
void write_regs(unsigned char *regs);
static void vmemwr(unsigned dst_off, unsigned char *src, unsigned count);









#define cursory CurrentConsole->cury
#define cursorx CurrentConsole->curx

#define NO_BUFFER 0
#define NORMAL_BUFFER 1
#define BUFFER_REFRESH 2

//createconsole types
#define SYSTEM_CONSOLE 0
#define USER_CONSOLE 1
#define REALTIME_CONSOLE 2
#define VIRTUAL_CONSOLE 0
//more attributes
#define NO_KEYBOARD 4 //means that keyboard is to be ignored








void SwitchConsole(CONSOLE *which);
//video modes and their appropriate arrays.. --not arranged in BIOS order!
//only text for now
void *video_modes[]={
    g_90x60_text,
    g_80x25_text,
    g_90x30_text,
    g_80x50_text,
    g_40x50_text,
    g_40x25_text};



volatile CONSOLE *FirstConsole; //these are mostly for list purposes..
volatile CONSOLE *LastConsole;
volatile CONSOLE *CurrentConsole;
volatile CONSOLE *ProcessConsole; //used in multitasking --this is the console the current process draws to
//but it not actually used to display stuff yet(not refreshed..)

volatile CONSOLE *JouleCon; //The JouleOS console handle
volatile CONSOLE *DebugCon; //the JouleOS debugging handle
volatile CONSOLE *SysControlCon; //the system control console

CONSOLE *CreateConsole(unsigned char type,unsigned int owner, char *name,unsigned char video_mode){
    CONSOLE *new_console;

    __asm("cli"); //disable ints so no drawing crap going on
    new_console=malloc(sizeof(CONSOLE));
    if(new_console==0){return 0;}
    LastConsole->next=new_console;
    new_console->prev=LastConsole;
    LastConsole=new_console;
    new_console->next=FirstConsole;
    //copy the name
    new_console->name=malloc(strlen(name));
    memcpy(new_console->name,name,strlen(name));
    //set video mode info
    new_console->video_mode=video_mode;
    new_console->vga_registers=video_modes[video_mode];
    memset(&new_console->ConsoleEvents,0,sizeof(CONSOLE_EVENTS)); //zero the events
    //must resolve the font for each video mode
    switch(video_mode){ //set appropriate fonts, and sizes
        case 0:
        new_console->font=g_8x8_font;
        new_console->text_height=8;
        new_console->width=90;
        new_console->height=60;
        new_console->size=90*60*2;
        break;
        case 1:
        new_console->font=g_8x16_font;
        new_console->text_height=16;
        new_console->width=80;
        new_console->height=25;
        new_console->size=80*25*2;
        break;
        case 2:
        new_console->font=g_8x16_font;
        new_console->text_height=16;
        new_console->width=90;
        new_console->height=30;
        new_console->size=90*30*2;
        break;
        case 3:
        new_console->font=g_8x8_font;
        new_console->text_height=8;
        new_console->width=80;
        new_console->height=50;
        new_console->size=80*50*2;
        break;
        case 4:
        new_console->font=g_8x8_font;
        new_console->text_height=8;
        new_console->width=40;
        new_console->height=50;
        new_console->size=40*50*2;
        break;
        case 5:
        new_console->font=g_8x16_font;
        new_console->text_height=16;
        new_console->width=40;
        new_console->height=25;
        new_console->size=40*25*2;
        break;
    }

    //allocate buffer, even if buffering is disabled
    new_console->buffer=malloc(new_console->size+(new_console->width*2*2)); //provides room for 2 extra lines to avoid memory overwriting
    new_console->video_memory=0xB8000;
    //set owner process
    //only the owner process can kill a console(anyone can request..)
    //unless of course it is a system process(ring0 drivers or kernel services)
    new_console->owner_process=owner;
    switch(type){
        case SYSTEM_CONSOLE: //same as virtual console
        new_console->buffering=NO_BUFFER;
        break;
        case USER_CONSOLE:
        default: //also the default
        new_console->buffering=NORMAL_BUFFER;
        break;
        case REALTIME_CONSOLE:
        new_console->buffering=BUFFER_REFRESH; //puets instantly refreshed
        break;
    }

    //zero out the cursor
    new_console->curx=0;
    new_console->cury=0;
    //set color to white on black
    new_console->color=0x0F;
    new_console->text=TRUE;

    CurrentConsole=new_console; //replace this with SwitchConsole..
    ProcessConsole=new_console;
    SwitchConsole(CurrentConsole);
    _memsetw(ProcessConsole->buffer,(ProcessConsole->color<<8)|' ',ProcessConsole->size/2+1); //cls...
    _memsetw(ProcessConsole->video_memory,(ProcessConsole->color<<8)|' ',ProcessConsole->size/2+1); //and the actual video memory
    __asm("sti"); //renabled interrupts
    return new_console;
}


void RefreshConsole(){
    memcpy(CurrentConsole->video_memory,CurrentConsole->buffer,CurrentConsole->size);
}


void cls(){
    if(ProcessConsole->buffering==NO_BUFFER){
        _memsetw(ProcessConsole->video_memory,(ProcessConsole->color<<8)|' ',ProcessConsole->size/2+1);
    }else{
        _memsetw(ProcessConsole->buffer,(ProcessConsole->color<<8)|' ',ProcessConsole->size/2+1); //use 'space' so the color is printed
        if(ProcessConsole==CurrentConsole && ProcessConsole->buffering==BUFFER_REFRESH){
        RefreshConsole();
        }
    }
}



void SwitchConsole(CONSOLE *which){
    __asm("cli");
    CurrentConsole=which;
    ProcessConsole=which; //just for now..
    write_regs(which->vga_registers);
    write_font(which->font,which->text_height);
    RefreshConsole();
    __asm("sti");
}
void SwitchConsole_int(CONSOLE *which){ //this one doesn't sti
    __asm("cli");
    CurrentConsole=which;
    ProcessConsole=which; //just for now..
    write_regs(which->vga_registers);
    write_font(which->font,which->text_height);
    RefreshConsole();
}

void StartConsole(){
     //CONSOLE *tmp;
     //FirstConsole=malloc(sizeof(CONSOLE));
     LastConsole=malloc(sizeof(CONSOLE));

     FirstConsole=CreateConsole(USER_CONSOLE,0,"JouleCon",2);
     FirstConsole->prev=LastConsole;
     LastConsole->prev=FirstConsole;
     DebugCon=CreateConsole(REALTIME_CONSOLE,0,"JouleOS Debug",1); //realtime so that in an error we could see it
     SysControlCon=CreateConsole(USER_CONSOLE,0,"System Control Console",1);
     //FirstConsole=CreateConsole(USER_CONSOLE,0,"JouleCon",1);
     CurrentConsole=FirstConsole;
     ProcessConsole=FirstConsole;
     cls();
     SwitchConsole(CurrentConsole->next);
     SwitchConsole(CurrentConsole->next);
     SwitchConsole(CurrentConsole->next);

     //SwitchConsole(FirstConsole);
     //free(FirstConsole);
     JouleCon=FirstConsole;
     SwitchConsole(JouleCon);
}


unsigned int _GetCurx(CONSOLE *which){
        unsigned int rety;
        rety=which->curx;
        return rety;
}

void  _SetCurx(unsigned int x,CONSOLE *which){
        which->curx=x;
}

unsigned int GetCurx(){
        return ProcessConsole->curx;
}

void SetCurx(unsigned int x){
        ProcessConsole->curx=x;
}

void _SetCury(unsigned int y,CONSOLE *which){
     which->cury=y;
}

unsigned int _GetCury(CONSOLE *which){
        return which->cury;
}

void SetCury(y){
        ProcessConsole->cury=y;
}

unsigned int GetCury(){
        return ProcessConsole->cury;
}


//this is an advanced putchar letting you specify everything
char _aputchar(unsigned char txt,CONSOLE *which,unsigned char buffering,unsigned char color,unsigned int x,unsigned int y){
        unsigned char *tmp;
        text_char *dat;
     switch(buffering){ //this determines how the buffering works
          case NO_BUFFER: //uses the video memory instead of buffer
          dat=which->video_memory;
          dat=dat+(((y*(which->width))+(x)));
          if((unsigned int)dat>(which->size+which->video_memory)){return -1;} //at the end so just return
          tmp=dat;
          tmp[0]=txt;
          tmp[1]=color;
          break;
          case NORMAL_BUFFER:
          dat=&which->buffer[((y*which->width)+x)*2];
          if((unsigned int)dat>(which->size+which->buffer)){return -1;} //at the end so just return
          dat->txt=txt;
          dat->color=color;
          break;
          case BUFFER_REFRESH:
          dat=&which->buffer[((y*which->width)+x)*2];
          if((unsigned int)dat>(which->size+which->buffer)){return -1;} //at the end so just return
          dat->txt=txt;
          dat->color=color;
          memcpy(which->video_memory,which->buffer,which->size); //refresh the buffer
          break;
     }
}

char putc(char *txt){ //simplified!
    _aputchar(txt,ProcessConsole,ProcessConsole->buffering,ProcessConsole->color,ProcessConsole->curx,ProcessConsole->cury);
    //ProcessConsole->curx++;
}

void _k_putc(char *txt,int x,int y){ //uses no buffering
     _aputchar(txt,ProcessConsole,NO_BUFFER,ProcessConsole->color,x,y);
}
void k_putc(char *txt){
     _aputchar(txt,ProcessConsole,NO_BUFFER,ProcessConsole->color,ProcessConsole->curx,ProcessConsole->cury);
}




char putc2(char *txt){ //simplified!
    _aputchar(txt,ProcessConsole,ProcessConsole->buffering,ProcessConsole->color,ProcessConsole->curx,ProcessConsole->cury);
    ProcessConsole->curx++;
}

void scrolldowna(unsigned int lines){ //this is improperly named


     memcpy(ProcessConsole->buffer,
     ProcessConsole->buffer+((ProcessConsole->width*2)*lines),
     ProcessConsole->size);
     if(ProcessConsole->buffering!=BUFFER_REFRESH){
     _memsetw(ProcessConsole->buffer+((ProcessConsole->width*2)*(ProcessConsole->height-1)),(ProcessConsole->color<<8)+' ',ProcessConsole->width*2);
     }else{
          _memsetw(ProcessConsole->buffer+((ProcessConsole->width*2)*(ProcessConsole->height-1)),(ProcessConsole->color<<8)+' ',ProcessConsole->width*2);
       //   _memsetw(ProcessConsole->video_memory+((ProcessConsole->width*2)*(ProcessConsole->height-1)),(ProcessConsole->color<<8)+' ',ProcessConsole->width*2);
     }
    // memset(ProcessConsole->buffer+ProcessConsole->size-(ProcessConsole->width*2*2),(ProcessConsole->color<<8)|' ',ProcessConsole->width*2*2); //sets
     ProcessConsole->curx=0;
     ProcessConsole->cury-=lines;
    // RefreshConsole();
}


void _putc(char txt,unsigned int x,unsigned int y){ //a slightly advanced version of putc
     _aputchar(txt,ProcessConsole,ProcessConsole->buffering,ProcessConsole->color,x,y);
}



void SetConsoleMode(unsigned char video_mode){
     ProcessConsole->video_mode=video_mode;
     ProcessConsole->vga_registers=video_modes[video_mode];
    //must resolve the font for each video mode
    switch(video_mode){ //set appropriate fonts, and sizes
        case 0:
        ProcessConsole->font=g_8x8_font;
        ProcessConsole->text_height=8;
        ProcessConsole->width=90;
        ProcessConsole->height=60;
        ProcessConsole->size=90*60*2;
        break;
        case 1:
        ProcessConsole->font=g_8x16_font;
        ProcessConsole->text_height=16;
        ProcessConsole->width=80;
        ProcessConsole->height=25;
        ProcessConsole->size=80*25*2;
        break;
        case 2:
        ProcessConsole->font=g_8x16_font;
        ProcessConsole->text_height=16;
        ProcessConsole->width=90;
        ProcessConsole->height=30;
        ProcessConsole->size=90*30*2;
        break;
        case 3:
        ProcessConsole->font=g_8x8_font;
        ProcessConsole->text_height=8;
        ProcessConsole->width=80;
        ProcessConsole->height=50;
        ProcessConsole->size=80*50*2;
        break;
        case 4:
        ProcessConsole->font=g_8x8_font;
        ProcessConsole->text_height=8;
        ProcessConsole->width=40;
        ProcessConsole->height=50;
        ProcessConsole->size=40*50*2;
        break;
        case 5:
        ProcessConsole->font=g_8x16_font;
        ProcessConsole->text_height=16;
        ProcessConsole->width=40;
        ProcessConsole->height=25;
        ProcessConsole->size=40*25*2;
        break;
    }
     free(ProcessConsole->buffer);
     ProcessConsole->size=(ProcessConsole->width*2*ProcessConsole->height);
     ProcessConsole->buffer=malloc(ProcessConsole->size);
     cls();
     __asm("cli");
    write_regs(ProcessConsole->vga_registers);
    write_font(ProcessConsole->font,ProcessConsole->text_height);
    RefreshConsole();
    __asm("sti");
}


void set_video_mode_s(char *argp){
     if(_strcmp(argp,"0")){
          SetConsoleMode(0);
     }
     if(_strcmp(argp,"1")){
          SetConsoleMode(1);
     }
     if(_strcmp(argp,"2")){
          SetConsoleMode(2);
     }
     if(_strcmp(argp,"3")){
          SetConsoleMode(3);
     }
     if(_strcmp(argp,"4")){
          SetConsoleMode(4);
     }
     if(_strcmp(argp,"5")){
          SetConsoleMode(5);
     }
}

