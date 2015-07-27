//See COPYRIGHT.txt for info on copyright
#include "k_JouleOS.h"
#include <video.h>

unsigned int xpos=0,ypos=0;
void _printc(char chr,CONSOLE *console);
extern volatile CONSOLE *ProcessConsole;




extern char getkey(char *buf);






void em_printf(char *txt){ //this prints straight to video memory assuming mode 0; for emergency
    char *ptr;
    while (*txt!=0) {
        switch (*txt) {
            case '\n':
            xpos=0;
            ypos++;
            break;

            default:
            ptr=(xpos+(ypos*80))*2;
            ptr=ptr+0xb8000;
            *ptr=*txt;
            ptr++;
            *ptr=RED;
            xpos++;
            break;
        }
        txt++;
    }
}




void g_printf(char *txt){
	while(*txt!=0){


	}
}

void special_printf(char *txt){ //doesn't check format..
     char **args=&txt; //get the stack location..
     args=args+4; //now at the ...
     while (*txt!=0) {
        switch (*txt) {
            case '\n':
            ProcessConsole->cury++;
            ProcessConsole->curx=0;
            break;
            case '\r':
            //do nothing
            break;
            case '\b': //backspace
            ProcessConsole->curx--; //do error checking and stuff later
            break;
            default:
            if (ProcessConsole->cury>=ProcessConsole->height-1){
            	if (c_ops.wait_on_scroll==1){waitkey();}
            	scrolldowna(1);//scrollup(1);def_console->curx=0;def_console->cury++;

          }
            putc(*txt);
            ProcessConsole->curx++;
            break;
        }
        txt++;
    }
}

void k_special_printf(char *txt){ //doesn't check format..
     char **args=&txt; //get the stack location..
     args=args+4; //now at the ...
     while (*txt!=0) {
        switch (*txt) {
            case '\n':
            ProcessConsole->cury++;
            ProcessConsole->curx=0;
            break;
            case '\r':
            //do nothing
            break;
            case '\b': //backspace
            ProcessConsole->curx--; //do error checking and stuff later
            break;
            default:
            if (ProcessConsole->cury>=ProcessConsole->height-1){
            	if (c_ops.wait_on_scroll==1){waitkey();}
            	scrolldowna(1);//scrollup(1);def_console->curx=0;def_console->cury++;

          }
            k_putc(*txt);
            ProcessConsole->curx++;
            break;
        }
        txt++;
    }
}


void printf(char *txt,...){
     unsigned char o_color,fcolor;
     unsigned int x,y;
     char **args=(char**)&txt; //get the stack location..

     //args=(char**)&txt;
     fcolor=ProcessConsole->color;
     *args++; //now at the ...
     while (*txt!=0) {
        switch (*txt) {
            case '\n':
            ProcessConsole->cury++;
            ProcessConsole->curx=0;
            break;
            case '\r':
            //do nothing
            break;
            case '\b': //backspace
            ProcessConsole->curx--; //do error checking and stuff later
            break;
            case '%': //format
            //printf("4");
            txt++;
            switch(*txt){
                 case 's': //string
                 //printf("1");
                 special_printf(*args++); //do it special so it don't check the format
                 break;
                 case 'x': //hex
                 special_print_hex((unsigned int)*args);
                 *args++;
                 break;
                 case 'd':
                 case 'u': //unsigned int
                 print_decu((unsigned int)*args);
                 *args++;
                 break;

                 case 'j': //change console...
                 txt++;
                 switch(*txt){
                      case 'c': //color
                      o_color=ProcessConsole->color;
                      ProcessConsole->color=*args++;
                      break;
                      case 'x':
                      x=ProcessConsole->curx;
                      ProcessConsole->curx=*args++;
                      break;
                      case 'y':
                      y=ProcessConsole->cury;
                      ProcessConsole->cury=*args++;
                      default:
                      txt--;
                      break;
                 }
                 break;
                 case 'r': //restore..
                 txt++;
                 switch (*txt){
                      case 'c': //restore color
                      ProcessConsole->color=o_color;
                      break;
                      case 'f': //fully restore color
                      ProcessConsole->color=fcolor;
                      break;
                      case 'x': //x
                      ProcessConsole->curx=x;
                      break;
                      case 'y':
                      ProcessConsole->cury=y;
                      break;

                      default:
                      txt--;
                      break;
                 }
               break;



                 default:
                 txt--;
                 if (ProcessConsole->cury>=ProcessConsole->height-1){
            	if (c_ops.wait_on_scroll==1){waitkey();}
            	scrolldowna(1);//scrollup(1);def_console->curx=0;def_console->cury++;
               }
               _putc(*txt);
               ProcessConsole->curx++;
               break;
          }
          //*args=*args+1;
          break;

            default:
            if (ProcessConsole->cury>=ProcessConsole->height-1){
            	if (c_ops.wait_on_scroll==1){waitkey();}
            	scrolldowna(1);//scrollup(1);def_console->curx=0;def_console->cury++;

          }
            putc(*txt);
            ProcessConsole->curx++;
            break;
        }
        txt++;
    }
    ProcessConsole->color=fcolor;
}
void k_printf(char *txt,...){
     unsigned char o_color,fcolor;
     unsigned int x,y;
     char **args=(char**)&txt; //get the stack location..
     //args=(char**)&txt;
     *args++; //now at the ...
     fcolor=ProcessConsole->color;
     while (*txt!=0) {
        switch (*txt) {
            case '\n':
            ProcessConsole->cury++;
            ProcessConsole->curx=0;
            break;
            case '\r':
            //do nothing
            break;
            case '\b': //backspace
            ProcessConsole->curx--; //do error checking and stuff later
            break;
            case '%': //format
            //printf("4");
            txt++;
            switch(*txt){
                 case 's': //string
                 //printf("1");
                 k_special_printf(*args++); //do it special so it don't check the format
                 break;
                 case 'x': //hex
                 k_special_print_hex((unsigned int)*args);
                 *args++;
                 break;
                 case 'd':
                 case 'u': //unsigned int
                 k_print_decu((unsigned int)*args);
                 *args++;
                 break;

                 case 'j': //change console...
                 txt++;
                 switch(*txt){
                      case 'c': //color
                      o_color=ProcessConsole->color;
                      ProcessConsole->color=*args++;
                      break;
                      case 'x':
                      x=ProcessConsole->curx;
                      ProcessConsole->curx=*args++;
                      break;
                      case 'y':
                      y=ProcessConsole->cury;
                      ProcessConsole->cury=*args++;
                      default:
                      txt--;
                      break;
                 }
                 break;
                 case 'r': //restore..
                 txt++;
                 switch (*txt){
                      case 'c': //restore color
                      ProcessConsole->color=o_color;
                      break;
                      case 'f': //fully restore color
                      ProcessConsole->color=fcolor;
                      break;
                      case 'x': //x
                      ProcessConsole->curx=x;
                      break;
                      case 'y':
                      ProcessConsole->cury=y;
                      break;

                      default:
                      txt--;
                      break;
                 }
               break;


                 default:
                 txt--;
                 if (ProcessConsole->cury>=ProcessConsole->height-1){
            	if (c_ops.wait_on_scroll==1){waitkey();}
            	scrolldowna(1);//scrollup(1);def_console->curx=0;def_console->cury++;
               }
               k_putc(*txt);
               ProcessConsole->curx++;
               break;
          }
          //*args=*args+1;
          break;

            default:
            if (ProcessConsole->cury>=ProcessConsole->height-1){
            	if (c_ops.wait_on_scroll==1){waitkey();}
            	scrolldowna(1);//scrollup(1);def_console->curx=0;def_console->cury++;

          }
           k_putc(*txt);
            ProcessConsole->curx++;
            break;
        }
        txt++;
    }
    ProcessConsole->color=fcolor;
}


void scanf(char *frmt,...){
//     char **args=(char**)&txt; //get the stack location..



}


unsigned int GetUnsignedInt(char *buffer){ //gets it in string..
     unsigned int i;
     char buf[3];
     buf[1]=0;
     for(i=7;i==0;i--){
     *buffer=getkey(buf);
     switch(*buffer){
          case '\n':
          return;
          break;
          default:
          if(*buffer>='0' && *buffer<='9'){buf[0]=*buffer;buf[1]=0;printf(buf);}
          break;
          case '\b':
          break;
     }

     }
}




void inputf(char *txt,...){
      unsigned char o_color,fcolor;
     unsigned int x,y;
     char **args=(char**)&txt; //get the stack location..

     //args=(char**)&txt;
     fcolor=ProcessConsole->color;
     *args++; //now at the ...
     while (*txt!=0) {
        switch (*txt) {
            case '\n':
            ProcessConsole->cury++;
            ProcessConsole->curx=0;
            break;
            case '\r':
            //do nothing
            break;
            case '\b': //backspace
            ProcessConsole->curx--; //do error checking and stuff later
            break;
            case '%': //format
            //printf("4");
            txt++;
            switch(*txt){
                 case 's': //string
                 //printf("1");
                 special_printf(*args++); //do it special so it don't check the format
                 break;
                 case 'x': //hex
                 special_print_hex((unsigned int)*args);
                 *args++;
                 break;
                 case 'd':
                 case 'u': //unsigned int
                 print_decu((unsigned int)*args);
                 *args++;
                 break;

                 case 'j': //change console...
                 txt++;
                 switch(*txt){
                      case 'c': //color
                      o_color=ProcessConsole->color;
                      ProcessConsole->color=*args++;
                      break;
                      case 'x':
                      x=ProcessConsole->curx;
                      ProcessConsole->curx=*args++;
                      break;
                      case 'y':
                      y=ProcessConsole->cury;
                      ProcessConsole->cury=*args++;
                      default:
                      txt--;
                      break;
                 }
                 break;
                 case 'r': //restore..
                 txt++;
                 switch (*txt){
                      case 'c': //restore color
                      ProcessConsole->color=o_color;
                      break;
                      case 'f': //fully restore color
                      ProcessConsole->color=fcolor;
                      break;
                      case 'x': //x
                      ProcessConsole->curx=x;
                      break;
                      case 'y':
                      ProcessConsole->cury=y;
                      break;

                      default:
                      txt--;
                      break;
                 }
               break;



                 default:
                 txt--;
                 if (ProcessConsole->cury>=ProcessConsole->height-1){
            	if (c_ops.wait_on_scroll==1){waitkey();}
            	scrolldowna(1);//scrollup(1);def_console->curx=0;def_console->cury++;
               }
               _putc(*txt);
               ProcessConsole->curx++;
               break;
          }
          //*args=*args+1;
          break;

            default:
            if (ProcessConsole->cury>=ProcessConsole->height-1){
            	if (c_ops.wait_on_scroll==1){waitkey();}
            	scrolldowna(1);//scrollup(1);def_console->curx=0;def_console->cury++;

          }
            putc(*txt);
            ProcessConsole->curx++;
            break;
        }
        txt++;
    }
    ProcessConsole->color=fcolor;



}











extern char getkey(char *buf);
char *gets(char *buf){ //get a string from the keyboard
     int x,y;
	char tmp[2];
	x=ProcessConsole->curx;
     for(;;){
     	getkey(tmp);
     	//tmp[1] is asci code; tmp[0] is scancode
     	if(tmp[1]==0){
     	switch(tmp[0]){
     	}
     	}else{
     	switch(tmp[1]){
     		//case 27:
     		//buf--;
     		//break;
     		case '\n':
     		//enter/return
     		*buf=0;
     		return buf;
     		break;
     		case '\b':
     		if (x<ProcessConsole->curx){
     		buf--;
     		*buf=0;
     		ProcessConsole->curx--;
     		putc(' ');
     		//ProcessConsole->curx--;


     		}
     		break;



     		default:
               //*buf=tmp[1];
               *buf=tmp[1];
               putc2(*buf);
               buf++;
               break;
     	}
     	}
     }
     //buf--;
     //*buf=0;
}




















