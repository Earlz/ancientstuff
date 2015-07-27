//See COPYRIGHT.txt for info on copyright
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


struct gdt_entry_bits
{
     unsigned short limit_low;
     unsigned base_low : 24;
     //attribute byte split into bitfields
     unsigned char accessed :1;
     unsigned char read_write :1; //readable for code, writable for data
     unsigned char conforming_expand_down :1; //conforming for code, expand down for data
     unsigned char code :1; //1 for code, 0 for data
     unsigned char always_1 :1; //should be 1 for everything but TSS and LDT
     unsigned char DPL :2; //priveledge level
     unsigned char present :1;
     //and now into granularity
     unsigned char limit_high :4;
     unsigned char available :1;
     unsigned char always_0 :1; //should always be 0
     unsigned char big :1; //32bit opcodes for code, dword stack for data
     unsigned char gran :1; //1 to use 4k page addressing, 0 for byte addressing
     unsigned char base_high :8;
}
__attribute__((packed));


/* Special pointer which includes the limit: The max bytes
*  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
struct gdt_ptr {
    unsigned short limit;
    unsigned int base;
}
__attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */
#ifdef DEBUG
#define GDT_LIMIT 0xFF //so bochs actually gives me the error not just invalid selector
#else
#define GDT_LIMIT 10
#endif
struct gdt_entry gdt[GDT_LIMIT];
struct gdt_ptr gp;

/* This will be a function in start.asm. We use this to properly
*  reload the new segment registers */

extern void gdt_flush();





/* Setup a descriptor in the Global Descriptor Table */
unsigned short gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    /* Setup the descriptor base address */

    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
    return num;
}









/* Should be called by main. This will setup the special GDT
*  pointer, set up the first 3 entries in our GDT, and then
*  finally call gdt_flush() in our assembler file in order
*  to tell the processor where the new GDT is and update the
*  new segment registers */


void gdt_install() {
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * GDT_LIMIT) - 1;
    gp.base = &gdt;

    /* Our NULL descriptor */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* The thrid entry is our Code Segment. The base address
    *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
    *  uses 32-bit opcodes, and is a Code Segment descriptor.
    *  Please check the table above in the tutorial in order
    *  to see exactly what each value means */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* The second entry is our Data Segment. It's EXACTLY the
    *  same as our code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x92, 0xCF);
    /*The fourth entry is our stack segment. The base address
    * is 0x1000, the limit is 0x1000 or 1 4k page(1024 stack entries), it uses 4Kbyte
    * granularity, is 32bit, and is a data/stack segment
    */
    //need to add some extra stuff now with multitasking

    //divide these by 4096 so we get the page number since were using gran
    //This is the ring0 tss stack
    //has 1 page(4096 bytes and starts at 0x100000 (after 1 mb)
    //access- 1 (0)00 1 0 0 1 1
    //gran- 1 1 0 0
    gdt_set_gate(4,0x100000,1,0x93,0xCF);

    //this is the ring1 tss stack
    //has 1 page and starts at 0x101000
    gdt_set_gate(5,0x101000,1,0xB3,0xCF);

    //this is the ring2 tss stack
    //has 1 page and starts at 0x102000 --w00t i did that math by myself
    gdt_set_gate(6,0x102000,1,0xD3,0xCF);

    //this is the ring3 stack --this can change with each task
    //it starts out with 1 page and at 0x103000
    gdt_set_gate(7,0x103000,1,0xF3,0xCF);
    //a second one for speed purposes
   // gdt_set_gate(8,0x104000,1,0xF3,0xCF);

    gdt_set_gate(0x11,0,0xFFFFFFFF,0xFB,0xCF);


    //Application segments





    /*This is used for our printc routine for an easier and faster way to avoid
    cursor overflow*/
    /* Flush out the old GDT and install the new changes! */
    //gdt_set_gate(3, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_flush();
}