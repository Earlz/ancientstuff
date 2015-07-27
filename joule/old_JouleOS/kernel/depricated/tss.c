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

/* Our GDT, with 3 entries, and finally our special GDT pointer */
#ifdef DEBUG
#define GDT_LIMIT 0xFFFF //so bochs actually gives me the error not just invalid selector
#else
#define GDT_LIMIT 65
#endif
extern struct gdt_entry gdt[GDT_LIMIT];

/* This will be a function in start.asm. We use this to properly
*  reload the new segment registers */


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

volatile TSS_STRUCT *TSS;

extern void gdt_flush();
//limit is 20 bits --1mb
unsigned short gdt_set_gate_tss(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    /* Setup the descriptor base address */
    
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;
    
    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);
    
    /* Finally, set up the granularity and access flags */
    gdt[num].granularity = gdt[num].granularity | (gran&0xF0); //the only difference
    gdt[num].access = access;
    return num;
}


void jmpback(){
	__asm(".intel_syntax noprefix\n"
     "jmp _TSS_JMP\n"
     ".att_syntax\n");
}

extern void TSS_JMP();
unsigned short TSS_ptr;
void Install_TSS(){ //run after install_gdt   --WORKING!!!!!! and got it all in one try excluding stupid ltr
unsigned int tmp;
//1 0 0 0 limit(4) --granularity
//1 (3)11 0 1 0 0 1
TSS=AllocCore(1); //alloc a page for future stuff and so its aligned well
TSS_ptr=3*8;
tmp=TSS;
    // gdt_set_gate(3,0,103, 0x89,0x0); //81
    gdt[3].limit_low=104;
    gdt[3].base_low=0;
    gdt[3].base_middle=0;
    gdt[3].access=0x89;
    gdt[3].granularity=0;
    gdt[3].base_high=0;
    
     TSS->IO_map=0;
     TSS->LDT_selector=0;
     //this stuff changes by task
     TSS->ES=0x8;
     TSS->CS=0x10;
     TSS->SS=8; //while were in kernel mode anyway
     TSS->DS=8;
     TSS->FS=8;
     TSS->GS=8;
     TSS->ESP=GetESP();
     TSS->EBP=GetEBP();
     TSS->ESI=GetESI();
     TSS->EDI=GetEDI();
     TSS->EAX=GetEAX();
     TSS->ECX=GetECX();
     TSS->EDX=GetEDX();
     TSS->EBX=GetEBX();
     TSS->EIP=sizeof(TSS)+2; //special jump label
     TSS->EFLAGS=GetEFLAGS();
     TSS->CR3=0; //for now anyway
     TSS->ESP2=0;
     TSS->SS2=8*6;
     TSS->ESP1=0;
     TSS->SS1=8*5;
     TSS->ESP0=0;
     TSS->SS0=8*4; //assuming theirs a difference
     TSS->prev_task=0; //not used...I hope *_*
     memcpy(tmp+sizeof(TSS)+2, &jmpback, 20);
     LTR(3*8); //load task register
     __asm(".intel_syntax noprefix\n"
     "_TSS_JMP:\n"
     ".att_syntax\n");
     
}
