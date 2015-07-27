//this file has all the global macros and vars we need
#ifndef _BASE_H
#define _BASE_H
/********defines that change the behaviour of Open86..****/
#define DEV_LIMIT 256 //this is the max number of devices
#define MEMORY_SIZE 0x10000 //this is the memory size in bytes
#define MAX_MEMORY_ALERTS 64 //maximum amount of memory alerts
//by memory alerts I mean for example how when someone writes to 0xb800 it will alert the video device of the write
//#define LOW_MEMORY_USAGE  //you define this if you want to use the least amount of memory possible in exchange for slower performance




//#define MAYBE_OPTIMIZATIONS //you can define this for optimizations that may or may not be faster
//the optimizations in helpers.c are NOT safe!!!

/***end***/

//16 bit register macros
#define AX 0
#define CX 1
#define DX 2
#define BX 3
#define SP 4
#define BP 5
#define SI 6
#define DI 7

//8 bit register macros
#define AL 0
#define CL 1
#define DL 2
#define BL 3
#define AH 4
#define CH 5
#define DH 6
#define BH 7

//segment registers
#define ES 0
#define CS 1
#define SS 2
#define DS 3
//#define FS 4   //only 8086 stuff!!
//#define GS 5
#define IS 6 //this is an imaginary segment only used for direct segment overrides
//for instance it would be used in mov [1000:bx],ax

//bit locations for the return of the error code of Memxxxx
#define MEM_SUCCESS
#define GPF 1
#define PAGE_FAULT 2
#define STACK_FAULT 4
#define UNDEFINED_OPCODE 8

#define OPCODE_SPECIFIC 16 //this is used in GetModRM if it doesn't use memory, and therefore opcode specific

//cpu level numbers..
#define XBEST 0 //this means it will use the best cpu currently stable and fully supported
#define X086 1
#define X186 2
#define X286 4
#define X386 8
#define X486 16


#ifdef _64BITS_
#warning 64bit mode may not work properly in this file!!
#endif

#ifndef LOW_MEMORY_USAGE //SPEED_UP will inline a function unless low memory usage is required
#define SPEED_UP inline
#else
#define SPEED_UP
#endif


//extra macros
//#define doeip eip=(ip+(sregs[1]<<4)) //this will update eip
//#define doiptr iptr=(eip+core) //this will update iptr from eip
/*#define doeip eip=(ip+(sregs[1]<<4)) //this will update eip
#define doiptr_code iptr=(core+(ip+(sregs[1]<<4))); eip=(unsigned int)iptr-(unsigned int)core //this will update iptr from eip
#define doiptr_data iptr=(core+(ip+(sregs[CurrentSegment]<<4)))*/

#define doeip eip=(ip+(sregs[1]<<4)) //this will update eip
#define doiptr iptr=(eip+core) //this will update iptr from eip

void Warning(char *msg);
extern volatile unsigned short gregs16[8];
extern volatile unsigned char *gregs8[8]; //all the highs are up 1 byte so they use the same memory
//I'm using this super messy initialization to be able to easily go *gregs8[AH] and it work
extern volatile unsigned short sregs[7]; //segment registers
extern volatile unsigned short flagreg;
extern volatile unsigned short ip;
extern volatile unsigned int eip; //this is only for our use and is with CS calculated
extern volatile unsigned char *iptr; //this is with all offsets included(like core)
extern volatile unsigned char no_advance;
extern void *core;
extern volatile unsigned char CurrentSegment; //we use this for segment overrides
extern unsigned char StackSegment; //we use this for segment overrides(on mov's usually)
typedef void (*opcode)(void);
extern volatile opcode *Opcodes;

typedef unsigned int(*Port)(unsigned int,unsigned char,unsigned char); //in=Port(out,in_or_out,size_of_data) --size of data is in number of bytes
//anything smaller than a 32bit value should be zero extended and in_or_out is 1 for 1 and 0 for out
extern volatile Port **ports;

typedef struct{
    unsigned char cf:1;
    unsigned char r0:1;
    unsigned char pf:1;
    unsigned char r1:1;
    unsigned char af:1;
    unsigned char r2:1;
    unsigned char zf:1;
    unsigned char sf:1;
    unsigned char tf:1;
    unsigned char _if:1;
    unsigned char df:1;
    unsigned char of:1;
    unsigned char iopl:2; //not yet used
    unsigned char nt:1;
    unsigned char r3:1;
}__attribute__((packed))FLAGS; //this is a better representation of flags


extern volatile FLAGS *flags;
extern void **Devices;
extern unsigned int dev_i; //index for devices

struct{
    unsigned char timer_0:1;
    unsigned char keyboard_1:1;
    unsigned char retrace_2:1;
    unsigned char com2_3:1;
    unsigned char com1_4:1;
    unsigned char disk2_5:1;
    unsigned char floppy_6:1;
    unsigned char lpt_7:1;
    unsigned char RTC_8:1;
    unsigned char lan_9:1;
    unsigned char reserved_A:1;
    unsigned char reserved_B:1;
    unsigned char mouse_C:1;
    unsigned char fpu_error_D:1;
    unsigned char disk2_E:1;
    unsigned char reserved_F:1;
}PIC_storage
__attribute__((packed));


#include "Open86_Device.h"

extern volatile unsigned char UsesMemory;

typedef unsigned int(*O86Init)(GlobalDevice *__GlobStruct);
typedef void (*O86End)(void);
typedef unsigned char *(*O86GetName)(void);
typedef unsigned char *(*O86DevName)(void);
extern unsigned int cpu_level;
extern char *open86_dir;
extern char *config_dir;
extern char *dev_dir;
extern char *tmp_filename;





typedef struct{
     unsigned char rm:3;
     unsigned char extra:3;
     unsigned char mod:2;
}
__attribute__((packed))mod_rm; //this struct is a described mod r/m byte

typedef struct{
    void *ptr; //the calculated ptr
    mod_rm rm; //the rm byte
    unsigned short prev_ip; //the ip when the function started
}mod_rm_return;
/*
typedef struct{
    unsigned char r3:1;
    unsigned char nt:1; //unused
    unsigned char iopl:2; //unused right now
    unsigned char of:1;
    unsigned char df:1;
    unsigned char _if:1;
    unsigned char tf:1;
    unsigned char sf:1;
    unsigned char zf:1;
    unsigned char r2:1; //reserved
    unsigned char af:1;
    unsigned char r1:1; //reserved
    unsigned char pf:1;
    unsigned char r0:1; //reserved
    unsigned char cf:1;
}__attribute__((packed))FLAGS; //this is a bitfield struct of flags
*/


void *GetModRM8r();
void *GetModRM16r();
unsigned int push(unsigned short);
unsigned short pop();
void CallInt(unsigned char num);



//opcode defs
void unknown();
void hlt();
void nop();
void jmp_rel8();
void jmp_rel16();
void jmp_ptr16seg16();
void mov_8greg_ib();
void mov_16greg_iw();
void mov_rm8_greg8();
void push_r16();
void pop_r16();
void push_imm8();
void push_imm16();
void call_rel16();
void retn0();
void call_far_ptr16();
void push_es();
void push_cs();
void push_ds();
void push_ss();
void group2_ff();
void sub_ax_iw();
void sub_al_ib();
void ja_rel8();
void jnc_rel8();
void jbe_rel8();
void jc_rel8();
void jz_rel8();
void jnz_rel8();
void jp_rel8();
void jnp_rel8();
void jg_rel8();
void jge_rel8();
void jle_rel8();
void jl_rel8();
void jo_rel8();
void jno_rel8();
void js_rel8();
void jns_rel8();
void salc();
void cmc();
void cli();
void cld();
void clc();
void out_imm8_al();
void loop_rel8();
void out_imm8_ax();
void mov_rm16_greg16();
void mov_rm16_sreg();
void mov_rm8_greg8();
void mov_sreg_rm16();
void mov_greg8_rm8();
void mov_greg16_rm16();
void int_normal();
void iret();
void int3();
void into();
void add_al_ib();
void add_ax_iw();
void pop_es();
void pop_ss();
void pop_ds();
void lods_m8();


extern volatile GlobalDevice *GlobStruct;




#ifdef NDEBUG //the debug{} macro
#define debug if(1)
#else
#define debug if(0)
#endif



#endif


