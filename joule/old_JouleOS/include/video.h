//See COPYRIGHT.txt for info on copyright
#ifndef _K_VIDEO_H
#define _K_VIDEO_H
#ifdef __cplusplus
extern "C" {
#endif
 # define BLACK		0
 # define BLUE  	1
 # define GREEN		2
 # define CYAN		3
 # define RED		4
 # define MAGENTA	5
 # define BROWN 	6
 # define LIGHTGRAY	7
 # define DARKGRAY	8
 # define LIGHTBLUE 	9
 # define LIGHTGREEN	10
 # define LIGHTCYAN	11
 # define PINK		12
 # define LIGHTMAGENTA	13
 # define YELLOW	14
 # define WHITE		15

//this is for conversion from one video driver to another
#include <mytypes.h>
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

typedef struct{
     void(*KeyRecv)(unsigned char); //just gives you the scancode.. use getc for ascii code
     void(*Redraw)();
     void(*OutFocus)(void *); //gives you the pointer to the console it goes to
     void(*InFocus)(void *); //gives you the pointer to the console it goes out from
     void(*ChangeDisplay)(unsigned char); //gives you the mode of display it changes to
}CONSOLE_EVENTS;



typedef struct{
    void *prev; //previous console
	char *name;
	unsigned int width,height;
     char video_mode; //video mode for it, only used for reference
     BOOL text; //1 if text mode else its graphics
	void *font; //this is the pointer to the font for text, for graphics its a pointer to a font handle
	char *buffer; //this is the buffer used in double buffering;
	unsigned int size; //max size for buffer
	unsigned int attributes;
	char *video_memory; //this is the actual video memory
	unsigned char color_bits,text_height; //color_bits is how many bits are used for colors in graphics mode; text_height is how tall a font is
	void *vga_registers; //this is actually what controls the video mode;
//	unsigned char right,left,top,bottom; //these are margins for misc. uses
	unsigned int color; //current color
	unsigned int curx,cury;
	void *owner_process; //not used now...
	unsigned char buffering; //this is not used most of the time but anyway
	//possibly have an event stack of some sort in here...
	CONSOLE_EVENTS ConsoleEvents; //all those nifty little events
	void *next;
}CONSOLE;


typedef struct{
	unsigned char txt,color;
}text_char;


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


extern volatile CONSOLE *JouleCon; //The JouleOS console handle
extern volatile CONSOLE *FirstConsole; //these are mostly for list purposes..
extern volatile CONSOLE *LastConsole;
extern volatile CONSOLE *CurrentConsole;
extern volatile CONSOLE *ProcessConsole; //used in multitasking --this is the console the current process draws to
//but it not actually used to display stuff yet(not refreshed..)


volatile CONSOLE *DebugCon; //the JouleOS debugging handle
volatile CONSOLE *SysControlCon; //the system control console
//Lowlevel stuff
#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
/*			COLOR emulation		MONO emulation */
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
				VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)




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



#ifdef __cplusplus
}
#endif

#endif
