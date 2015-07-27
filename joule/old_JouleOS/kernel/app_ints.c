//See COPYRIGHT.txt for info on copyright
#include <k_JouleOS.h>
#include <stdio.h>
#include <event_drive.h>


/*this contains interrupts for use at runtime by applications and drivers*/


/***The int 0x51 paremeters and index's
you must move data to the special data area which is at 0x2000
the maximum size of the structs is 1 page(4096 bytes) making the limit 0x3000
0x2000 actually should be the index and 0x2002 be the sub-index for the interrupt and paremeters start at 0x2004

everything is an int, unless stated otherwise

index 0: -cursor control
sub-index 0: -Set Cursor Location (x,y)
gets data from X,Y
Sets X to -1 if either one is out of scope for the current video mode

sub-index 1: -(x,y)=Get Cursor Location
Sets X,Y

sub-index 2: Set Cursor Color/blinking(byte color_value)
Gets from color_value and uses it as the current_color
sets nothing

sub-index 3: (byte color_value)=Get Cursor Color/Blinking
Sets color_value to the current_color

sub-index 4: (width,height)=Get Cursor Limit
gets width and height to the current video mode width and height


index 1: -Lowlevel screen control(video modes and such)
sub-index 0: -Set Current Video Mode(mode)
Gets mode and uses it as the current video mode

sub-index 1: (mode)=Get Current Video Mode
Sets Mode to the current video mode

sub-index 2:  Set Video Registers(width,height,graphics,59 bytes for them) -video mode is set to 0xFF when in this custom mode
sets the vga registers to those 59 bytes
width and height are just for setting the cursor stuff
graphics is 0 if its text, else its how many bits are for each pixel(in mode 13h its 8 and 12h its 4)

sub-index 3: (mode,width,hieght,graphics,59bytes)=Get Video Registers
mode will be 0xFF if you used Set Video Registers earlier else it will be its assigned number
will read everything as set above

sub-index 4: (size, pointer to malloc'ed data area)=Get Raw Text Font
because some fonts can be bigger than 1 page we malloc(size) and in that will be the raw data
for the font


-- inside of both of these, size tells the height for the text, 2048=8, 4096=16(in text mode that is!)
sub-index 5: Set Raw Text Font(pointer to data, size)
same as above except for we do not malloc though you are welcome to


index 2: higher level drawing stuff

sub-index 0: Cls
this clears the screen with the current color

sub-index 1: Cls(color)
this clears the screen with that color

sub-index 2: Scroll(SIGNED how_much)
this will scroll how_much with - being scroll down and + being scroll up

sub-index 3: (pointer to video memory)=Get Video Memory Address
this returns a pointer to video memory, this is the hardware video memory -usually 0xb8000 for text

sub-index 4: Set Video Buffer(pointer to new video buffer)
This sets the video buffer address -usually should allocate 3 pages to be safe

sub-index 5: (pointer to video buffer)=Get Video Buffer
This gets the video buffer address

sub-index 6: Set No_Draw status(no_draw)
this will set no_draw  --this should be 1 to disable buffering or 0 to enable buffering

sub-index 7: (no_draw) Get No Draw Status
as above

sub-index 8: Put Char _normal(BYTE char,BYTE color)
puts a character onto the screen (through the buffer..)

sub-index 9: (BYTE char,BYTE color)=Get Char _normal
gets a character from the screen(actually the buffer but anyway..)

sub-index 10: Put Char _refresh(char,color)
this will put a character into the video buffer and then do a refresh --good for when cli'd

sub-index 11: (char,color)=Get Char _refresh
This will get a character from the video buffer

sub-index 12: Put Char _no_buffer(char,color)
This will put a character straight into video memory, no buffering

sub-index 13: (char,color)=Get Char _no_buffer
This will get a character straight from video memory

index 3: --memory allocation.. and such

sub-index 0: (void *=AddPages(unsigned int number)
this will add number of pages to the address space





*/

volatile unsigned short *int_index=0x2000;
volatile unsigned short *int_sub=0x2002;


void CursorControl_Int51(regs *r){
     struct CurCtrl_0_1 *CurData1=INT_DATA_AREA;
	switch(*int_sub){
		case 0:
		ProcessConsole->curx=CurData1->x;
		ProcessConsole->cury=CurData1->y;
		if(ProcessConsole->curx>ProcessConsole->width || ProcessConsole->cury>ProcessConsole->height){
			CurData1->x= -1;
		}
		break;


	}

}


void AppInt51(regs *r){ //This is for general use of kernel apps
     switch(*int_index){
     	case 0:
     	CursorControl_Int51(r);
     	break;
     	case 51: //special, does a thread switch
     	//SwitchTask(r);
     	break;




     }




}























