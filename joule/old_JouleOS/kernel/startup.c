//See COPYRIGHT.txt for info on copyright
/**************
JouleOS Version 0.0.8
With JouleCon version 0.0.8
Release Version 1

requires an i486 and at least 4mb of memory
**************/









void _main(void);

//NOTHING ABOVE THIS!!!!
void startit(void){ //this is the kernel entry point
__asm("cli");
__asm(".intel_syntax noprefix\n"
"mov eax,0x10000B\n"
"jmp eax\n"
"jmp _asm_start \n"
".att_syntax\n");
}




#include <stdio.h>
#include <k_misc.h>
#include <k_JouleOS.h>
#include <string.h>
#include <video.h>
#include <FDD.h>
volatile unsigned int int_stack; //shouldn't need more than that

unsigned char kstack[4096];
unsigned char kstack_012[4096]; //for interrupts at ring0 and in tss


void testtask(){
	__asm(
	".intel_syntax noprefix\n"
	"hlt\n"
	".att_syntax \n");
}
extern void StartTSS();



unsigned int zero=0;
extern unsigned char debug_tmp;
unsigned char *extra_buffer;
extern unsigned int TS;
extern void NewTask(void *padr,unsigned int size,void *stack,unsigned int stack_size);
void _main(void){
	char *tmp[3];char *tmpe;
     gdt_install();


     init_memman();

     idt_install();
     //__asm("cli");
     //__asm("hlt");

     StartConsole();
     //printf("hi");
     //RefreshConsole();
     //tmp[0]=4/0;
     //__asm("hlt");
     putc('h');
     //->vptr=AllocCore(3);
     cls();
     //sys_buffer=AllocCore(3);
     cls();
     //set the cursor to on
     outportb(0x3D4,0xA);
     outportb(0x3D5,0x1F);
     //set_video_mode_s("4");

     printf("System tables:");
     JouleCon->curx=60;JouleCon->color=GREEN;printf("[OK]");
     JouleCon->cury++;JouleCon->curx=0;JouleCon->color=0x0F;
    outportb(0x21,0); //enable all irq's  they will be sent to blank interrupts if unsupported
     printf("Memory Manager:");
     JouleCon->curx=60;JouleCon->color=GREEN;printf("[OK]");
     JouleCon->cury++;JouleCon->curx=0;JouleCon->color=0x0F;
     InitEventDrive();
     printf("Event Drive:");
     JouleCon->curx=60;JouleCon->color=GREEN;printf("[OK]");
     JouleCon->cury++;JouleCon->curx=0;JouleCon->color=0x0F;
     InitFDD(1);
     Init_ComPort1(1);
     //mouse_install();
     printf("PS/2 Mouse:");
     JouleCon->curx=60;JouleCon->color=GREEN;printf("[OK]");
     JouleCon->cury++;JouleCon->curx=0;JouleCon->color=0x0F;
     printf("Cpu identification: \n");
     cpu();
     printf("\n");
     start_console(0);
	for(;;){}
}

/*
_________________
|
|
_________________
*/
