/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>Version 0.0.3 ALPHA<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>Shell\Main.c; Contains the (unnamed)shell<<<<*/
/*>>>>Shell Extension Version 0.1.0<<<<*/

/*making arrays of pointers is useless, change it*/

#ifndef SHELL
#define SHELL
#define SHELL_VERSION 0.0.4
#define SHELL_VERSIONs "0.0.4"
char command[scanf_BUFFER];
#include "shell\interf.c"
#include "shell\notes.c"
#endif
void Shelp(void);
void Load_lowbasic(void);
void Scheck_scroll(void);
void testing(void);
void shtime(void);
void test2(void);
void sfdfs(void);
void ver(void);
void notes(void);
void sload(void);
void Sview(void);
void checkmem(void);
void ssetvid(void);
void bdraw(void);


byte docmd(void)
{/*something great to do would be to assign this to a memory area and then have it behave like an interrupt*/
	byte didit=0;
	if (streq(command,"help")==1) {Shelp(); didit=1;}
	if (streq(command,"ver")==1){ver();didit=1;}
	if (streq(command,"cls")==1){cls();didit=1;}
	if (streq(command,"reboot")==1){ reboot(); didit=1;}
	if (streq(command,"basic")==1){ Load_lowbasic(); didit=1;}
	if (streq(command,"test")==1){testing(); didit=1;}
	if (streq(command,"time")==1){shtime(); didit=1;}
	if (streq(command,"test2")==1){test2(); didit=1;}
	if (streq(command,"format")==1){sformat(); didit=1;}
	if (streq(command,"notes")==1){notes();didit=1;}
	if (streq(command,"load")==1){sload();didit=1;}
	if (streq(command,"fdfs")==1){sfdfs();didit=1;}
	if (streq(command,"view")==1){Sview();didit=1;}
	if (streq(command,"setv")==1){ssetvid(); didit=1;}
	if (streq(command,"draw")==1){bdraw();didit=1;}
	if (didit==0) {
		printf("Command not recognized!");
	}
	didit=0;
}

signed int SMain(char* Arguments)  /*The big shell main; Arguments is currently not used and is there for future use; can return an error value*/
{
	char tmp[32];
	byte didit=0;
	tmp[0]=0;
	tmp[1]=0;
	init(tmp);
	tmp[0]=1;
	printf("JuleOS version ");printf(JULEOS_VERs); printf(" pre-alpha");
	while (tmp[0]=1) {
		nl();
		printf("->:");
		input(&command);
		docmd();

Scheck_scroll();


}
}

signed int init(char args[32]) /*shell initalization args is currently unused*/
{
	setvideomode(0x02);
	if (args[0]==1){
		setvideomode(0x02);}
		if (args[0]>1) { return -1;}
	setvideopage(args[1]);
	setvideomode(0x03);
	checkmem();
}

void Shelp(void) /*displays help message*/
{
			printf("basic -Starts the basic interpreter -not included in this version");
			nl();
			printf("help -Displays this text");
			nl();
			printf("reboot -Reboots the PC -beta");
			nl();
			printf("cls -clears the screen");
			nw
			printf("format -formats a floppy disk using FDFS -beta");
			nw
			printf("time -displays current time -alpha");
			nw
			printf("ver -displays version information");
			nw
			printf("test -a reserved testing command to prevent clutter -use at own risk");
			nw
			printf("test2 -another test function so you can work on 2 things at one time");
			nw
			printf("load -loads up to 18 sectors(4kb) from a floppy and jumps to it");nw
			printf("notes -loads up the Notes 0.0.3 diagnostic program(use at own risk)");
			nw printf("draw -starts the draw program;consult the readme.txt for help");
			
}

void Load_lowbasic(void) /*probably wont be here for long*/
{
	printf("Currently under development."); nl(); printf("Remeber this is an alpha!");
}

void Scheck_scroll(void) /*checks the scrolling and if it is down to where u cant see the cursor it will cls*/
{
	if (getcury()>20) {
		cls();
	}
}
void test4(void){
	cls;
}
void testing(void) /*just a reserved function for testing junk*/
{
byte i=1;
byte x=0;
byte y=0;
word e=0;
int t,n;
n=inputn();

while (bioskey(1)==0)
			{
				if (bioskey(1)>0){break;}



	printa(x,y,"JuleOS");
	x++;
	if (x==80){x=0;y++;}
	if (y==25){x=0;y=0;return 0;}
	while (t<=n){
	while (e<=10000){e++;}e=0;t++;
}t=0;
cls();
	e=0;	
	

}
}

void shtime(void)
{
long ticks;
long tmp,tmp2;
byte buffer[20];byte *minutess,*hourss;
int hours;
int minutes;
asm hi:
ticks=biostime(0,0);
tmp=ticks/18;
hours=tmp/60; /*get hours*/
minutes=tmp%60;
minutess= itoa(minutes,*buffer,10); /*problem with this, not the time*/
hourss=itoa(hours,*buffer,10);
printf(hourss);nw
printf(minutess);nw

}
byte isset(word val,byte bit) /*fucked up turboc odnt liek for something useful to be in a function*/
{
	          char buffer[20];
          char *str;

buffer[19]=itoa(125,&buffer,10);
if (buffer[0]=='1') {printf("yay");}
printf(buffer);

}
void test2(void)
{	
	pokeb32(0x000b800,'h');
         

}

void printequip(void)
{
	char buffer[20];
	char tmp[20];
	word equip;
	equip=biosequip();		
	buffer[19]=itoa(equip,&buffer,2);
	
}
/*first byte is characters, second is attribs*/




void num2asci(word number)
{
}

void ver(void)
{
	printf("Shell version: ");printf(SHELL_VERSIONs); nw
	printf("JuleOS version: ");printf(JULEOS_VERs);
}

void sload(void){
	char buffer[512];
	char input=0;
	word i=0,next=0,i2=0;
	printf("this will load sectors from a floppy disk");nw
	printf("the interface is poor so it will load 18 sectors from cylinder 50");nw
	printf("so here we go, this can take a few seconds");nw
	asm push es
	asm pusha
	asm mov ah,02h
	asm mov al,18
	asm mov ch,50
	asm mov cl,1
	asm mov dh,0
	asm mov dl,0
	asm mov bx,2000h
	asm mov es,bx
	asm mov bx,0
	asm int 13h
	asm popa
	asm pop es
printf("done loading, now jumping");nw
asm include asm\tmp1.asm
}
	
void sfdfs(void){
	char *tmp[16];
	tmp[0]='h';tmp[1]='i';
	/*cacheindex(0);
	write_entry(1,&tmp);*/
}
void Sview(void){
	byte tmp;
}

void checkmem(void){/*makes sure the system has enough memory for JuleOS*/
word t;
t=memsize();
if (t<4096){printf("Your system does not meet the memory requirements for JuleOS");nw printf("you need at least 4mb of total memory to run JuleOS");waitkey reboot();}
	total_memory=t;
	
}


void ssetvid(void){
	byte t;
	t=inputn();
	while (t!=255){

	setvideomode(t);
		
		putdot(t,3,RED);printf("1");
		waitkey
		t++;
	}
}

void bdraw(void){
	byte color=WHITE;
	word x=1,y=1,tx,ty;
	keys it;byte going=1;
	byte no_draw=0;
	byte c2=0;
	setvideomode(0x13);
		while(going==1){
			tx=x;
			ty=y;
			c2=color;
			while (bioskey(1)==0)
			{			
				if (bioskey(1)>0){break;}
			}
			it=getkey();
					switch(it.scan) /*setup key trap*/
		{
			case 72: /*up*/
			if (y!=1){y--;}
			break;
			case 80: /*down*/
			if (y!=200){y++;}
			break;
			case 75: /*left*/
			if (x!=1){x--;}
			break;
			case 77: /*right*/
			if (x!=320){x++;}
			break;
			}
			switch(it.asc){
				case 'r':
				color=RED;
				break;
				case 'g':
				color=GREEN;
				break;
				case 'b':
				color=BLUE;
				break;
				case 'e':
				setvideomode(0x03);
				return 0;
				break;
				case 'c':
				setvideomode(0x13);
				break;
				case 'w':
				color=WHITE;
				break;
				case 'n':
				if (no_draw==1){no_draw=0;}else{no_draw=1; }
				break;
				case 's':
				segsave(0xa000,100);
				break;
				case 'l':
				segrestore(0xa000,100);
				break;

			}


			if (no_draw==0){
			putdot(x,y,color);
			}else{
			putdot(tx,ty,BLACK);
			putdot(x,y,WHITE);
			}





		}
	

setvideomode(0x03);
}




/*

number to ascii numeric character converter

; ax = unsigned integer
; ax, at most, is a 5-digit number
mov si, outStr+4 ; point si at the right-most char of outStr
mov cx, 10 ; the base
xor dx, dx ; zero out dx as dx:ax will be divided by cx
LP:
div cx
; ax = Quotient ; dx = Remainder < cx(=10), so only dl matters
add dl, 30h ; convert a binary value to its ASCII numeric digit
mov [si], dl ; store this ASCII numeric digit to outStr
dec si ; point si to the next char (to the left)
xor dx, dx ; zero out dx (to prepare dx:ax for the next div)
or ax, ax ; check if ax is already zero
jnz LP ; if not, next loop; if yes, quit
inc si ; cancel out the effect of the last "dec si"
; so that si now points at the first (the left-most) char
; of the number to be printed
int 21h
...
outStr db '01234', 0 ; output string for a 5-digit numbe*/

	


/*
library of successful function tests
void test2(void)
{
            #include <stdlib.h>

            int bas2 = 2;
            int bas8 = 8;
            int bas16 = 16;
            char *string = "5";
            long longint;
            char *termn;
                longint = strtol(string,&termn,10);
				if (longint==5){printf("yay");} /*i bet i could use this function for bios equip checks

}

void test2(void)
{
 byte a; 
 pokeb(0xfa,0x01,24);
     a=peekb(0xfa,0x01);
     if (a==24) {printf("w00t");}  /*i spy... something useful ,vga: A000 segment

}




unworking

 byte a; 
 dword offset;
 setvideomode(0x12);
 offset = 640 * 5 + 5;
 pokeb(0xA000 ,offset,0xff);
*/
	