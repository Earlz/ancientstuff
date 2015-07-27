//this file has all the global macros and vars we need
/**I will try to give a description of everything in this file**/
#ifndef _BASE_H
#define _BASE_H

#include "config.h"

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
/**NOTE! SPEED_UP will be defined as "inline" if we aren't in low memory usage mode**/
/**In most cases, rather than using inline, you should use SPEED_UP to keep this feature**/


//extra macros
//#define doeip eip=(ip+(sregs[1]<<4)) //this will update eip
//#define doiptr iptr=(eip+core) //this will update iptr from eip
/*#define doeip eip=(ip+(sregs[1]<<4)) //this will update eip
#define doiptr_code iptr=(core+(ip+(sregs[1]<<4))); eip=(unsigned int)iptr-(unsigned int)core //this will update iptr from eip
#define doiptr_data iptr=(core+(ip+(sregs[CurrentSegment]<<4)))*/

//#define doeip eip=(ip+(sregs[1]<<4)) //this will update eip
//#define doiptr iptr=(eip+core) //this will update iptr from eip

void Warning(char *msg);
extern volatile unsigned short gregs16[8]; //This is 16bit general registers
extern volatile unsigned char *gregs8[8]; //all the highs are up 1 byte so they use the same memory
//I'm using this super messy initialization to be able to easily go *gregs8[AH] and it work
extern volatile unsigned short sregs[7]; //segment registers
extern volatile unsigned short flagreg; //This is the unsigned short version of flags(useful for push and such)
extern volatile unsigned short ip;
//extern volatile unsigned int eip; //this is only for our use and is with CS calculated
//extern volatile unsigned char *iptr; //this is with all offsets included(like core)
extern volatile unsigned char no_advance; //This should be set to 1 if you don't want Process() to automatically increment
//note no_advance will only not advance one time, after that it will advance again
extern char core[]; //This is the pointer to the allocated memory used for the guest machine
extern volatile unsigned char CurrentSegment; //we use this for segment overrides
extern unsigned char StackSegment; //we use this for segment overrides(for BP)
typedef void (*opcode)(void); //This is the function type opcodes should be in
extern volatile opcode *Opcodes;

typedef unsigned int(*Port)(unsigned int,unsigned char,unsigned char); //in=Port(out,in_or_out,size_of_data) --size of data is in number of bytes
//anything smaller than a 32bit value should be zero extended and in_or_out is 1 for 1 and 0 for out
extern volatile Port **ports; //This is the array of pointers that point to port functions

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
}__attribute__((packed))FLAGS; //this is a better representation of flags(much easier to use)

extern unsigned int BIOS_port(unsigned int out,unsigned char in_or_out,unsigned char size);

extern unsigned char bin_loaded;
extern unsigned short bin_ip;

extern volatile FLAGS *flags; //This pointer to flagsreg
extern void **Devices; //?
extern unsigned int dev_i; //index for devices
void SendInterrupt(unsigned int number);
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
__attribute__((packed)); //This is the IRQ list of stuff...not really going to be used now


#include "Open86_Device.h" //include GlobalDevice

extern volatile unsigned int pending_messages; //This is how many messages are pending(only provided for speed really)
extern open86_message_event *MessageEvents; //This is allocated by malloc
//This is an array of function pointers sorted by the device type and is called when a message is pushed

extern Open86MessageList *first_message;
extern volatile Open86MessageList *latest_message;
extern volatile unsigned int doing_messages; //This is set when things are being done with messages
//it should be clear before messing with messages, then it should be set while doing things with them

SPEED_UP unsigned int Open86PutMessage(unsigned int target,unsigned int message, unsigned int extra,void *return_info, Open86MessageCallBack callback,void *extra_ptr, Open86MessageCallBack done_callback );

void UninstallMessageEvent(unsigned int target);
extern volatile unsigned char UsesMemory; //not used?
unsigned int PIC_port_20h(unsigned int data,unsigned char in_or_out,unsigned char size);
typedef unsigned int(*O86Init)(GlobalDevice *__GlobStruct); //This is the type the function of
//Open86Init should be in devices, if the return isn't 0, then open86 panics
typedef void (*O86End)(void);
typedef unsigned char *(*O86GetName)(void); //not used?
typedef unsigned char *(*O86DevName)(void); //not used?
extern unsigned int cpu_level; //The CPU model to use
extern char *open86_dir; //This is the relative path to Open86
extern char *config_dir; //This is the relative path to the config file
extern char *dev_dir;    //This is the relative path to the devices
extern char *tmp_filename; //This is preallocated space to use a filename

extern unsigned char reset_segment;
extern unsigned char segment_override;

extern unsigned char compares; //This is used in string functions to determine if rep or repe should be used


typedef struct{
     unsigned char rm:3;
     unsigned char extra:3;
     unsigned char mod:2;
}
__attribute__((packed))mod_rm; //this struct is a described mod r/m byte..

typedef struct{
    void *ptr; //the calculated ptr
    mod_rm rm; //the rm byte
    unsigned short prev_ip; //the ip when the function started
}mod_rm_return; //not used?
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

SPEED_UP unsigned int MemRead8(unsigned char segment,unsigned short offset, unsigned char *buffer);
SPEED_UP unsigned int MemWrite8(unsigned char segment,unsigned short offset, unsigned char *buffer);
SPEED_UP unsigned int MemRead16(unsigned char segment,unsigned short offset, unsigned short *buffer);
SPEED_UP unsigned int MemWrite16(unsigned char segment,unsigned short offset, unsigned short *buffer);




//a few prototypes..
SPEED_UP unsigned short add16(unsigned short base,unsigned short adder);
SPEED_UP unsigned short sub16(unsigned short base,unsigned short subt);
SPEED_UP unsigned char sub8(unsigned char base,unsigned char subt);
SPEED_UP unsigned char add8(unsigned char base,unsigned char adder);
void *GetModRM8r();
void *GetModRM16r();
unsigned int push(unsigned short);
unsigned short pop();
void CallInt(unsigned char num);
void LockCPU();
void UnlockCPU();
unsigned char dev_ReadMemory8(unsigned int offset);

SPEED_UP void Process();

unsigned char EventInstalled(unsigned int target);
unsigned int GetEventIndex(unsigned int target);
typedef struct{
	unsigned int target;
	open86_message_event func;
}open86_event_func_list;

extern open86_event_func_list *EventFuncList;
extern unsigned int EventFuncList_size;
extern unsigned int EventFuncList_i;
void InitEventList();
unsigned char InstallMessageEvent(unsigned int target,open86_message_event func);
void DoCycle();
void x86Init();
//opcode prototypes(yea, a big list)
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
void group_f6();
void group_f7();
void cmp_modrm8_r8();
void cmp_modrm16_r16();
void inc_r16();
void dec_r16();
void and_modrm16_r16();
void and_modrm8_r8();
void cbw();
void cmps_m16();
void cmps_m8();
void repne();
void rep_repe();
void sub_modrm16_r16();
void add_modrm16_r16();
void sub_modrm8_r8();
void add_modrm8_r8();
void group_c0_through_d3();
void sti();
void mov_al_mem8();
void mov_ax_mem16();
void mov_mem16_ax();
void mov_mem8_al();
void mov_rm8_imm8();
void mov_rm16_imm16();
void add_modrm16_imm16();
void add_modrm16_imm8();
void add_modrm8_imm8();
void sub_modrm16_imm16();
void sub_modrm8_imm8();
void sub_modrm16_imm8();
void group_80();
void group_81();
void group_83();
void cmp_al_imm8();
void cmp_ax_imm16();
void ES_override();
void DS_override();
void CS_override();
void SS_override();
void aaa();
void daa();
void das();
void aas();
void group_d4();
void group_d5();
void les_m16();
void lds_m16();
void movs_m8();
void movs_m16();
void lods_m8();
void lods_m16();
void scas_m8();
void scas_m16();
void stos_m8();
void stos_m16();
void retf0();


extern volatile GlobalDevice *GlobStruct; //This is the allocated GlobStruct
//The pointer of GlobStruct is sent to all the devices in Open86Init()



#ifdef NDEBUG //the debug{} macro //not really used now, but still useful
#define debug if(1)
#else
#define debug if(0)
#endif



#endif


