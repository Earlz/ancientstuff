#ifndef _K_JOULEOS
#define _K_JOULEOS
/************/
//these defines will modify how jouleos behaves
#define GDT_LIMIT 10

/****end****/

/*
//this define will do get a linked object number
#define linked_GetObject(pointer,number_wanted,name_of_next,name_of_prev,tmp_int)\
    tmp=number_wanted;\
    if(number_wanted>0){\
        for(;tmp_int!=0;tmp_int--){\
            pointer=pointer->name_of_next; \
        }\
    }else{\
        if(number_wanted!=0){\
        for(;tmp_int!=0;tmp_int++){\
            pointer=pointer->name_of_prev; \
        }\
        }\
    }

*/




//GDT stuff
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
     unsigned short limit_low : 16;
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
}  __attribute__((packed));

struct gdt_ptr {
    unsigned short limit;
    unsigned int base;
}
__attribute__((packed));


extern volatile void *GdtPtr;
extern struct gdt_entry_bits Gdt[GDT_LIMIT];

typedef struct{
    unsigned int *address; //this is a bitmap that tells if the page/byte is used  [32768]
    void *owner;
    unsigned int limit; //this is the limit of either pages or bytes, depending on which is used
    unsigned char page_addressing; //this should be 1 if this uses 4k paging, else it uses byte address
}AddressSpace;


extern AddressSpace physical_memory;
extern AddressSpace virtual_memory;





#endif



