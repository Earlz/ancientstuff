/**Copyright Jordan Earls <http://Earlz.biz.tm>
All rights reserved.
See "copyright.txt" for information on the copyright of this project and source file
This file is part of the "x86Lib" project.
**/
#ifndef X86LIB
#define X86LIB
#include <iostream>
#include <stdint.h>
#include <string>
#include "config.h"
namespace x86Lib{

//32 bit register macros
static const int EAX=0;
static const int ECX=1;
static const int EDX=2;
static const int EBX=3;
static const int ESP=4;
static const int EBP=5;
static const int ESI=6;
static const int EDI=7;


//16 bit register macros
static const int AX=0;
static const int CX=1;
static const int DX=2;
static const int BX=3;
static const int SP=4;
static const int BP=5;
static const int SI=6;
static const int DI=7;

//8 bit register macros
static const int AL=0;
static const int CL=1;
static const int DL=2;
static const int BL=3;
static const int AH=4;
static const int CH=5;
static const int DH=6;
static const int BH=7;

/**The way segment overrides work is that it changes these values
to what they should be for that instruction, then changes them back to the "default"
**/
extern int ES;
extern int CS;
extern int SS;
extern int DS;
extern int FS;
extern int GS;


//segment registers constants(the defaults)
static const int cES=0;
static const int cCS=1;
static const int cSS=2;
static const int cDS=3;
static const int cFS=4;
static const int cGS=5;
static const int cIS=6; //this is an imaginary segment only used for direct segment overrides
//for instance it would be used in mov [1000:bx],ax



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


extern FLAGS freg;
extern uint16_t seg[7];
extern volatile uint32_t reg32[8];
extern volatile uint16_t *regs16[];
extern volatile uint8_t *regs8[];
extern uint32_t eip;

extern uint8_t op_cache[];



typedef void (*opcode)(void); //This is the function type opcodes should be in
extern opcode *Opcodes; //The current opcode table
static inline void ResetSegments(){
	ES=cES;
	CS=cCS;
	SS=cSS;
	DS=cDS;
	FS=cFS;
	GS=cGS;
}

static inline void SetSegments(int segm){
	ES=segm;
	CS=segm;
	SS=segm;
	DS=segm;
	FS=segm;
	GS=segm;
}


/**Hooks to be filled in...**/

/**This class is for the x86Lib to read/write to Physical Memory...
The client application need not worry about paging, or virtual memory
or anything of that sort...they should only worry about physical memory
and memory mapped devices(both of which, they completely control)
I rely on this to automatically round over addresses if A20 is not set**/
class PhysMemory{ //These functions MUST be filled out by the actual app..
	uint32_t size;
	uint8_t *ptr; //for free use....
	bool A20Flag;
	public:
	/*Hooks that must be filled in*/
	PhysMemory(); //This should
	~PhysMemory();
	uint32_t GetSize();
	bool CanRead(uint32_t off); //for gaps and such?
	bool CanWrite(uint32_t off);
	void WriteByte(uint32_t ptr,uint8_t val);
	void WriteWord(uint32_t ptr,uint16_t val);
	void WriteDword(uint32_t ptr,uint32_t val);
	uint8_t ReadByte(uint32_t ptr);
	uint16_t ReadWord(uint32_t ptr);
	uint32_t ReadDword(uint32_t ptr);

};


/**This is for x86Lib to read/write from/to ports.
No guidelines are actually demanded here as long as everything is defined**/
class x86Ports{ //must define this hook


	public:
	x86Ports();
	~x86Ports();
	void WriteByte(uint16_t port,uint8_t val);
	void WriteWord(uint16_t port,uint16_t val);
	void WriteDword(uint16_t port,uint32_t val);
	uint8_t ReadByte(uint16_t port);
	uint16_t ReadWord(uint16_t port);
	uint32_t ReadDword(uint16_t port);
};


void Push16(uint16_t);
uint16_t Pop16();
void Jmp16_near16(uint16_t);
void Jmp16_near8(uint8_t);
void Int16(uint8_t);


using std::cout;
using std::hex;
using std::endl;
class x86CPU{ //The master control class...


	public:
	x86CPU(PhysMemory *mmem,x86Ports *pport,uint32_t,uint8_t);
	~x86CPU();
	void Cycle(); //Do one CPU Opcode
	void DumpState(){
		cout << "EAX: "<< hex << reg32[EAX] <<endl;
		cout << "ECX: "<< hex << reg32[ECX] <<endl;
		cout << "EDX: "<< hex << reg32[EDX] <<endl;
		cout << "EBX: "<< hex << reg32[EBX] <<endl;
		cout << "ESP: "<< hex << reg32[ESP] <<endl;
		cout << "EBP: "<< hex << reg32[EBP] <<endl;
		cout << "ESI: "<< hex << reg32[ESI] <<endl;
		cout << "EDI: "<< hex << reg32[EDI] <<endl;

		cout << "CS: " << hex << seg[cCS] << endl;
		cout << "SS: " << hex << seg[cSS] << endl;
		cout << "DS: " << hex << seg[cDS] << endl;
		cout << "ES: " << hex << seg[cES] << endl;
		cout << "FS: " << hex << seg[cFS] << endl;
		cout << "GS: " << hex << seg[cGS] << endl;
		cout << "EIP: " << hex << eip << endl;

		cout << "--Flags:" <<hex << *(uint16_t*)&freg<< endl;
		cout << "CF: " << (int)freg.cf << endl;
		cout << "PF: " << (int)freg.pf << endl;
		cout << "AF: " << (int)freg.af << endl;
		cout << "ZF: " << (int)freg.zf << endl;
		cout << "SF: " << (int)freg.sf << endl;
		cout << "TF: " << (int)freg.tf << endl;
		cout << "IF: " << (int)freg._if << endl;
		cout << "DF: " << (int)freg.df << endl;
		cout << "OF: " << (int)freg.of << endl;
	}
	/**Note! This accepts just normal interrupts! it does not resolve IRQs to CPU interrupt numbers**/
	void Int(uint8_t num){ //external interrupt
		if(num==2){ //NMI
			eip--; //A workaround...I am not sure why I must do this yet...
			Int16(num);
			eip++;
		}else{
			if(freg._if==1){
				eip--;
				Int16(num);
				eip++;
			}
		}
	}








};

/**This maps virtual memory to physical memory, in short, it is the MMU
All of these take segment and offsets as addresses...
Note! The segment to be given is the constant internal segment number!
Not the value of the segment!**/
class x86Memory{

	public:
	x86Memory();
	~x86Memory();
	bool CanRead(uint8_t segm,uint32_t off);
	bool CanWrite(uint8_t segm,uint32_t off);
	void WriteByte(uint8_t segm,uint32_t off,uint8_t val);
	void WriteWord(uint8_t segm,uint32_t off,uint16_t val);
	void WriteDword(uint8_t segm,uint32_t off,uint32_t val);
	uint8_t ReadByte(uint8_t segm,uint32_t off);
	uint16_t ReadWord(uint8_t segm,uint32_t off);
	uint32_t ReadDword(uint8_t segm,uint32_t off);
};




extern x86Memory mem;
extern PhysMemory *pmem;
extern x86Ports *ports;




typedef struct{
     unsigned char rm:3;
     unsigned char extra:3;
     unsigned char mod:2;
}
__attribute__((packed))mod_rm16; //this struct is a described mod r/m byte..

//Note, this will re-cache op_cache, so do not use it afterward
//Also, eip should be on the modrm byte!
//On return, it is on the last byte of the modrm block, so no advancement needed unelss there is an immediate
//Also, this will advance EIP upon exiting the opcode(deconstruction)
class ModRM16{ //This is the best thing I have ever done...
	//I love this class so much...am I cheating on her? lol
	protected:
	bool use_ss;
	bool op_specific;
	private:
	mod_rm16 modrm;
	int16_t GetRegD(){ //This returns the register displacement value
		switch(modrm.rm){
			case 0:
			return *regs16[BX]+*regs16[SI];
			break;
			case 1:
			return *regs16[BX]+*regs16[DI];
			break;
			case 2:
			use_ss=1;
			return *regs16[BP]+*regs16[SI];
			break;
			case 3:
			use_ss=1;
			return *regs16[BP]+*regs16[DI];
			break;
			case 4:
			return *regs16[SI];
			break;
			case 5:
			return *regs16[DI];
			case 6: //immediate Displacement only, so no register displace..
			return 0;
			break;
			case 7:
			return *regs16[BX];
			break;
		}
		return 0;
	}

	int16_t GetDisp(){
		uint16_t reg;
		reg=GetRegD();
		if(modrm.rm==6){ //Don't worry, it's safe...
			use_ss=1;
			reg=*regs16[BP];
		}
		switch(modrm.mod){
			case 0: //no displacement

			if(modrm.rm==6){ //if only word displacement...
				use_ss=0;
				//eip++;
				//eip++;
				return *(uint16_t*)&op_cache[1];
			}else{
				return reg;
			}
			break;
			case 1: //byte displacement(signed)
			//eip++;
			return (signed)reg+(signed)op_cache[1];
			break;
			case 2: //word displacement(signed)
			return (signed)reg+(signed)(*(uint16_t*)&op_cache[1]);
			break;
			case 3: //opcode specific...
			op_specific=1;
			return 0;
			break;
		}
		return 0;
	}

	public:

	ModRM16(){
		use_ss=0;
		op_specific=0;
		*(uint32_t*)&op_cache=mem.ReadDword(cCS,eip);
		*(uint8_t*)&modrm=op_cache[0];
		//cout << hex << (int)modrm.rm << endl;
		//knowing how to do type casting owns!
	}

	~ModRM16(){
		eip+=GetLength()-1;
		//eip--;
	}

	//The r suffix means /r, which means for op_specific=1, use general registers
	uint8_t ReadByter(){
		use_ss=0;
		op_specific=0;
		uint16_t disp=GetDisp();
		if(op_specific==1){
			return *regs8[modrm.rm];
		}else{
			if(use_ss==1){
				return mem.ReadByte(SS,disp);
			}else{
				return mem.ReadByte(DS,disp);
			}
		}
	}

	uint16_t ReadWordr(){
		use_ss=0;
		op_specific=0;
		uint16_t disp=GetDisp();
		if(op_specific==1){
			//cout << "h" << endl;
			return *regs16[modrm.rm];
		}else{

			if(use_ss==1){
				return mem.ReadWord(SS,disp);
			}else{
				return mem.ReadWord(DS,disp);
			}
		}
	}

	void WriteByter(uint8_t byte){
		use_ss=0;
		op_specific=0;
		uint16_t disp=GetDisp();
		if(op_specific==1){
			*regs8[modrm.rm]=byte;
		}else{

			if(use_ss==1){
				mem.WriteByte(SS,disp,byte);
			}else{
				mem.WriteByte(DS,disp,byte);
			}
		}
	}
	void WriteWordr(uint16_t word){
		use_ss=0;
		op_specific=0;
		uint16_t disp=GetDisp();
		if(op_specific==1){
			*regs16[modrm.rm]=word;
		}else{

			if(use_ss==1){
				mem.WriteWord(SS,disp,word);
			}else{
				mem.WriteWord(DS,disp,word);
			}
		}
	}
	uint8_t GetLength(){ //This returns how many total bytes the modrm block consumes
		if((modrm.mod==0) && (modrm.rm==6)){
			return 3;
		}
		switch(modrm.mod){
			case 0:
			return 1;
			break;
			case 1:
			return 2;
			break;
			case 2:
			return 3;
			break;
			case 3:
			return 1;
			break;
		}
		return 1; //should never reach here, but to avoid warnings...
	} //that was easier than I first thought it would be...
	uint8_t GetExtra(){ //Get the extra fied from mod_rm
		return modrm.extra;
	}

}; //I hope that SIB and ModR/M32 will be this good!




extern volatile bool string_compares;

inline void InstallOp(unsigned char num,opcode func);
void tfunc();






/**Exceptions...**/
//codes...
static const uint32_t CLIHLT_EXCP=1; //cli/hlt...nothing to do
static const uint32_t MEM_ACCESS_EXCP=3; //Memory Access Error...(can actually be page fault, or GPF, or stack fault...
static const uint32_t TRIPLE_FAULT_EXCP=0x10000; //Triple fault...This should be ORd with the last exception

//CPU Exceptions(interrupt handled)
static const uint32_t DIV0_IEXCP=0xF000; //Divide by zero exception
static const uint32_t DEBUG_IEXCP=0xF001; //Debug exception
static const uint32_t NMI_IEXCP=0xF002; //NMI
static const uint32_t BREAK_IEXCP=0xF003; //Breakpoint/int 3
static const uint32_t OVERFLOW_IEXCP=0xF004; //Overflow/into
static const uint32_t BOUNDS_IEXCP=0xF005; //Bounds Check
static const uint32_t UNK_IEXCP=0xF006; //unknown opcode
static const uint32_t UNDEV_IEXCP=0xF007; //Unknown device
static const uint32_t DOUBLE_FAULT_IEXCP=0xF008;
static const uint32_t SEG_OVERRUN_IEXCP=0xF009; //Co-processor segment overrun..(not used after i486
static const uint32_t ITSS_IEXCP=0xF00A; //Invalid TSS
static const uint32_t ISEG_IEXCP=0xF00B; //Invalid/non-existent segment
static const uint32_t STACK_IEXCP=0xF00C; //Stack Exception
static const uint32_t GPF_IEXCP=0xF00D; //GPF
static const uint32_t PAGE_FAULT_IEXCP=0xF00E;
static const uint32_t RESERVED_IEXCP=0xF00F; //Reserved by intel, so internal use?
static const uint32_t FLOAT_ERROR_IEXCP=0xF010; //Floating Point Error..
static const uint32_t ALIGN_IEXCP=0xF011; //Alignment Check...





class Default_excp{ //Internal only...these should never happen when released...

	public:
	Default_excp(std::string file_,std::string func_,int line_){
		file=file_;
		func=func_;
		line=line_;
	}

	std::string file;
	std::string func;
	int line;
};

class CpuPanic_excp{ //used for fatal CPU errors, such as triple fault..

	public:
	CpuPanic_excp(std::string desc_,uint32_t code_){
		desc=desc_;
		code=code_;
	}
	std::string desc;
	uint32_t code;
};

class CpuInt_excp{ //Used internally for handling interrupt exceptions...
	public:
	CpuInt_excp(uint32_t code_){
		code=code_;
	}
	uint32_t code;
};

class Mem_excp{ //Exclusively for the Memory Classes, these are caught and then a more appropriate excp is thrown
	public:
	Mem_excp(uint32_t address_){
		address=address_;
	}
	uint32_t address;
};


static inline uint16_t SignExtend8(uint8_t val){ //sign extend a byte to a word
	if((val&0x80)!=0){
		return 0xFF00|val;
	}else{
		return val;
	}
}



}







namespace op16{
using namespace x86Lib;
void unknown_op();
void mov_r8_imm8(); //Tested, pass #1;
void hlt(); //Tested, pass #1;
void nop(); //Tested, pass #1;
void mov_r16_imm16(); //Tested, pass #1;
void jmp_rel8(); //Tested, pass #1;
void sub_al_imm8(); //Tested, pass #1;
void sub_ax_imm16(); //Tested, pass #1;
void ja_rel8();
void jnc_rel8();
void jbe_rel8();
void jc_rel8(); //Tested, pass #1;
void jz_rel8(); //Tested, pass #1;
void jnz_rel8(); //Tested, pass #1;
void jp_rel8(); //Tested, pass #1;
void jnp_rel8(); //Tested, pass #1;
void jg_rel8(); //Tested, pass #1;
void jge_rel8();
void jle_rel8();
void jl_rel8(); //Tested, pass #1;
void jo_rel8();
void jno_rel8();
void js_rel8();
void jns_rel8();
void mov_sr_rm16(); //Tested, pass #1;
void mov_rm16_sr(); //Tested, pass #1;
void pop_r16(); //Tested, pass #1;
void push_imm16(); //Tested, pass #1;
void push_r16(); //Tested, pass #1;
void push_es(); //Tested, pass #1;
void push_cs(); //Tested, pass #1;
void push_ss(); //Tested, pass #1;
void push_ds(); //Tested, pass #1;
void pop_es(); //Tested, pass #1;
void pop_ss(); //Tested, pass #1;
void pop_ds(); //Tested, pass #1;
void mov_r16_rm16(); //Tested, pass #1;
void mov_rm16_r16(); //Tested, pass #1;
void call_rel16(); //Tested, pass #1;
void retn(); //Tested, pass #1;
void loop_rel8(); //Tested, pass #1;
void pre_cs_override(); //Tested, pass #1;
void pre_ds_override(); //Tested, pass #1;
void pre_es_override(); //Tested, pass #1;
void pre_ss_override(); //Tested, pass #1;
void movsw(); //Tested, pass #1;
void movsb(); //Tested, pass #1;
void clc();
void cld(); //Tested, pass #1;
void cli(); //Tested, pass #1;
void stc();
void std_(); //Tested, pass #1;
void sti(); //Tested, pass #1;
void rep(); //Tested, pass #1;(only rep, not conditionals)
void out_imm8_al(); //Tested, pass #1;
void out_imm8_ax(); //Tested, pass #1;
void call_imm16_imm16(); //Tested, pass #1;
void retf(); //Tested, pass #1;
void int_imm8(); //Tested, pass #1;
void iret(); //Tested, pass #1;
void into();
void int3();
void in_al_imm8();
void in_ax_imm8();
void add_al_imm8(); //Tested, pass #1;
void add_ax_imm8(); //Tested, pass #1;
void sub_rm8_r8(); //Tested, pass #1;
void group_80(); //Tested, pass #1;
void sub_rm8_r8();
void sub_r8_rm8();
void sub_rm16_r16();
void sub_r16_rm16();
void group_81(); //Tested, pass #1;
void add_rm8_r8();
void add_rm16_r16();
void add_r8_rm8();
void add_r16_rm16();
void mov_al_off8();
void mov_ax_off16();
void mov_rm8_r8();
void mov_r8_rm8();
void mov_off8_al(); //Tested, pass #1;
void mov_off16_ax(); //Tested, pass #1;
void mov_m8_imm8(); //Tested, pass #1;
void mov_m16_imm16();  //Tested, pass #1;//currently have 85 instructions or prefixes implemented(actually more, not counting group instructions)
void cmp_rm8_r8();
void cmp_rm16_r16();
void cmp_r8_rm8();
void cmp_r16_rm16();
void cmp_al_imm8(); //Tested, pass #1;
void cmp_ax_imm16(); //Tested, pass #1;
void group_83(); //Tested, pass #1;


//Oh God how I hate prototyping and adding the opcodes to the master InstallOp list...




/**Group Include Functions(not direct opcodes)**/
void sub_rm8_imm8(ModRM16&); //group 0x80 /5
void sub_rm16_imm16(ModRM16&);
void add_rm8_imm8(ModRM16&);
void add_rm16_imm16(ModRM16&);
void cmp_rm8_imm8(ModRM16&);
void cmp_rm16_imm16(ModRM16&);
void sub_rm16_imm8(ModRM16&);
void add_rm16_imm8(ModRM16&);
void cmp_rm16_imm8(ModRM16&);




}

#ifdef SDEBUG
//op16_debug just gives debugging info and tracing info for op16
namespace op16_debug{

void unknown_op();
void mov_r8_imm8();
void hlt();
void mov_r16_imm16();
void jmp_rel8();



}

#endif









#endif








