//See COPYRIGHT.txt for info on copyright
#include <k_JouleOS.h>
#include <debug.h>
#include <stdio.h>
typedef struct{
     unsigned int prev_task, //not used for my task sytstem
     ESP0,SS0, //stack for ring0 interrupts
     ESP1,SS1, //stack for ring1
     ESP2,SS2, //stack for ring2
     CR3, //page table address
     EIP,EFLAGS, //eip and flags
     EAX,ECX,EDX,EBX,// These are
     ESP,EBP,ESI,EDI,// general registers
     ES,CS,SS,DS,FS,GS, //segment registers
     LDT_selector, //segement selector for ldt
     IO_map; //iomap i think for v86
}TSS_STRUCT;

volatile TSS_STRUCT TSS;

volatile unsigned int temp_tss;

extern unsigned char kstack_012[4096];

unsigned char halt_int=0xF4;


extern void *TSS_jmpback;
void InitTSS(){
	gdt_set_gate(3,0,0xFFFFFFFF,0xF3,0xC0); //the stack segment for below ring2
     memset(&TSS,0,sizeof(TSS_STRUCT)); //0 it all
     TSS.ESP0=kstack_012;
     TSS.ESP1=kstack_012;
     TSS.ESP2=kstack_012;
     TSS.SS0=0x18;
     TSS.SS1=0x18;
     TSS.SS2=0x18;
     TSS.EIP=&TSS_jmpback;
     __asm(".intel_syntax noprefix\n"
     "pushf\n"
     "pop [_temp_tss]\n"
     ".att_syntax\n");
     TSS.EFLAGS=temp_tss;
     __asm(".intel_syntax noprefix\n"
     "mov [_temp_tss],esp\n"
     ".att_syntax\n");
     TSS.ESP=temp_tss; //were in a call so we can't just set the stack to 0
     TSS.EBP=TSS.ESP;
     TSS.ES=0x08;
     TSS.CS=0x10;
     TSS.SS=0x8;
     TSS.DS=8;
     TSS.FS=8;
     TSS.GS=8;

     gdt_set_gate(4,&TSS,20,0xEB,0x90); //our special TSS selector
     temp_tss=0x20;
     printf("t");
     __asm(".intel_syntax noprefix\n"
     "mov  eax,_temp_tss\n"
    "LTR [eax]\n"
     ".att_syntax\n");
     //LTR(4*8);
     printf("t");

     gdt_set_gate(5,0,0xFFFFFFFF,0xFB, 0xD0); //our ring3 testing code segment
     gdt_set_gate(6,0,0xFFFFFFFF, 0xF3, 0xD0);//our ring3 testing data/stack segment
     k_printf("!");
     SendByteCom1('a');
     __asm(".intel_syntax noprefix\n"
     "mov ax,0x8\n"
     "mov ds,ax\n"
     "mov ss,ax\n"
     "push 0x30\n"
     "push esp\n"
     "pushf\n"
     "push 0x28\n"
     "push _halt_int\n"
     "iret\n"
     "cli\n"
     "hlt\n"
     ".att_syntax\n");


}


     __asm(".intel_syntax noprefix\n"
     "_TSS_jmpback:\n"
     "hlt\n"
     ".att_syntax\n");

