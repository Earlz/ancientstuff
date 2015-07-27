#include <k_JouleOS.h>






struct gdt_entry_bits Gdt[GDT_LIMIT];
struct gdt_ptr gp; //our gdt pointer
//this will setup the gdt base and limit(does our distributions to high and low)
void SetGdtBaseAndLimit(unsigned int gdt_number, unsigned int base,unsigned int limit){
    //yea horrible stuff up there but it works..
    unsigned int tmp;
    Gdt[gdt_number].limit_low=(limit&0x0000FFFF);
    Gdt[gdt_number].limit_high=((limit&0x000F0000)>>16);
    Gdt[gdt_number].base_low=(base&0x00FFFFFF);
    Gdt[gdt_number].base_high=((base&0xFF000000)>>24);
}

extern void (*flush_me)(void);
void flush_segments(void);
void InstallGdt(){
    unsigned int gdt_size=(sizeof(struct gdt_entry_bits)*(GDT_LIMIT+1));
    memset(&Gdt,0,gdt_size);
    //set up our data segment first
    SetGdtBaseAndLimit(1,0, 0xFFFFFF);
    Gdt[1].accessed=1;
    Gdt[1].read_write=1;
    Gdt[1].conforming_expand_down=0;
    Gdt[1].code=0;
    Gdt[1].always_1=1;
    Gdt[1].DPL=0;
    Gdt[1].present=1;
    Gdt[1].available=1;
    Gdt[1].always_0=0;
    Gdt[1].big=1;
    Gdt[1].gran=1;

    //now setup our code segment
    SetGdtBaseAndLimit(2,0, 0xFFFFFF);
    Gdt[2].accessed=1;
    Gdt[2].read_write=1;
    Gdt[2].conforming_expand_down=0;
    Gdt[2].code=1;
    Gdt[2].always_1=1;
    Gdt[2].DPL=0;
    Gdt[2].present=1;
    Gdt[2].available=1;
    Gdt[2].always_0=0;
    Gdt[2].big=1;
    Gdt[2].gran=1;

    //now setup our gdt pointer
    gp.limit = (sizeof(struct gdt_entry_bits) * GDT_LIMIT) - 1;
    gp.base = &Gdt;
    flush_segments();

}



































//extra


/******WARNING!!!! your GDT variable should be declared as VOLATILE!! if it isn't then you may have problems...****/

/*******required structs*****/
struct _gdt_entry_bits
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
     unsigned char none :1; //not used
     unsigned char always_0 :1; //should always be 0
     unsigned char big :1; //32bit opcodes for code, dword stack for data
     unsigned char gran :1; //1 to use 4k page addressing, 0 for byte addressing
     unsigned char base_high :8;
}  __attribute__((packed));


/*******Required Variables*****/
volatile void *GdtPtr; //warning! this assumes flat address space!
volatile unsigned short gdt_CurrentCS;


/*****just little helper functions******/
void *gdt_Get_GdtPtr(){
    return GdtPtr;
}
void gdt_Set_GdtPtr(void *ptr){
    GdtPtr=ptr;
}




/*****Actual Gdt Functions******/
unsigned int gdt_GetLimit(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt[num].gran==1){
        putc_tmp('t');
        return (gdt[num].limit_low+(gdt[num].limit_high<<16))*4096; //use paging granularity
    }else{
        return (gdt[num].limit_low+(gdt[num].limit_high<<16)); //use byte granularity
    }
}

unsigned char gdt_IsCode(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    return gdt[num].code;
}

unsigned char gdt_GetDPL(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    return gdt[num].DPL;
}

unsigned char gdt_IsAccessed(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    return gdt[num].accessed;
}

unsigned char gdt_IsConforming(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt[num].code==1){
        return gdt[num].conforming_expand_down;
    }else{return 0;}
}

unsigned char gdt_IsExpandDown(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt[num].code==0){
        return gdt[num].conforming_expand_down;
    }else{return 0;}
}


unsigned int gdt_GetBase(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    return ((gdt[num].base_low)+(gdt[num].base_high<<24));
}


unsigned char gdt_IsReadable(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt[num].code==1){
        return gdt[num].read_write;
    }else{
        return 1;
    }
}

unsigned char gdt_IsWriteable(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt[num].code==0){
        return gdt[num].read_write;
    }else{
        return 0;
    }
}

unsigned char gdt_IsPresent(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    return gdt[num].present;
}

unsigned char gdt_Is32Bit(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    return gdt[num].big;
}

unsigned char gdt_Uses4kPaging(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    return gdt[num].gran;
}




//these with memory added to the function checks it can write to that memory location
unsigned char gdt_CanAccessThis(unsigned short num,void *memory){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt_GetLimit(num)<memory){return 0;} //is limit greater than limit?
    if(gdt[num].present==0){return 0;} //segment isn't present
    __asm(".intel_syntax noprefix\n"
    "mov [_gdt_CurrentCS],CS\n" //get the current code segment
    ".att_syntax\n");
    if(gdt[num].code==1 && gdt[num].conforming_expand_down==0){
        if(gdt[num].DPL!=gdt[gdt_CurrentCS].DPL){return 0;} //for code segments we can't access anything but current priveledge
    }
    if(gdt[num].DPL<gdt[gdt_CurrentCS].DPL){return 0;} //is the requested segment at a higher priveledge?

    return 1;
}

unsigned char gdt_CanReadThis(unsigned short num,void *memory){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt_GetLimit(num)<memory){return 0;} //is limit greater than limit?
    if(gdt[num].present==0){return 0;} //segment isn't present
    __asm(".intel_syntax noprefix\n"
    "mov [_gdt_CurrentCS],CS\n" //get the current code segment
    ".att_syntax\n");
    if(gdt[num].code==1 && gdt[num].conforming_expand_down==0){
        if(gdt[num].DPL!=gdt[gdt_CurrentCS].DPL){return 0;} //for code segments we can't access anything but current priveledge
    }
    if(gdt[num].DPL<gdt[gdt_CurrentCS].DPL){return 0;} //is the requested segment at a higher priveledge?
    if(gdt[num].code==1 && gdt[num].read_write==0){return 0;}

    return 1;
}

unsigned char gdt_CanWriteThis(unsigned short num,void *memory){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt_GetLimit(num)<memory){return 0;} //is limit greater than limit?
    if(gdt[num].present==0){return 0;} //segment isn't present
    __asm(".intel_syntax noprefix\n"
    "mov [_gdt_CurrentCS],CS\n" //get the current code segment
    ".att_syntax\n");
    if(gdt[num].code==1 && gdt[num].conforming_expand_down==0){
        if(gdt[num].DPL!=gdt[gdt_CurrentCS].DPL){return 0;} //for code segments we can't access anything but current priveledge
    }
    if(gdt[num].DPL<gdt[gdt_CurrentCS].DPL){return 0;} //is the requested segment at a higher priveledge?
    if((volatile)gdt[num].code==1){return 0;}
    if(gdt[num].read_write==0){return 0;}

    return 1;
}

unsigned char gdt_CanExecuteThis(unsigned short num,void *memory){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt_GetLimit(num)<memory){return 0;} //is limit greater than limit?
    if(gdt[num].present==0){return 0;} //segment isn't present
    __asm(".intel_syntax noprefix\n"
    "mov [_gdt_CurrentCS],CS\n" //get the current code segment
    ".att_syntax\n");
    if(gdt[num].code==1 && gdt[num].conforming_expand_down==0){
        if(gdt[num].DPL!=gdt[gdt_CurrentCS].DPL){return 0;} //for code segments we can't access anything but current priveledge
    }
    if(gdt[num].DPL<gdt[gdt_CurrentCS].DPL){return 0;} //is the requested segment at a higher priveledge?
    if(gdt[num].code==0){return 0;}

    return 1;
}


//these without memory added onto it just check general memory..no specific address

unsigned char gdt_CanAccess(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt[num].present==0){return 0;} //segment isn't present
    __asm(".intel_syntax noprefix\n"
    "mov [_gdt_CurrentCS],CS\n" //get the current code segment
    ".att_syntax\n");
    if(gdt[num].code==1 && gdt[num].conforming_expand_down==0){
        if(gdt[num].DPL!=gdt[gdt_CurrentCS].DPL){return 0;} //for code segments we can't access anything but current priveledge
    }
    if(gdt[num].DPL<gdt[gdt_CurrentCS].DPL){return 0;} //is the requested segment at a higher priveledge?
    if(gdt[num].code==1 && gdt[num].read_write==0){return 0;}

    return 1;
}

unsigned char gdt_CanWrite(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt[num].present==0){return 0;} //segment isn't present
    __asm(".intel_syntax noprefix\n"
    "mov [_gdt_CurrentCS],CS\n" //get the current code segment
    ".att_syntax\n");
    if(gdt[num].code==1 && gdt[num].conforming_expand_down==0){
        if(gdt[num].DPL!=gdt[gdt_CurrentCS].DPL){return 0;} //for code segments we can't access anything but current priveledge
    }
    if(gdt[num].DPL<gdt[gdt_CurrentCS].DPL){return 0;} //is the requested segment at a higher priveledge?
    if(gdt[num].code==1){return 0;}
    if(gdt[num].read_write==0){return 0;}

    return 1;
}

unsigned char gdt_CanExecute(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt[num].present==0){return 0;} //segment isn't present
    __asm(".intel_syntax noprefix\n"
    "mov [_gdt_CurrentCS],CS\n" //get the current code segment
    ".att_syntax\n");
    if(gdt[num].code==1 && gdt[num].conforming_expand_down==0){
        if(gdt[num].DPL!=gdt[gdt_CurrentCS].DPL){return 0;} //for code segments we can't access anything but current priveledge
    }
    if(gdt[num].DPL<gdt[gdt_CurrentCS].DPL){return 0;} //is the requested segment at a higher priveledge?
    if(gdt[num].code==0){return 0;}

    return 1;
}



void *gdt_SegmentToFlat(unsigned short num,void *segmented){ //this converts the memory with 1 segment to flat memory address
    struct _gdt_entry_bits *gdt=GdtPtr;
    return segmented+gdt_GetBase(num);
}

void *gdt_FlatToSegment(unsigned short num,void *flat){ //you should always make sure that the address is valid!
    return flat-gdt_GetBase(num);
}

//this converts segmented memory to a different segment
//flat should be the destination segment and segmented the "to be converted" segment and ptr be the offset of segmented
void *gdt_SegmentToSegment(unsigned short flat,unsigned short segmented,void *ptr){
    //do this later maybe...can't figure out how..

}



//these are just little extra's...
unsigned char gdt_IsData(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    return gdt[num].code^1; //flip it over so if it is code, it's not data
}




//decided to add gdt writing instructions...lucky you

//this automatically sets gran and limit should be in BYTES not pages!!
//this returns what the limit is actually set to(in bytes or pages)
unsigned int gdt_SetLimit(unsigned short num,unsigned int limit){ //warning! if this is over 0xFFFFF then it will automatically set the gran bit
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(limit>0xFFFFF){
        gdt[num].gran=1;
        limit=limit/4096; //convert bytes into pages
        gdt[num].limit_low=(limit&0xFFFF);
        gdt[num].limit_high=(limit&0xF0000)>>16;
    }else{
        gdt[num].gran=0;
        gdt[num].limit_low=(limit&0xFFFF);
        gdt[num].limit_high=(limit&0xF0000)>>16;
    }
    return limit;
}

unsigned int gdt_SetLimit_NoGran(unsigned short num,unsigned int limit){ //this doesn't modify gran and the limit should be in bytes or pages
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].limit_low=(limit&0xFFFF);
    gdt[num].limit_high=(limit&0xF0000)>>16;
}


void gdt_SetBase(unsigned short num,unsigned int base){
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].base_low=(base&0xFFFFFF);
    gdt[num].base_high=(base&0xFF000000)>>24;
}


void gdt_SetAccess(unsigned short num,unsigned char accessed){
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].accessed=accessed;
}

void gdt_SetPresent(unsigned short num,unsigned char present){
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].present=present;
}

void gdt_SetReadable(unsigned short num,unsigned char readable){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt[num].code==1){
        gdt[num].read_write=readable;
    }
}

void gdt_SetWriteable(unsigned short num,unsigned char writeable){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt[num].code==0){
        gdt[num].read_write=writeable;
    }

}

void gdt_SetConforming(unsigned short num,unsigned char conforming){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt_IsCode(num)==1){
        gdt[num].conforming_expand_down=conforming;
    }
}

void gdt_SetExpandDown(unsigned short num,unsigned char expand_down){
    struct _gdt_entry_bits *gdt=GdtPtr;
    if(gdt_IsData(num)==1){
        gdt[num].conforming_expand_down=expand_down;
    }
}

void gdt_SetCode(unsigned short num,unsigned char code){
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].code=code;
}

void gdt_SetToCode(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].code=1;
}
void gdt_SetToData(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].code=0;
}

void gdt_SetDPL(unsigned short num,unsigned char priv){
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].DPL=priv;
}

void gdt_SetGran(unsigned short num,unsigned char gran){
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].gran=gran;
}

void gdt_Set32bit(unsigned short num,unsigned char bits32){
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].big=bits32;
}

void gdt_SetTo32bit(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].big=1;
}

void gdt_SetTo16bit(unsigned short num){
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].big=0;
}


//these are little useful things...

//this should ALWAYS be called when setting up a new segment descriptor
void gdt_SetForGdt(unsigned short num){ //this does nothing more than setting the always 0 and always 1 bits
    struct _gdt_entry_bits *gdt=GdtPtr;
    gdt[num].always_1=1;
    gdt[num].always_0=0;
}


void gdt_SetBasicData(unsigned short num){ //this will setup a basic flat data segment..good for templating
    gdt_SetForGdt(num); //set it up for a gdt
    gdt_SetToData(num);
    gdt_SetLimit(num,0xFFFFFFFF); //set limit to max of 32bits
    gdt_SetBase(num,0);
    gdt_SetExpandDown(num,0);
    gdt_SetPresent(num,1);
    gdt_SetAccess(num,0); //set to hasn't been accessed
    gdt_SetWriteable(num,1);
    gdt_SetTo32bit(num);
    gdt_SetDPL(num,0); //set to ring0
}


void gdt_SetBasicCode(unsigned short num){ //this will setup a basic flat code segment..good for templating
    gdt_SetForGdt(num); //set it up for a gdt
    gdt_SetToCode(num);
    gdt_SetLimit(num,0xFFFFFFFF); //set limit to max of 32bits
    gdt_SetBase(num,0);
    gdt_SetConforming(num,0);
    gdt_SetPresent(num,1);
    gdt_SetAccess(num,0); //set to hasn't been accessed
    gdt_SetReadable(num,1);
    gdt_SetTo32bit(num);
    gdt_SetDPL(num,0); //set to ring0
}






