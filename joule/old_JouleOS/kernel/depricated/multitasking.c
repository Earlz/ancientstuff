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


extern volatile TSS_STRUCT *TSS;

typedef struct
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity; //has limit
    unsigned char base_high;
}gdt_entry
__attribute__((packed));



typedef struct{
	unsigned int EAX,ECX,EDX,EBX,
	ESP,EBP,ESI,EDI
}GENERAL_REGISTERS;
typedef struct{
	unsigned short ES,CS,SS,DS,FS,GS
}SEGMENT_REGISTERS;

typedef struct{
	unsigned int EFLAGS,EIP
}SPECIAL_REGISTERS;


typedef struct{ //aligned on 4
	unsigned int PID; //proccess ID
	void *next; //next task -0 for last task
	unsigned int priority; //priority of task
	char *name; //name to be displayed as process name in stuff
	void *start; //where in memory it starts
	unsigned int size; //size in pages
	void *stack_start; //assumes already allocated
	unsigned int stack_size; //in bytes so nothing over 1mb stack
     GENERAL_REGISTERS gregs;
     SEGMENT_REGISTERS sregs;
     SPECIAL_REGISTERS special; //eflags, eip, maybe CR3 later on
     unsigned int attributes; //see below 
}TASK;

/*******************************
Attributes:
0:vital task- can not be ended
1:Ring }uses [ring] segments
2:Ring }/
3:driver
4:library --if not then assumed to be normal application
5:process --clear if it is a thread; pid is replaced by owner pid and name is replaced by Thread ID(as unsigned int)
6:do not run --skip this thread(must be thread) only the owner can change this bit
7:

*******************************/
#define VITAL 1
#define RING0 0
#define RING1 2
#define RING2 4
#define RING3 5
#define DRIVER 8
#define LIBRARY 16
#define NORMAL 0
#define PROCESS 32
#define NO_RUN 64



#define PRIORITY_LOW 1
#define PRIORITY_MLOW 2
#define PRIORITY_MEDIUM 3
#define PRIORITY_MHIGH 4
#define PRIORITY_HIGH 5
#define PRIORITY_REALTIME 6 //reserved for system tasks and even then should not be used much




TASK *EmptyTask; //task that does nothing and is always the first task
TASK *Console; //task for the console
unsigned int num_tasks;

TASK *ptr_to_last_task;


TASK *current_task; //this is the currently running task



void empty_task(){
	__asm("hlt");
}



void init_taskman(){
	unsigned int i;
	EmptyTask=malloc(sizeof(TASK));
	Console=malloc(sizeof(TASK));
	EmptyTask->PID=0;
	EmptyTask->next=Console;
	EmptyTask->priority=1; //1 means 1 tick
	EmptyTask->name="Empty Task";
	EmptyTask->start=&empty_task;
	EmptyTask->size=1; 
	EmptyTask->stack_start=malloc(512); //512 byte stack
	EmptyTask->stack_size=512;
	memset(&EmptyTask->gregs,0,sizeof(GENERAL_REGISTERS)); //0 the general registers
	//segement registers are a bit more complex
	//I guess I will use "set on task" segement setting
	
	//Code segment-- access--1 (0)00 1 1 0 1 1
	//gran--1 1 0 0
	gdt_set_gate(8,EmptyTask->start,EmptyTask->size,0x9B,0xCF);
	//Data segment-- access--1 (0)00 1 0 0 1 1
	//1 1 0 0
	gdt_set_gate(9,EmptyTask->start,EmptyTask->size,0x93,0xCF);
	//Stack segment- access--1 (0)00 1 0 0 1 1
	//gran--0 1 0 0
	gdt_set_gate(10,EmptyTask->stack_start,EmptyTask->stack_size,0x93,0x4F);
	
	
	EmptyTask->sregs.ES=9*8;
	EmptyTask->sregs.CS=8*8;
	EmptyTask->sregs.SS=10*8;
	EmptyTask->sregs.DS=9*8;
	EmptyTask->sregs.FS=9*8;
	EmptyTask->sregs.GS=9*8;
	
	EmptyTask->special.EIP=0;
	EmptyTask->special.EFLAGS=GetEFLAGS();
	
	EmptyTask->attributes=VITAL|RING0|NORMAL|PROCESS; //for now anyway
	ptr_to_last_task=EmptyTask;
	
	// W00000000000000000000000000T!!!!!!!!!!!!!!!!!!!
	//hopefully it compiles and works now
	
	

}



TASK *NewTask(char *name,unsigned int priority,void *start,unsigned int size,void *stack_start,unsigned int stack_size,unsigned int attributes){
	//stuff
	TASK *new_task;
	new_task=malloc(sizeof(TASK));
	ptr_to_last_task->next=new_task;
	
	new_task->PID=new_task;
	new_task->next=0;
	new_task->priority=priority;
	new_task->name=name;
	new_task->start=start;
	new_task->size=size;
	new_task->stack_start=stack_start;
	new_task->stack_size=stack_size;
	
	memset(&new_task->gregs,0,sizeof(GENERAL_REGISTERS));
	
	//code segment
	gdt_set_gate(11,start,size,((attributes&0x6)>>1)|0x9B,0xCF);
	//data segment
	gdt_set_gate(12,start,size,((attributes&0x6)>>1)|0x92,0xCF);
	//stack segment
	gdt_set_gate(13,stack_start,stack_size,((attributes&0x6)>>1)|0x92,0x4F);
	//set the segments
	new_task->sregs.ES=12*8;
	new_task->sregs.CS=11*8;
	new_task->sregs.SS=13*8;
	new_task->sregs.DS=12*8;
	new_task->sregs.FS=12*8;
	new_task->sregs.GS=12*8;
	
	new_task->special.EIP=0;
	new_task->special.EFLAGS=GetEFLAGS();
	
	new_task->attributes=attributes;
	ptr_to_last_task=new_task;
	
	return new_task;
}
	

typedef struct
{
    unsigned int ds, es, fs, gs;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
}
regs;



void SwitchTask(regs *r){
	unsigned int i;
}
	
	



     
     
     
     
	
	
	













     
	
	



