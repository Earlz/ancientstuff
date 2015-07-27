//This is PUBLIC DOMAIN and has been depricated...
/*************************************
This is a text based graphics engine
I'm not sure on some stuff but anyway


notes:
1.I'm ignoring negative for now, just to much hassel




*************************************/

#include <TextGL.h>



/***********************************
proposed function list:
text_line(x1,y1,x2,y2); //not sure on how to do a slope that is not 1 or -1
text_circle(x,y,radius) //no idea on how to do this without a custom character set; for now use bblock
text_set_defaults(text_video_info) //sets the defaults for most video adapters, can be used as a template
text_plotg(x,y,char number in graphics struct)
text_plotr(x,y,character)





*/


TEXT_VIDEO_INFO *text_main_video;
unsigned char TextGL;


unsigned char text_convert2graphics(unsigned char what){
	unsigned int i;unsigned char number;
	unsigned char *graphics;
	graphics=&text_main_video->graphics;
	for(i=0;i<sizeof(TEXT_GRAPHICS);i++){
		if(what==graphics[i]){return i;}
	}
	return 0xFF;
}


void start_TextGL(){
	TextGL=1;
}
void stop_TextGL(){
	TextGL=0;
}

void text_load_video(void *ptr){
	text_main_video=ptr;
}

void *text_get_video(){
     return text_main_video;
}

void text_set_defaults(TEXT_VIDEO_INFO *info){
	info->buffered=0;
	info->size=4000;
	info->vmem=0xB8000;
	info->skip_nl=0;
	info->lcolor=0x0F;
	info->mcolor=0x0F;
	info->char_color=0x0F;
	info->width=80;
	info->height=25;
	//now the long and hard part
	info->graphics.hline=0xC4;
	info->graphics.vline=0xB3;
	info->graphics.dline_p='/';
	info->graphics.dline_n='\\';
	info->graphics.lblock=0xB0;
	info->graphics.mblock=0xB1;
	info->graphics.dblock=0xB2;
	info->graphics.bblock=0xDB;
	info->graphics.ltcorner=0xDA;
	info->graphics.lbcorner=0xC0;
	info->graphics.rtcorner=0xBF;
	info->graphics.rbcorner=0xD9;
	info->graphics.intersect=0xC5;
	info->graphics.rline=0xC3;
	info->graphics.lline=0xB4;
	info->graphics.bline=0xC2;
	info->graphics.tline=0xC1;
	info->graphics.dot=0xFE;
}





unsigned char text_plotg(signed int x,signed int y,unsigned char number){
	unsigned char *graphics; //so we can address it by numbers
	unsigned int i;
	unsigned char buffer,negative;
	unsigned int length;
	char *offset;

	buffer=text_main_video->buffered;
	graphics=&text_main_video->graphics;
	//negative=text_main_video->negative;
	switch(buffer){
		case 0: //no buffering
		if(y>text_main_video->height){return 2;}
		offset=x*2+(y*text_main_video->width*2);
		offset=(unsigned int)offset+(unsigned int)text_main_video->vmem;
		if((unsigned int)offset-(unsigned int)text_main_video->vmem+1>text_main_video->size){return 3;}
          *offset=graphics[number];
          offset++;
		if(number!=DOT || LBLOCK || MBLOCK || DBLOCK || BBLOCK){ //if the character is a line
			*offset=text_main_video->lcolor;
		}else{
		*offset=text_main_video->mcolor;
		}
		return 1;
		break;

		case 1: //1 buffer


		break;


		case 2: //2 buffers


		break;


		case 3: //emulation no buffer


		break;


		case 4: //emulation 1 buffer


		break;


		case 5: //emulation 2 buffers



		break;


	}

}


unsigned char text_plotr(signed int x,signed int y,unsigned char number){
	unsigned char *graphics; //so we can address it by numbers
	unsigned int i;
	unsigned char buffer,negative;
	unsigned int length;
	char *offset;

	buffer=text_main_video->buffered;
	//graphics=&text_main_video->graphics;
	//negative=text_main_video->negative;
	switch(buffer){
		case 0: //no buffering
		if(y>text_main_video->height){return 2;}
		offset=x*2+(y*text_main_video->width*2);
		offset=(unsigned int)offset+(unsigned int)text_main_video->vmem;
		if((unsigned int)offset-(unsigned int)text_main_video->vmem+1>text_main_video->size){return 3;}
          *offset=number;
          offset++;
          *offset=text_main_video->bgcolor;
		return 1;
		break;

		case 1: //1 buffer


		break;


		case 2: //2 buffers


		break;


		case 3: //emulation no buffer


		break;


		case 4: //emulation 1 buffer


		break;


		case 5: //emulation 2 buffers



		break;


	}

}









unsigned char text_getg(unsigned int x,unsigned int y){
     unsigned char *graphics; //so we can address it by numbers
	unsigned int i;
	unsigned char number;
	unsigned char buffer,negative;
	unsigned int length;
	char *offset;

	buffer=text_main_video->buffered;
	graphics=&text_main_video->graphics;
     switch(buffer){
		case 0: //no buffering
		if(y>text_main_video->height){return 2;}
		offset=x*2+(y*text_main_video->width*2);
		offset=(unsigned int)offset+(unsigned int)text_main_video->vmem;
		if((unsigned int)offset-(unsigned int)text_main_video->vmem+1>text_main_video->size){return 3;}
          number=*offset;
          number=text_convert2graphics(number);
          return number;
		break;

		case 1: //1 buffer


		break;


		case 2: //2 buffers


		break;


		case 3: //emulation no buffer


		break;


		case 4: //emulation 1 buffer


		break;


		case 5: //emulation 2 buffers



		break;


	}
}




extern unsigned int xpos,ypos;
void text_line(signed int x1,signed int x2,signed int y1,signed int y2){
	unsigned int i;
     if(x2-x1==0){ //undefined/vertical line  --wow algebra helped with somehting!
          for (i=y1;i<=y2;i++){
          	if (text_getg(x1,i)==HLINE){text_plotg(x1,i,INTERSECT);
          	}else{
          	text_plotg(x1,i,VLINE);
          	}

          }
          return;
     }
     if(y2-y1==0){ //0 slope/horizontal line
          for (i=x1;i<=x2;i++){
          	if (text_getg(i,y1)==VLINE){text_plotg(i,y1,INTERSECT);
          	}else{
          	text_plotg(i,y1,HLINE);
          	}

          }
          return;
     }
     if((signed int)(y2-y1)/(signed int)(x2-x1)==1){ //slope of 1/1- diaganol line going right and up
          while(x1<=x2 || y1<=y2){
               text_plotg(x1,y1,DLINE_P);
               x1++;y1++;
          }
          return;
     }
     if((signed int)(y2-y1)/(signed int)(x2-x1)==-1){ //slope of -1/1 or 1/-1 - diaganol line going left and up
          while(x1>=x2 || y1<=y2){
               text_plotg(x1,y1,DLINE_P);
               x1--;y1++;
          }
          return;
     }


}










//TGF image routines

typedef struct
{
	char sig[4];
	unsigned char has_color;
	unsigned int width;
	unsigned int height;
}TGF_HEADER;



void tgf_draw(unsigned int x,unsigned int y,char *ptr){
	TGF_HEADER *head;
	unsigned int i;
	unsigned int ox,oy;
	head=ptr;
	ox=x;
	oy=y;
	ptr=ptr+sizeof(head);
	if(_strcmp(head->sig,"TGF")!=1){return 0;} //error
	if(head->has_color==1){return 0;} //not yet supported
	for(i=0;i<head->width*head->height;i++){
		text_plotr(x,y,*ptr);
		x++;
		if(x==head->width+1){x=ox;y++;}
		if(y==head->height+1){return 0;}
	}
}












