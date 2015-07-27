//See COPYRIGHT.txt for info on copyright
#ifndef MOUSE_H
#define MOUSE_H



void mouse_install();
void mouse_handler();


typedef struct{
	volatile unsigned int mouse_x;
     volatile unsigned int mouse_y;
     volatile unsigned char mouse_click;
}MouseInfo;

extern volatile MouseInfo Mouse1;



#endif


