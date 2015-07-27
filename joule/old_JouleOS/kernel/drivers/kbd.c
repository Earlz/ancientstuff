//See COPYRIGHT.txt for info on copyright
#include <k_JouleOS.h>
#include <event_drive.h>

unsigned char kbdus[128] =
    {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
        '9', '0', '-', '=', '\b', /* Backspace */
        '\t',   /* Tab */
        'q', 'w', 'e', 'r', /* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
        0,   /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
        '\'', '`',   0,  /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',   /* 49 */
        'm', ',', '.', '/',   0,    /* Right shift */
        '*',
        0, /* Alt */
        ' ', /* Space bar */
        0, /* Caps lock */
        0, /* 59 - F1 key ... > */
        0,   0,   0,   0,   0,   0,   0,   0,
        0, /* < ... F10 */
        0, /* 69 - Num lock*/
        0, /* Scroll Lock */
        0, /* Home key */
        0, /* Up Arrow */
        0, /* Page Up */
        '-',
        0, /* Left Arrow */
        0,
        0, /* Right Arrow */
        '+',
        0, /* 79 - End key*/
        0, /* Down Arrow */
        0, /* Page Down */
        0, /* Insert Key */
        0, /* Delete Key */
        0,   0,   0,
        0, /* F11 Key */
        0, /* F12 Key */
        0, /* All other keys are undefined */
    };

unsigned char kbduscaps[128] =
    {
        0,  27, '!', '@', '#', '$', '%', '^', '&', '*', /* 9 */
        '(', ')', '_', '+', '\b', /* Backspace */
        '\t',   /* Tab */
        'Q', 'W', 'E', 'R', /* 19 */
        'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
        0,   /* 29   - Control */
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
        '"', '~',   0,  /* Left shift */
        '|', 'Z', 'X', 'C', 'V', 'B', 'N',   /* 49 */
        'M', '<', '>', '?',   0,    /* Right shift */
        '*',
        0, /* Alt */
        ' ', /* Space bar */
        0, /* Caps lock */
        0, /* 59 - F1 key ... > */
        0,   0,   0,   0,   0,   0,   0,   0,
        0, /* < ... F10 */
        0, /* 69 - Num lock*/
        0, /* Scroll Lock */
        0, /* Home key */
        0, /* Up Arrow */
        0, /* Page Up */
        '-',
        0, /* Left Arrow */
        0,
        0, /* Right Arrow */
        '+',
        0, /* 79 - End key*/
        0, /* Down Arrow */
        0, /* Page Down */
        0, /* Insert Key */
        0, /* Delete Key */
        0,   0,   0,
        0, /* F11 Key */
        0, /* F12 Key */
        0, /* All other keys are undefined */
    };
typedef struct {
    unsigned char caps;
    unsigned char shift;
    unsigned char scroll;
    unsigned char num;
    unsigned char ctrl;
    unsigned char alt;
}
shift_locks; /*for simplicity and speed*/
volatile shift_locks shifts;
volatile unsigned int keycount=0;
volatile unsigned char keys[128];


unsigned char scan2asc(unsigned char scan){
     if (shifts.caps^shifts.shift==1) {
          scan=kbduscaps[scan];
     }else{
     	scan=kbdus[scan];
     }
     return scan;
}



void put_kbd_buffer(unsigned char scan,unsigned char chr){
	keys[keycount]=chr;
	keycount++;
	keys[keycount]=scan;
     keycount++;
}


unsigned char get_kbd_buffer(unsigned char *scan_buf,unsigned char *asc_buf){
	keycount--;
     *scan_buf=keys[keycount];
	keycount--;
	*asc_buf=keys[keycount];
	return *asc_buf;
}


char getkey(char *buf){
	//char tmp[1]; error! allocated on stack

	while(keycount==0){}
	get_kbd_buffer(&buf[0],&buf[1]);
	return buf[1];
}
char trash_whatever[2];
void waitkey(){
	while(keycount==0){}
	get_kbd_buffer(&trash_whatever[0],&trash_whatever[1]);
}


unsigned int kbd_temp[5];

void ConsoleReset(){
	start_console("Console has been reset");
}

/*F6-F10 are reserved for window functions and are not useable in applications*/
//extern int TextUI;
unsigned char f12_counts=0;
void kbd_handler(regs *r){
	unsigned char scancode;unsigned char tmp;
	void *prev;char tmpy[2];
	if (keycount==128){return 0;}
     scancode = inportb(0x60);
     if (scancode & 0x80) { //this i thought meant that the key was released
          switch (scancode) {
               case 42|0x80:
               shifts.shift=0;
               break;
               case 54|0x80:
               shifts.shift=0;
               break;
               case 29|0x80:
               shifts.ctrl=0;
               break;
               case 56|0x80:
               shifts.alt=0;
               break;
          }
     }else{

     switch (scancode){
     	case 42:
     	shifts.shift=1;
     	break;
     	case 54:
     	shifts.shift=1;
     	break;
     	case 29:
     	shifts.ctrl=1;
     	break;
     	case 56:
          shifts.alt=1;
          break;
          case 58:
          shifts.caps=shifts.caps ^ 1;
          break;
          case 69:
          shifts.num=shifts.num ^ 1;
          break;
          case 70:
          shifts.scroll=shifts.scroll ^ 1;
          break;

     }
     if (scancode==0x3E){ //F4-- hang recover, or at least try

               	r->eip=&ConsoleReset;
     }
     if(scancode==0x3F){ //F5-- switch to previous console
          SwitchConsole_int(CurrentConsole->prev);
     }
     if(scancode==0x40){//F6-- switch to next console

          SwitchConsole_int(CurrentConsole->next);
     }






    /* if (TextUI==1){ //ignore this
     	switch(scancode){
     		case 0x40: //f6

     		break;
     		case 0x41: //f7

     		break;
     		case 0x42:

     		break;
     		case 0x43:

     		break;
     		case 0x44:

     		break;
     	}
     	//call_evh(mcon,7,(scancode<<8)+scan2asc(scancode)); //modification for ring3?
     	//return;

     }*/

     tmp=scan2asc(scancode);
     CallEvent(KEYBOARD_EVENT,(scancode<<8)|tmp,0);
     /*
     kbd_temp[0]=r->eip;
     kbd_temp[1]=r->cs;
     kbd_temp[2]=r->ds;
     kbd_temp[3]=scancode;
     kbd_temp[4]=tmp;
     r->eip=&kbd_call;
     r->cs=0x10;
     r->ds=0x8;
     */

     }

}

/*

void kbd_call(){

	__asm(".intel_syntax noprefix\n"
	"sub esp,4\n"
	"jmp

*/



















