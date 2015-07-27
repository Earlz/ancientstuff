//See COPYRIGHT.txt for info on copyright
#include <k_misc.h>
#include <stdio.h>
#include <string.h>
#include <k_JouleOS.h>
#include <liballoc.h>
#include <TextGL.h>
#include <assert.h>
#define sarg ';'
#define earg '>'
#define command(cmd_name,function) if (_strcmp(cmd,cmd_name)==1){valid=1; function(argp);}
/*
typedef struct{
	unsigned char wait_on_scroll; //1 for everytime a scroll down happens it waits for a key

}console_ops_struct;
*/





/*actaully is working almost to expectations*/
void test(char *argp);
void print_header(char *argp);
void c_info(char *argp);
void help(char *argp);
void sinfo(char *argp);
void not_on(char *argp);
void c_cls(char *argp);
void new_info(char *argp);
void set_mode(char *argp);
void resize(char *argp);
void move(char *argp);
void list_consoles(char *argp);
volatile char cmdbuf[256]; //not sure why volatile is needed
console_ops_struct c_ops;
void start_console(unsigned char *start_message){
	unsigned int i;char *argp;unsigned char valid;
	char *cmd;unsigned int count;
	if(start_message==0){
	print_header(0);
	}else{
		printf(start_message);
	}
     for(;;){ //begin infinite loop
     //memset(cmdbuf,0,256);
     get_command: //prints the cursor, clears buffer, gets the command(and arg)
     printf(">: ");
     memset(cmdbuf,0,255); //clears the buffer
     gets(cmdbuf);
     printf("\n");
     seperate_arg: //check for semicolon, change semicolon to a null, and set argp to right after the semicolor
    /* for (i=0;i<256 && cmdbuf[i]!=0;i++){
     	if (cmdbuf[i]==sarg){
     		cmdbuf[i]=0; //so we can compare names, we dont compare args
     		argp=&cmdbuf[i+1];
     		break;
     	}

     }*/
     count=_replacec(cmdbuf,sarg,0);
     cmd=cmdbuf;
     for(i=0;i<=count;i+=2){
     (unsigned int)argp=(unsigned int)cmd+strlen(cmd)+1;
     do_command: //figure out what the command is, launch proper function/app, and set valid to 1
     //k_printf(cmdbuf);
     command("test",test);
     command("ver",print_header);
     command("info",c_info);
     command("help",help);
     command("set info",not_on);
     command("cls",c_cls);
     command("set video mode",set_mode);
     command("notepad",note_pad);
     command("terminal",_Terminal);
     command("list",list_consoles);


     /*
     if (_strcmp(cmdbuf,"test")==1){valid=1; test(argp);}
     if (_strcmp(cmdbuf,"ver")==1){valid=1; print_header();}
     if (_strcmp(cmdbuf,"info")==1){valid=1; c_info(argp);}
     if (_strcmp(cmdbuf,"help")==1){valid=1; help(argp);}
     */
     if (strlen(cmdbuf)==0){valid=1;} //so that  when someone enters nothing it dont say invalid

     closing_loop: //this is where error checking and ending initializations are made
     if(valid!=1){ //command wasn't valid
     printf("Invalid command\n");
     }
     valid=0;
     (unsigned int)cmd=(unsigned int)argp+strlen(argp)+1;

     }
     }

}




void not_on(char *argp){
printf("Function currently not stable or supported\n");
}

void sinfo(char *argp){
	if (_strcmp(argp,"wait on scroll")==1){c_ops.wait_on_scroll=1;k_printf("Console now waits for a key on an overload scroll\n");}
     if (_strcmp(argp,"no wait on scroll")==1){c_ops.wait_on_scroll=0;k_printf("Console now don't wait for a key on an overload scroll\n");}
}

void print_header(char *argp){
	printf("JouleOS RD'ed version 0.0.8 alpha\n");
	printf("JouleCon version 0.0.8 alpha\n");
	printf("Release Version 1\n");
}

void c_cls(char *argp){
	cls();
	ProcessConsole->cury=1;
	ProcessConsole->curx=0;

}


void list_consoles(char *argp){
     CONSOLE *tmp;
     tmp=FirstConsole;
     while(tmp!=LastConsole){
          printf("Name: %s Width: %u Height: %u\n",tmp->name,tmp->width,tmp->height);
          tmp=tmp->next;
     }
     printf("Name: %s Width: %u Height: %u\n",tmp->name,tmp->width,tmp->height); //print the last one
}









void set_mode(char *argp){
	set_video_mode_s(argp);
}


void GotFocus(int event,int parameter){

}
void gotkey(char event,int parameter){
	printf("testing");
	//put_kbd_buffer((parameter&0xFF00)>>8,parameter&0xFF);
}
typedef void(*FuncPtr)(char*);
FuncPtr tests[20];


extern unsigned int xpos,ypos;
extern char logo1[];
char tmpbufxy[512]="hello world what is up; this is a test for writing to a floppy!";
typedef struct{
	unsigned char sector;
	unsigned char track;
	unsigned char head;
}drive_geometry;
extern void testtask();
TEXT_VIDEO_INFO test_video;


#include "test.c"
//the test function has got huge so I've put it in its own file



void c_info(char *argp){
	printf("Currently in kernel mode\n");
	printf("command seperation char is ");
	putc(sarg);
	printf("\n");

}



void help(char *argp){
	if(_strcmp(argp,"ver")){printf("--no args-- prints the version info to the screen\n");return;}
	if(_strcmp(argp,"info")){printf("--no args-- prints some useful info\n");return;}
	if(_strcmp(argp,"help")){printf("what your reading!\n");return;}
	if(_strcmp(argp,"cls")){printf("--no args-- clears the screen\n");return;}
	if(_strcmp(argp,"set info")){printf("not supported yet\n");return;}
	if(_strcmp(argp,"set video mode")){printf("--which mode-- sets video mode:\n");
	//this table won't look right in C but it will in the OS
	printf("+------------------------------+ \n");
	printf("|     Video modes:             |\n");
	printf("|______________________________|  \n");
	printf("|number|width|height|font width|\n");
	printf("+------+-----+------+----------+\n");
	printf("| 0    | 80  | 25   | 16       |\n");
	printf("| 1    | 90  | 60   | 8        |\n");
	printf("| 2    | 40  | 25   | 16       |\n");
	printf("| 3    | 40  | 50   | 8        |\n");
	printf("| 4    | 80  | 50   | 16       |\n");
	printf("| 5    | 90  | 30   | 16       |\n");
	printf("| g1   | 320 | 240  |graphics  |\n");
	printf("+------+-----+------+----------+\n");
	printf("g1 is actually just a test function\n");
	return;
	}
	if(_strcmp(argp,"textui")){printf("--start and end-- textui;start to start textui textui;end to end it\n");
	printf("press F6 to switch to main console and F7 to switch to small console\n");
	return;
	}
	if(_strcmp(argp,"resize")){printf("--no args-- resizes the small window, textui must be on\n");return;}
	if(_strcmp(argp,"move")){printf("--no args-- moves the small window, textui must be on\n");return;}
	if(_strcmp(argp,"terminal")){printf("--no args-- this makes a terminal over COM1\n");
          printf("The terminal settings on the remote pc should be ASCII, no parity, no flow control, data bits 8\n");
          printf("stop bits 1, data rate 96000\n");}

	printf("The current command seperator is ;\n");
	printf("2 commands on 1 line are supported\n");
	printf("To do this type <command name here>;<argument>;<next command>;<next arg;.. and so on\n");
	printf("to do a command you do this my_cmd;argument\n");
	printf("and to get 1 thing straight, spaces are allowed in a command\n");
	printf("Now for a list of commands\n");
	printf("test: no args yet; this is a testing function so it might be unstable\n");
	printf("ver:no args planned; this prints the versions of this OS\n");
	printf("info:no args yet; this just tells some useful information that can change\n");
	printf("help:command or none for list; what do you think you are reading!\n");
	printf("cls:no args planned; this clears the screen\n");
	printf("set info: currently not supported\n");
	printf("set video mode:arg is the mode number; This changes the video mode, currently 0-5 is all\n");
	printf("textui;start to make a new window, end to close it; shows a good example of TextUI\n");
	printf("move;no args;moves the small console, requires textui\n");
	printf("resize;no args;resizes the small console, requires textui\n");
	printf("terminal;no args;is a terminal for use over COM1, use help;terminal for more info\n");
	printf("\nType the command you need help with as an argument like\n");
	printf("this: help;COMMAND_YOU_NEED_HELP_WITH\n");

}







