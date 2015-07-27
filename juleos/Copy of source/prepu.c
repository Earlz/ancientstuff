/*>>>>JuleOS<<<<*/
/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>Version 0.0.3 ALPHA<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>prepu.c; Initializations, defines, and includes<<<<*/
#include <bios.h>
 #include <stdlib.h>
 #include <math.h>
#ifndef  _DEFS_H_
# define _DEFS_H_
# define JULEOS_VER 0.0.4
# define JULEOS_VERs "0.0.4"
# define scanf_BUFFER 128
 #define nw nl();
 #define waitkey getkey();
#define index_seg 0x14ff
#define curx getcurx()
#define cury getcury()
#define str2int atoi
#define int2str itoa
#define asminc asm include
/* Color Constants */

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
unsigned char DISPLAY_TYPE=1; /*1=vga 2=cga rest 3=text(maybe) */
 typedef signed	char	s8b	;
 typedef signed	int	s16b	;
 typedef signed long	s32b	;

 typedef unsigned char	u8b	;
 typedef unsigned int	u16b	;
 typedef unsigned long	u32b	;
 typedef unsigned char byte;
 typedef unsigned int word;
 typedef unsigned long dword;
typedef struct {
int sector;
	int head;
	int drive;
	int track;
	byte nsecs;
}drive_params;
typedef struct{
	word cluster_size;
	dword disk_size; /*disk size in bytes for ease of development even if there is a 4gb limit*/
}fs_info;
typedef struct{
	byte scan;
	byte asc;
}keys; /*special struct used by getkey, can be used by any function*/
typedef struct{
	byte mode;
	byte page;
	word width;
	word height;
	byte tcolor;
	byte rows;
	byte cols;
	byte* vram_ptr;
}console_info; /*contains console/shell internal info, these are to be changed only by update_cinfo()*/
typedef struct{
	byte number;
	byte ver[1];
	word lastptr;
	word lastadr;
	word next;
	word table1[255]; /*table contains cluster adress of each index*/
}index;
byte shell_v; /*used by input, should only be set to 1 if you are setup for it, should be returned to 0 IMMEDIANTLY after use*/
index gindex;
console_info con_inf; /*global struct meant to be made only right here*/
byte video_page=0; /*This should be updated when video page is changed*/
/*word curx,cury;*/
byte *VideoMemory;
byte indexb[1024]; /*temporary*/
word total_memory; /*should only be written by checkmem, can be read by anything, contains the total amount of memory in kilobytes*/
# define NULL 	((void*)0)	
#endif
#include "include\conv.h"
# include "lowlevel.c"

# include "midlevel.c"
# include "include\stdio.h"
# include "FDFS\FDFS.c"
# include "shell\main.c"


