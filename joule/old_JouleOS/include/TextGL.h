//PUBLIC DOMAIN
#ifndef _TEXTGL
//#define _K_MISC_H
#ifdef __cplusplus
extern "C"{
#endif



#define HLINE 0
#define VLINE 1
#define DLINE_P 2
#define DLINE_N 3
#define LBLOCK 4
#define MBLOCK 5
#define DBLOCK 6
#define BBLOCK 7
#define LTCORNER 8
#define LBCORNER 9
#define RTCORNER 10
#define RBCORNER 11
#define INTERSECT 12
#define RLINE 13
#define LLINE 14
#define BLINE 15
#define TLINE 16
#define DOT 17





typedef struct{ //can also be cast to an array of chars which explains the macros above
	unsigned char hline; //horizontal line
	unsigned char vline; //vertical line
	unsigned char dline_p; //with positive slope
	unsigned char dline_n; //with negative slope
	unsigned char lblock; //light colored block
	unsigned char mblock; //medium colored block
	unsigned char dblock; //dark colored block
	unsigned char bblock; //total black block
	unsigned char ltcorner; //left top
	unsigned char lbcorner; //left bottom
	unsigned char rtcorner; //right top
	unsigned char rbcorner; //right bottom
	unsigned char intersect; //where 2 lines cross
	unsigned char rline; //vline with a right line coming out in the middle
	unsigned char lline; //vline with a left line coming out in the middle
	unsigned char bline; //hvline with a bottom line coming out in the middle
	unsigned char tline; //hvline with a top line coming out in the middle
	unsigned char dot; //a dot looking character
}TEXT_GRAPHICS; //this struct has all the values for the characters -18bytes




typedef struct{
	unsigned char buffered; //0 for no buffering; 1 for 1 buffer; 2 for 2 buffers; 3 for emulation of vmem but is actually buffered
	//4 for emulation of vmem and 1 buffer; 5 for emulation of vmem and 2 buffers
	unsigned int nagative_length; //this tell how far negative you can go that will actually matter, nothing above 2mil.
	unsigned short negative; //1&2:if negative values are supported for x/y;0 for not;1 for buffer2 is used as the buffer for it
	//3 for after buffer1;4 for before buffer1;5 for after buffer2;6 for before buffer2;7 for after vmem;8for before vmem
	unsigned int size; //size of vmem, and buffers
	void *vmem; //the pointer to the actual video memory or emulated video memory
	void *buffer1; //the pointer to the first buffer
	void *buffer2; //the pointer to the second buffer
	unsigned int skip_nl; //how many bytes to skip after new line
	unsigned char lcolor; //line color
	unsigned char mcolor; //color of characters over 128 that aren't lines
	unsigned char char_color;  //color of characters under 128
	unsigned char bgcolor; //background color -used for plotr() also
	TEXT_GRAPHICS graphics; //the graphics set for drawing
	unsigned int width;
	unsigned int height;
}TEXT_VIDEO_INFO; //This tells the current drawing "surface" information



extern TEXT_VIDEO_INFO *text_main_video;
extern unsigned char TextGL; //if using TextGL it is 1

void stop_TextGL();
void start_TextGL();
void text_set_defaults(TEXT_VIDEO_INFO *info);
unsigned char text_plotg(signed int x,signed int y,unsigned char number);
void *text_get_video();
void text_load_video(void *ptr);





#ifdef __cplusplus
}
#endif

#endif