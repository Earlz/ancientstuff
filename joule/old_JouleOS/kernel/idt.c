//See COPYRIGHT.txt for info on copyright
#include <k_misc.h>
#include <string.h>
#include <video.h>
#include <k_JouleOS.h>
#include <TextGL.h>
#include <mouse.h>
#include <stdio.h>




#include "fault_handlers.c"
#include "irqs.c"

//21





#define PIC1 0x20
#define PIC2 0xA0
#define ICW1 0x11
#define ICW4 0x01

void irq_remap(void) {
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}







void TaskInt(regs *r){
	printf("blah\n");
}
void start_console(unsigned char *start_message);
void ConsoleProc();
volatile unsigned char *kstack;
void StartTasking(regs *r){
     /*
	__asm("cli");
	printf("test");
	NewProcess("Idle Proc", "none", 3, 3, &IdleProc,1,IdleStack);
	NewProcess("Idle2","none",3,3,&ConsoleProc,1,&kstack);
	printf("\ncreated proccess\n");
	SwitchTask(r);
	SwitchTask(r);
	SwitchTask(r);
     multitasking=1;
	printf("got done\n");
	__asm("sti");
	//for(;;){}
	*/
}

volatile THREAD_STRUCT *testblah;
unsigned int threadblah1;
unsigned short threadblah2;
extern volatile THREAD_STRUCT *CurrentThread;
void ThreadSwitch(volatile regs *r){
     /* this does double stack switching..
     the first stack switch it should do is switch to get the right things

     when we switch stacks it actually will also update regs *r so no worries or well actually this causes problems..

     */

     //SwitchConsole(DebugCon); //for debugging...
     __asm("cli");


     SwitchThread(CurrentThread->next);

     //__asm("sti");
}




/* Defines an IDT entry */
struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;        /* Our kernel segment goes here! */
    unsigned char always0;     /* This will ALWAYS be set to 0! */
    unsigned char flags;       /* Set using the above table! */
    unsigned short base_hi;
}
__attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
}
__attribute__((packed));

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
struct idt_entry idt[512];
struct idt_ptr idtp;

/* This exists in 'start.asm', and is used to load our IDT */
extern void idt_load();


/* Use this function to set an entry in the IDT. Alot simpler
*  than twiddling with the GDT ;) */
void idt_set_gate(unsigned char num, void (*base), unsigned short sel, unsigned char flags) {
    unsigned long base2;
    base2=base; /*change to a non pointer to do maths*/
    idt[num].base_lo=base2 & 0xFFFF;
    idt[num].base_hi=(base2 >> 16) & 0xFFFF;
    idt[num].always0=0;
    idt[num].flags=flags;
    idt[num].sel=sel;
    /* We'll leave you to try and code this function: take the
    *  argument 'base' and split it up into a high and low 16-bits,
    *  storing them in idt[num].base_hi and base_lo. The rest of the
    *  fields that you must set in idt[num] are fairly self-
    *  explanatory when it comes to setup */
}

/* Installs the IDT *
void idt_install()
{byte i=19;
    /* Sets the special IDT pointer up, just like in 'gdt.c' *
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    /* Clear out the entire IDT, initializing it to zeros *
    memset(&idt, 0, sizeof(struct idt_entry) * 256);
 isrs_install();
 /*
    /* Add any new ISRs to the IDT here using idt_set_gate *
 idt_set_gate(0,int0h,16,0x8E);
 idt_set_gate(1,int1h,16,0x8E);
 idt_set_gate(2,int2h,16,0x8E);
 idt_set_gate(3,int3h,16,0x8E);
 idt_set_gate(4,int4h,16,0x8E);
 idt_set_gate(5,int5h,16,0x8E);
 idt_set_gate(6,int6h,16,0x8E);
 idt_set_gate(7,int7h,16,0x8E);
 idt_set_gate(8,int8h,16,0x8E);
 idt_set_gate(9,int9h,16,0x8E);
 idt_set_gate(10,int10h,16,0x8E);
 idt_set_gate(11,int11h,16,0x8E);
 idt_set_gate(12,int12h,16,0x8E);
 idt_set_gate(13,int13h,16,0x8E);
 idt_set_gate(14,int14h,16,0x8E);
 idt_set_gate(15,int15h,16,0x8E);
 idt_set_gate(16,int16h,16,0x8E);
 idt_set_gate(17,int17h,16,0x8E);
 idt_set_gate(18,int18h,16,0x8E);
 while (i<=31){
 idt_set_gate(i,int19_31h,16,0x8E);
 i++;
 }
 /*irq*
 irq_remap();
 idt_set_gate(32,irq0,16,0x8E);
 idt_set_gate(33,irq1,16,0x8E);
 idt_set_gate(34,irq2,16,0x8E);
 idt_set_gate(35,irq3,16,0x8E);
 idt_set_gate(36,irq4,16,0x8E);
 idt_set_gate(37,irq5,16,0x8E);
 idt_set_gate(38,irq6,16,0x8E);
 idt_set_gate(39,irq7,16,0x8E);
 idt_set_gate(40,irq8,16,0x8E);
 idt_set_gate(41,irq9,16,0x8E);
 idt_set_gate(42,irq10,16,0x8E);
 idt_set_gate(43,irq11,16,0x8E);
 idt_set_gate(44,irq12,16,0x8E);
 idt_set_gate(45,irq13,16,0x8E);
 idt_set_gate(46,irq14,16,0x8E);
 idt_set_gate(47,irq15,16,0x8E);
    /* Points the processor's internal register to the new IDT *
    idt_load();
}*/


void timer_phase(int hz) /*100 means 100 timer ticks a second*/
{
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outportb(0x43, 0x36);             /* Set our command byte 0x36 */
    outportb(0x40, divisor);   /* Set low byte of divisor */
    outportb(0x40, divisor >> 8);     /* Set high byte of divisor */
}


/* These are function prototypes for all of the exception
*  handlers: The first 32 entries in the IDT are reserved
*  by Intel, and are designed to service exceptions! */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irqm0();
extern void irqm1();
extern void irqm2();
extern void irqm3();
extern void irqm4();
extern void irqm5();
extern void irqm6();
extern void irqm7();
extern void irqs8();
extern void irqs9();
extern void irqs10();
extern void irqs11();
extern void irqs12();
extern void irqs13();
extern void irqs14();
extern void irqs15();

extern void _TaskInt();
extern void _StartTasking();

extern void _AppInt51();

void AppInt51(regs *r);

void idt_set_gate(unsigned char num, void (*base), unsigned short sel, unsigned char flags);
/* This is a very repetitive function... it's not hard, it's
*  just annoying. As you can see, we set the first 32 entries
*  in the IDT to the first 32 ISRs. We can't use a for loop
*  for this, because there is no way to get the function names
*  that correspond to that given entry. We set the access
*  flags to 0x8E. This means that the entry is present, is
*  running in ring 0 (kernel level), and has the lower 5 bits
*  set to the required '14', which is represented by 'E' in

for(i=0;i<32;i++){
         CreateInterrupt(i,0,0,0x10,0x08,NORMAL_INTERRUPT);
    } //fault handlers

*  hex. */
__asm("hlt");
void idt_install(void) {
     unsigned int i;
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    /* Clear out the entire IDT, initializing it to zeros */

    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    //exception handlers
    irq_remap();
    timer_phase(1000);

   /*FIGURED IT OUT!
   I am calling the isr0 which is the asm int stub code
   */


    idt_set_gate(0, (unsigned)isr0, 0x10, 0x8E);
    idt_set_gate(1, (unsigned)isr1, 0x10, 0x8E);
    idt_set_gate(2, (unsigned)isr2, 0x10, 0x8E);
    idt_set_gate(3, (unsigned)isr3, 0x10, 0x8E);
    idt_set_gate(4, (unsigned)isr4, 0x10, 0x8E);
    idt_set_gate(5, (unsigned)isr5, 0x10, 0x8E);
    idt_set_gate(6, (unsigned)isr6, 0x10, 0x8E);
    idt_set_gate(7, (unsigned)isr7, 0x10, 0x8E);
    idt_set_gate(8, (unsigned)isr8, 0x10, 0x8E);
    idt_set_gate(9, (unsigned)isr9, 0x10, 0x8E);
    idt_set_gate(11, (unsigned)isr11, 0x10, 0x8E);
    idt_set_gate(12, (unsigned)isr12, 0x10, 0x8E);
    idt_set_gate(13, (unsigned)isr13, 0x10, 0x8E);
    idt_set_gate(14, (unsigned)isr14, 0x10, 0x8E);
    idt_set_gate(15, (unsigned)isr15, 0x10, 0x8E);
    idt_set_gate(16, (unsigned)isr16, 0x10, 0x8E);
    idt_set_gate(17, (unsigned)isr17, 0x10, 0x8E);
    idt_set_gate(18, (unsigned)isr18, 0x10, 0x8E);
    idt_set_gate(19, (unsigned)isr19, 0x10, 0x8E);
    idt_set_gate(20, (unsigned)isr20, 0x10, 0x8E);
    idt_set_gate(21, (unsigned)isr21, 0x10, 0x8E);
    idt_set_gate(22, (unsigned)isr22, 0x10, 0x8E);
    idt_set_gate(23, (unsigned)isr23, 0x10, 0x8E);
    idt_set_gate(24, (unsigned)isr24, 0x10, 0x8E);
    idt_set_gate(25, (unsigned)isr25, 0x10, 0x8E);
    idt_set_gate(26, (unsigned)isr26, 0x10, 0x8E);
    idt_set_gate(27, (unsigned)isr27, 0x10, 0x8E);
    idt_set_gate(28, (unsigned)isr28, 0x10, 0x8E);
    idt_set_gate(29, (unsigned)isr29, 0x10, 0x8E);
    idt_set_gate(30, (unsigned)isr30, 0x10, 0x8E);
    idt_set_gate(31, (unsigned)isr31, 0x10, 0x8E);
    idt_set_gate(32, (unsigned)irqm0, 0x10, 0x8E);
    idt_set_gate(33, (unsigned)irqm1, 0x10, 0x8E);
    idt_set_gate(34, (unsigned)irqm2, 0x10, 0x8E);
    idt_set_gate(35, (unsigned)irqm3, 0x10, 0x8E);
    idt_set_gate(36, (unsigned)irqm4, 0x10, 0x8E);
    idt_set_gate(37, (unsigned)irqm5, 0x10, 0x8E);
    idt_set_gate(38, (unsigned)irqm6, 0x10, 0x8E);
    idt_set_gate(39, (unsigned)irqm7, 0x10, 0x8E);
    idt_set_gate(40, (unsigned)irqs8, 0x10, 0x8E);
    idt_set_gate(41, (unsigned)irqs9, 0x10, 0x8E);
    idt_set_gate(42, (unsigned)irqs10, 0x10, 0x8E);
    idt_set_gate(43, (unsigned)irqs11, 0x10, 0x8E);
    idt_set_gate(44, (unsigned)irqs12, 0x10, 0x8E);
    idt_set_gate(45, (unsigned)irqs13, 0x10, 0x8E);
    idt_set_gate(46, (unsigned)irqs14, 0x10, 0x8E);
    idt_set_gate(47, (unsigned)irqs15, 0x10, 0x8E);

    idt_set_gate(48, (unsigned)_TaskInt,0x10,0xEE);

    idt_set_gate(100,(unsigned)_StartTasking,0x10,0x8E);
    idt_set_gate(0x51,_AppInt51,0x10,0xEE);
    idt_load();

    //printf("t1");
     for(i=0;i<8;i++){
         CreateInterrupt(i,0,0,0x10,0x08,NORMAL_INTERRUPT);
    } //fault handlers
    //printf("t4");
    for(i=8;i<15;i++){
         CreateInterrupt(i,0,0,0x10,0x08,SPECIAL_INTERRUPT);
    }//a special interrupt with a prepushed error code
    for(i=15;i<32;i++){
         CreateInterrupt(i,0,0,0x10,0x08,NORMAL_INTERRUPT);
    }//even more fault handlers
    for(i=32;i<40;i++){
         CreateInterrupt(i,0,0,0x10,0x08,IRQM_INTERRUPT);
    }//master irq interrupts
    for(i=40;i<49;i++){
         CreateInterrupt(i,0,0,0x10,0x08,IRQS_INTERRUPT);
    }//slave irq interrupts
    //printf("t3");
    for(i=49;i<256;i++){
         CreateInterrupt(i,0,0,0x10,0x08,NORMAL_INTERRUPT);
    }//fill the rest of the itnerrupts
    __asm("hlt");
    // printf("t2");
    //now set the fault and a few irq hooks
    InstallInterruptHook(0,int0h);
    InstallInterruptHook(1,int1h);
    InstallInterruptHook(2,int2h);
    InstallInterruptHook(3,int3h);
    InstallInterruptHook(4,int4h);
    InstallInterruptHook(5,int5h);
    InstallInterruptHook(6,int6h);
    InstallInterruptHook(7,int7h);
    InstallInterruptHook(8,int8h);
    InstallInterruptHook(9,int9h);
    InstallInterruptHook(10,int10h);
    InstallInterruptHook(11,int11h);
    InstallInterruptHook(12,int12h);
    InstallInterruptHook(13,int13h);
    InstallInterruptHook(14,int14h);
    InstallInterruptHook(15,int15h);
    InstallInterruptHook(16,int16h);
    InstallInterruptHook(17,int17h);
    InstallInterruptHook(18,int18h);
    for(i=19;i<32;i++){
         InstallInterruptHook(i,int19_31h);
    }
    InstallInterruptHook(32,irq0);
    InstallInterruptHook(33,irq1);
    InstallInterruptHook(34,irq2);
    InstallInterruptHook(35,irq3);
    InstallInterruptHook(36,irq4);
    InstallInterruptHook(37,irq5);
    InstallInterruptHook(38,irq6);
    InstallInterruptHook(39,irq7);
    InstallInterruptHook(40,irq8);
    InstallInterruptHook(41,irq9);
    InstallInterruptHook(42,irq10);
    InstallInterruptHook(43,irq11);
    InstallInterruptHook(44,irq12);
    InstallInterruptHook(45,irq13);
    InstallInterruptHook(46,irq14);
    InstallInterruptHook(47,irq15);
    //now that I have vital fault handlers and irq's
    //install some of those extras..
    InstallInterruptHook(0x51,AppInt51); //later change this to a ring3 interrupt
    InstallInterruptHook(0x60,ThreadSwitch);

}

typedef void (*interrupt)(regs *);

unsigned char special_int_template[]={
     //an error code is already pushed...
     0x6A,0, //push byte [int_num] -byte 3 should be changed to interrupt number
     0xE9,0,0,0,0 //jmp [int_stub] -byte 5 through 8 should be changed to address of interrupt stub
};


/*unsigned char int_template[]={ //a normal interrupt template
     0x6A,0, //push byte 0 -dummy error code
     0x6A,0, //push byte [int_num] -byte 3 should be changed to interrupt number
     //0xE9,0,0,0,0 //jmp [int_stub] -byte 5 through 8 should be changed to address of interrupt stub

     //the interrupt stub



};*/

extern volatile unsigned char int_template[0x31];
extern volatile unsigned char irqm_template[0x35];
extern volatile unsigned char irqs_template[0x37];
typedef void (*interrupt_func)(regs *);
volatile interrupt_func interrupt_hooks[256];
volatile unsigned char *interrupt_stubs[256];


void int_stub();

#define NORMAL_INTERRUPT 1 // a normal interrupt
#define SPECIAL_INTERRUPT 0 //a special interrupt which means no dummy error code
#define IRQM_INTERRUPT 2 //a normal interrupt but with a irqm stub
#define IRQS_INTERRUPT 3  //same as above except with irqs stub


BOOL InstallInterruptHook(unsigned char num,void *func){
     if((volatile)interrupt_hooks[num]==0){
          interrupt_hooks[num]=func;
          return 1;
     }else{
          return 0;
     }
}

void UninstallInterruptHook(unsigned char num){
     interrupt_hooks[num]=0;
}



void CreateInterrupt(unsigned char num,void *func,unsigned char ring, unsigned short code_segment,unsigned short data_segment,unsigned char type){ //for intitial idt setup only
     void *templ;
     unsigned char *int_use;
     unsigned short *tmp;
     switch(type){
          case NORMAL_INTERRUPT:
          int_use=malloc(sizeof(int_template)+20);

          memcpy(int_use,int_template,sizeof(int_template)+20); //copy the template to the new data area
          idt_set_gate(num, int_use ,code_segment,0x8E | (ring<<5)); //set the idt gate
          int_use[3]=num; //change the interrupt number
          tmp=&int_use[0xD];
          *tmp=data_segment;  //change the segment used
          //maybe done
          break;
          case SPECIAL_INTERRUPT:
          int_use=malloc(sizeof(int_template)+20);

          memcpy(int_use,int_template+2,sizeof(int_template)+20); //copy the template to the new interrupt data area
          idt_set_gate(num, int_use ,code_segment,0x8E | (ring<<5)); //set the idt gate
          int_use[1]=num; //change the interrupt number
          tmp=&int_use[0xB];
          *tmp=data_segment;  //change the segment used
          break;

          case IRQM_INTERRUPT:
          int_use=malloc(sizeof(irqm_template)+30);

          memcpy(int_use,irqm_template,sizeof(irqm_template)+30);
          idt_set_gate(num, int_use ,code_segment,0x8E | (ring<<5)); //set the idt gate
          int_use[3]=num; //change the interrupt number
          tmp=&int_use[0xD];
          *tmp=data_segment;  //change the segment used
          break;
          case IRQS_INTERRUPT:
          int_use=malloc(sizeof(irqs_template)+20);

          memcpy(int_use,irqs_template,sizeof(irqs_template)+20);
          idt_set_gate(num, int_use ,code_segment,0x8E | (ring<<5)); //set the idt gate
          int_use[3]=num; //change the interrupt number
          tmp=&int_use[0xD];
          tmp=data_segment;  //change the segment used
          break;

     }
     //printf("k");
     interrupt_stubs[num]=int_use;
     InstallInterruptHook(num,func);


}



void int_catcher(regs *r){
     //printf("blah!\n");
     //print_hex(r->eip);
     //print_hex(r->cs);
    //printf("\n");
     //print_hex(r->int_no);

     if(r->int_no==67){printf("w00t");}
     if((volatile)interrupt_hooks[r->int_no]==0){
          if((volatile)interrupt_hooks[15]!=0){
               interrupt_hooks[15](r);
          }
     }else{
          interrupt_hooks[r->int_no](r);
     }
     //printf("\n");
     //print_hex((volatile)interrupt_hooks[r->int_no]);

}



    // waitkey();








/* All of our Exception handling Interrupt Service Routines will
*  point to this function. This will tell us what exception has
*  happened! Right now, we simply halt the system by hitting an
*  endless loop. All ISRs disable interrupts while they are being
*  serviced as a 'locking' mechanism to prevent an IRQ from
*  happening and messing up kernel data structures */
void fault_handler(regs *r) {
    // __asm("sti");
    switch(r->int_no) {
        case 0:
        int0h(r);
        break;
        case 1:
        int1h(r);
        break;
        case 2:
        int2h(r);
        break;
        case 3:
        int3h(r);
        break;
        case 4:
        int4h(r);
        break;
        case 5:
        int5h(r);
        break;
        case 6:
        int6h(r);
        break;
        case 7:
        int7h(r);
        break;
        case 8:
        int8h(r);
        break;
        case 9:
        int9h(r);
        break;
        case 10:
        int10h(r);
        break;
        case 11:
        int11h(r);
        break;
        case 12:
        int12h(r);
        break;
        case 13:
        int13h(r);
        break;
        case 14:
        int14h(r);
        break;
        case 15:
        int15h(r);
        break;
        case 16:
        int16h(r);
        break;
        case 17:
        int17h(r);
        break;
        case 18:
        int18h(r);
        break;
        case 32:
        irq0(r);
        break;
        case 33:
        irq1(r);
        break;
        case 34:
        irq2(r);
        break;
        case 35:
        irq3(r);
        break;
        case 36:
        irq4(r);
        break;
        case 37:
        irq5(r);
        break;
        case 38:
        irq6(r);
        break;
        case 39:
        irq7(r);
        break;
        case 40:
        irq8(r);
        break;
        case 41:
        irq9(r);
        break;
        case 42:
        irq10(r);
        break;
        case 43:
        irq11(r);
        break;
        case 44:
        irq12(r);
        break;
        case 45:
        irq13(r);
        break;
        case 46:
        irq14(r);
        break;
        case 47:
        irq15(r);
        break;


        case 48:
        TaskInt(r);
        break;

        case 100:
        StartTasking(r);
        break;

        case 0x51:
        AppInt51(r);
        break;


        default:
        printf("t");
        if (r->int_no>18 && r->int_no<30) {
            int19_31h(r);
        }
        else {
            unknown_int(r);
        }
        break;
    }

    return;
}