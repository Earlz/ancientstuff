//See COPYRIGHT.txt for info on copyright
/*included here is everything including:
all system table crap
console stuff
memory manager stuff
asm functions and variables including stack pointer
*/

#include <mytypes.h>

#ifndef _K_JOULEOS_H
#define _K_JOULEOS_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	unsigned char wait_on_scroll; //1 for everytime a scroll down happens it waits for a key

}console_ops_struct;
void waitkey();
extern console_ops_struct c_ops;
extern volatile char cmdbuf[256];
extern volatile unsigned long timer_ticks;

extern volatile unsigned long time_slice;

#define PRIORITY0 1 //gets a 1ms timeslice
#define PRIORITY1 5
#define PRIORITY2 7
#define PRIORITY3 10 //the default
#define PRIORITY4 15
#define PRIORITY5 25
#define PRIORITY6 40
#define PRIORITY7 100 //the highest

extern volatile unsigned int *IdleStack;
extern volatile BOOL multitasking;













/*
extern isr0()
extern isr1()
extern isr2()
extern isr3
extern isr4
extern isr5
extern isr6
extern isr7
extern isr8
extern isr9
extern isr10
extern isr11
extern isr12
extern isr13
extern isr14
extern isr15
extern isr16
extern isr17
extern isr18
extern isr19
extern isr20
extern isr21
extern isr22
extern isr23
extern isr24
extern isr25
extern isr26
extern isr27
extern isr28
extern isr29
extern isr30
extern isr31
*/
#include <video.h>
#include <mytypes.h>
typedef void (*EvhFunc)(unsigned char,unsigned int);


extern volatile unsigned int TextUI;
//extern volatile video_info def_console;
extern volatile unsigned long long ticks;
extern unsigned char floppy_intd;

/*event names*/
/*these are always enabled when there is a window*/
#define EV_UNSET 0
#define EV_GOT_FOCUS 1
#define EV_LOST_FOCUS 2
#define EV_MINIMIZED 3
#define EV_RESTORED 4
#define EV_MAXIMIZED 5
#define EV_CLOSE 6




void print_hex(unsigned int dat);
void k_print_hex(unsigned int dat);






extern volatile unsigned long timer1; //usable
extern volatile unsigned long timer2; //usable
extern volatile unsigned long timer3; //usable
extern volatile unsigned long timer4; //only for wait()!
extern volatile BOOL fdd_done;
extern void (*timer1_callback)(unsigned long timer_ticks);

typedef struct
{
    unsigned int ds, es, fs, gs;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
}
regs;

typedef struct{
	unsigned int eax,ecx,edx,ebx,esp,ebp,esi,edi;
}registers
__attribute__((packed));

typedef struct{
	unsigned int ds,es,fs,gs; //these must be int because it is a 32bit stack
}segments
__attribute__((packed));
struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity; //has limit in tss
    unsigned char base_high;
}
__attribute__((packed));

typedef struct{
     unsigned int eip,cs,eflags,esp,ss; //iret
     unsigned int ds;
     registers gregs;
}Task
__attribute__((packed));
extern volatile unsigned int int_stack;
extern volatile unsigned long timer_ticks;
extern volatile Task *tasks; //Alloc page here for 73 tasks
void TaskSwitch(Task old_task);
void NewTask(void *padr,unsigned int size,void *stack,unsigned int stack_size);
#ifdef DEBUG
#define GDT_LIMIT 0xFF //so bochs actually gives me the error not just invalid selector
#else
#define GDT_LIMIT 100
#endif
extern struct gdt_entry gdt[GDT_LIMIT];
BOOL InstallInterruptHook(unsigned char num,void *func);
void CreateInterrupt(unsigned char num,void *func,unsigned char ring, unsigned short code_segment,unsigned short data_segment,unsigned char type);
void UninstallInterruptHook(unsigned char num);


#define NORMAL_INTERRUPT 1 // a normal interrupt
#define SPECIAL_INTERRUPT 0 //a special interrupt which means no dummy error code
#define IRQM_INTERRUPT 2 //a normal interrupt but with a irqm stub
#define IRQS_INTERRUPT 3  //same as above except with irqs stub



#define INT_INDEX 0x2000
#define INT_SUB_INDEX 0x2002
#define INT_DATA_AREA 0x2004


struct CurCtrl_0_1{ //naming format is INDEX NAME_SUB INDEX'S THAT USE THIS
	signed int x,y;
}__attribute__((packed)); //so its not aligned
void kbd_handler(regs *r);




typedef struct{
     void(*LastTick)(void); //is called in the last tick of the thread.. not called in lowest priority
     void(*FirstTick)(void); //called in first tick of the thread
     void(*Error1)(unsigned int); //this is called when their is an unfixable error
}blah12334;





typedef struct{
     void *prev;
	char name[33];
	unsigned int TID; //a thread ID
	//maybe having cs and ss a constant...?
	//and then setting and getting cs and ss
	unsigned int other_esp; //this is used for scheduling
    unsigned int eip, cs, eflags, esp, ss;
    unsigned int attributes;
    unsigned int time_slice;
    CONSOLE *console;
   // unsigned int cr3;


    void *next;
}THREAD_STRUCT;

extern volatile unsigned long time_slice;
typedef struct{
     void *prev;
	char PName[33]; //the process name
	unsigned int PID; //the proccess ID
	char UserName[33]; //the username underwhich the process is loaded
	unsigned int MemoryUsed; //how much memory is used in kb
     unsigned int BasePriority; //what the base priority is
     unsigned int NumThreads; //how many threads are in this process
     unsigned int Attributes;
     THREAD_STRUCT *Threads;
     THREAD_STRUCT *LastThread;
     void *next; //only used internally
}PROCESS_STRUCT;











#ifdef __cplusplus
}
#endif

#endif
