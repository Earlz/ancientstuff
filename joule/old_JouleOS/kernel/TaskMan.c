//See COPYRIGHT.txt for info on copyright
#include <k_JouleOS.h>
#include <stdio.h>


/*
ok idea for my gdt segment problem...
at a thread switch, only trade out segments
at a process switch trade out 'address space' which is the segments actual data in the gdt


*/





/*attributes for threads:
0-2: priortiy level with 7 being the most commonly called and 1 being least, 0 uses the proccess's priority
3: ????
*/

/*attributes for processes:
0-2: 0 is a normal 32bit application, 1 is a ring3 device driver, 2 is a ring0 device driver, 3 is a kernel process, 4 is an external kernel process(relocatable),

*/

unsigned char Priority[]={1,5,7,10,15,25,40,100};



//volatile PROCESS_STRUCT *Procs;
volatile PROCESS_STRUCT *FirstProc;
volatile PROCESS_STRUCT *CurrentProc;
volatile PROCESS_STRUCT *LastProc;
volatile THREAD_STRUCT *CurrentThread;
volatile unsigned int ThreadCount;

volatile BOOL multitasking=0;




//if con is null then it creates a console with default settings
unsigned int NewProcess(char *name, char* username, unsigned int priority,unsigned int attributes,unsigned int base,unsigned int size, unsigned int vstack,unsigned int vstack_size,CONSOLE *con){
	PROCESS_STRUCT *Proc;
     regs *r;
     //__asm("cli"); //were busy real quick
	Proc=malloc(sizeof(PROCESS_STRUCT));
	LastProc->next=Proc;
	Proc->prev=LastProc;
	LastProc=Proc;
	LastProc->next=FirstProc; //make the loop
	//CurrentProc=Proc;
     //vstack is where at in their gdt
     switch(attributes){
     	case 3: //ring0 kernel process using kernel segments
     	memcpy(Proc->PName,name,33);
     	Proc->PID=Proc; //genius!
     	memcpy(Proc->UserName,username,33);
     	//Proc->MemoryUsed=size; //size should be in multiples of 4096(pages)
     	Proc->MemoryUsed=size;
     	Proc->BasePriority=priority;
     	Proc->NumThreads=1;
     	Proc->Attributes=attributes;
     	//Proc->next=FirstProc;
     	//now in thread setup
     	Proc->Threads=malloc(sizeof(THREAD_STRUCT));
     	Proc->Threads->prev=Proc->Threads;
     	Proc->Threads->next=Proc->Threads;
        Proc->LastThread=Proc->Threads;
          if(con==NULL){con=CreateConsole(USER_CONSOLE,Proc->Threads,name,1);}
          Proc->Threads->console=con;
     	//memcpy(Proc->Threads->name,name,33);
     	Proc->Threads->TID=Proc->Threads;
          //Proc->Threads->cr3=0;
          Proc->Threads->time_slice=Priority[priority];
          Proc->Threads->cs=0x10;
          Proc->Threads->ss=0x08;
          Proc->Threads->eflags=0x202;
          Proc->Threads->eip=base;
          Proc->Threads->esp=vstack+vstack_size-sizeof(regs);  //stack is expand down... so we set the stack to where it would be at an int
          Proc->Threads->other_esp=Proc->Threads->esp;
          //setup the first stack..
          r=Proc->Threads->esp;
          //do segments
          r->ds=8;
          r->es=8;
          r->fs=8;
          r->gs=8;
          r->ss=8;
          r->cs=0x10;
          //registers
          r->eax=0;
          r->ebx=0;
          r->ecx=0;
          r->edx=0;
          r->edi=0;
          r->esi=0;
          r->ebp=0;
          r->esp=Proc->Threads->esp;
          //other stuff
          r->eip=base;
          r->eflags=0x202;
          r->useresp=r->esp;
          //Proc->Threads->esp-=sizeof(regs); //set the stack right...
          r->esp=Proc->Threads->esp;
          r->useresp=r->esp;
     	//Proc->Threads->eflags=0x202
     	//__asm("sti");
     	//CurrentThread=Proc->Threads;
     	return Proc->PID;
     	break;
     }
}


THREAD_STRUCT *CreateThread(char *name,PROCESS_STRUCT *owner,unsigned int base,unsigned int vstack,unsigned int vstack_size){ //no size in this.. uses process's size
     THREAD_STRUCT *thread;
     regs *r;
     thread=malloc(sizeof(THREAD_STRUCT));
     memcpy(thread->name,name,33);
     __asm("cli"); //so the scheduler doesn't pop up
     owner->LastThread->next=thread;
    owner->LastThread=thread;
    thread->next=owner->Threads;
     switch(owner->Attributes){
          case 3:


     	//memcpy(Proc->Threads->name,name,33);
     	//Proc->Threads->TID=Proc->Threads;
          //Proc->Threads->cr3=0;
          thread->cs=0x10;
          thread->ss=0x08;
          thread->eflags=0x202;
          thread->eip=base;
          thread->esp=vstack+vstack_size-sizeof(regs);  //stack is expand down... so we set the stack to where it would be at an int
          thread->other_esp=thread->esp;

          //setup the first stack..
          r=thread->esp;
          //do segments
          r->ds=8;
          r->es=8;
          r->fs=8;
          r->gs=8;
          r->ss=8;
          r->cs=0x10;
          //registers
          r->eax=0;
          r->ebx=0;
          r->ecx=0;
          r->edx=0;
          r->edi=0;
          r->esi=0;
          r->ebp=0;
          r->esp=thread->esp;
          //other stuff
          r->eip=base;
          r->eflags=0x202;
          r->useresp=r->esp;
          //Proc->Threads->esp-=sizeof(regs); //set the stack right...
          r->esp=thread->esp;
          r->useresp=r->esp;


          break;
     }




__asm("sti");
return thread->TID;
}




//this completely works with everything including general registers... eventuslly include CR regs
void SwitchThread(THREAD_STRUCT *thread){ //this MUST be used inside of an interrupt
     CurrentThread->other_esp=int_stack;
     (volatile)int_stack=thread->other_esp;
     CurrentThread=thread;

}



//task init and testing stuff
#include <debug.h>
void TaskSchedule(){ //this is called for schedueling duing irq0 every ms --must be inside int
    int i;
    (volatile)time_slice--;
    if((volatile)time_slice==0){ //then switch
    debug{
        //printf("c");
       // printf("CurrentThread: 0x%x CurrentProc->LastThread: 0x%x",(volatile)CurrentThread,(volatile)CurrentProc->LastThread);
    }
        if((volatile)CurrentThread==(volatile)CurrentProc->LastThread){ //if were in the last thread of the proc
            debug{
               // printf("ble!");
               for(i=0;i>0xFF;i++){
               }
               // wait(20);
            }
            CurrentProc=CurrentProc->next;
            CurrentThread->console=ProcessConsole;
            SwitchThread(CurrentProc->Threads);
            ProcessConsole=CurrentThread->console;
        }else{
            SwitchThread(CurrentThread->next);
        }
        time_slice=CurrentThread->time_slice;
    }

}

void IdleFunc(){
    printf("x");
    //wait(5000);
    printf("z");
    for(;;){}
    //__asm("cli");
    //__asm("hlt");
}

void KernelStart(){
    printf("blah!");

    printf("c");
    printf("a");
    for(;;){}
    //dunno where to start...
}

volatile unsigned int IdleProc_pid;
volatile unsigned int KernelProc_pid;

volatile unsigned int *IdleStack;
volatile unsigned int *KernelStack;

void TempTaskingInt(regs *r){ //must be inside int
    PROCESS_STRUCT *Kerneltmp;
    //printf("t21");
    (volatile)multitasking=1;
    Kerneltmp=KernelProc_pid;
    CurrentProc=Kerneltmp;
     //CurrentThread->other_esp=int_stack; //don't save this
     (volatile)int_stack=Kerneltmp->Threads->other_esp;
     CurrentThread=Kerneltmp->Threads;
     ProcessConsole=CurrentThread->console;

    //SwitchThread(Kerneltmp->Threads);

}

void InitTasking(){
    IdleStack=malloc(4096);
    LastProc=malloc(sizeof(PROCESS_STRUCT)); //only for temp
    IdleProc_pid=NewProcess("Idle Proc","SYSTEM",0,3,IdleFunc,4096,IdleStack,4096,DebugCon);
    FirstProc=IdleProc_pid;
    KernelStack=malloc(4096*4);
    KernelProc_pid=NewProcess("Kernel","SYSTEM",3,3,KernelStart,(4096*32),KernelStack,(4096*4),JouleCon);
    (volatile)CurrentProc=IdleProc_pid;
    (volatile)CurrentThread=CurrentProc->Threads;
    debug{
        printf("CurrentThread: 0x%x CurrentProc->LastThread: 0x%x",(volatile)CurrentThread,(volatile)CurrentProc->LastThread);
    }
    (volatile)time_slice=CurrentProc->Threads->time_slice;
    (volatile)multitasking=1; //start it
    UninstallInterruptHook(60);
    InstallInterruptHook(60,TempTaskingInt);
    __asm(".intel_syntax\n"
    "int 60\n" //thread switch
    ".att_syntax\n");
    for(;;){
        wait(20);
        printf("z");
    }
    //point of no return
}















