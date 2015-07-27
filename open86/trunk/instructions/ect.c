#include "../_base.h"
/**This file has opcodes that aren't really able to be catagorized**/

void nop(){ //used as a debug function for now..
     printf("AX: %x\nCX: %x\nDX: %x\nBX: %x\nSP: %x\nBP: %x\nSI: %x\nDI: %x\n",gregs16[0],gregs16[1],gregs16[2],gregs16[3],gregs16[4],gregs16[5],gregs16[6],gregs16[7]);
     printf("ES: %x\nCS: %x\nSS: %x\nDS: %x\nIS(imaginery segment) %x\n",sregs[0],sregs[1],sregs[2],sregs[3],sregs[IS]);
     printf("flags: %x\nIP: %x\nEIP(private): %x\n",flagreg,ip,(sregs[CS]<<4)+ip);
     printf("viewable flags: ");
     if(flags->cf==1){printf("CF ");}
     if(flags->pf==1){printf("PF ");}
     if(flags->af==1){printf("AF ");}
     if(flags->zf==1){printf("ZF ");}
     if(flags->sf==1){printf("SF ");}
     if(flags->tf==1){printf("TF ");}
     if(flags->_if==1){printf("IF ");}
     if(flags->df==1){printf("DF ");}
     if(flags->of==1){printf("OF ");}
     printf("\n");
}

void unknown(){ //unknown opcode error(eventually will be replaced by an exception)
     panic("Unknown Opcode!!\n");
}

void hlt(){ //hlt
     panic("HALTED! interrupts not supported yet!\n");
}


/**flag control**/
void clc(){ //clear carry flag
    flags->cf=0;
}

void cld(){ //clear direction flag
    flags->df=0;
}

void cli(){
    flags->_if=0;
}

void cmc(){
    flags->cf=flags->cf^1;
}


void sti(){
	flags->_if=1;
}




void cbw(){
	if((*gregs8[AL]&0x80)==0){ //use bitwise instructions for a slight speed boost..
		*gregs8[AH]=0;
	}else{
		*gregs8[AH]=0xFF;
	}
}


void rep_repe(){
	ip++;
	segment_override=0; //this is only used in prefixes, so we don't need a "pre instruction" function
	//also note that the rep prefix comes before the segment prefix..
	if(gregs16[CX]==0){return;}
	while((volatile)gregs16[CX]!=0){
		reset_segment=0; //do this to preserve segment overrides..
		Process(); //get next instruction and such..
		if(segment_override==0){
			ip--; //it will do the autoincrement of ip so..

			gregs16[CX]--;
			if((volatile)compares==1){
				if((volatile)flags->zf==0){
					compares=0;
					return;
				}
		}
		}
	}
	segment_override=0;
	reset_segment=1;
	//ip++;
}

void repne(){
	ip++;
	segment_override=0;
	if(gregs16[CX]==0){return;}
	while(gregs16[CX]!=0){
		reset_segment=0;
		Process(); //get next instruction and such..
		ip--; //it will do the autoincrement of ip so..
		gregs16[CX]--;
		if(compares==1){
			if(flags->zf==1){
				compares=0;
				return;
			}
		}
	}
	reset_segment=1;
	segment_override=0;
	//ip++; //get past the  string instruction
}

/**No way to test overrides with rep prefixes..**/

void ES_override(){
	CurrentSegment=ES;
	StackSegment=ES;
	reset_segment=0;
	segment_override=1;
}
void DS_override(){
	CurrentSegment=DS;
	StackSegment=DS;
	reset_segment=0;
	segment_override=1;
}
void CS_override(){
	CurrentSegment=CS;
	StackSegment=CS;
	reset_segment=0;
	segment_override=1;
}
void SS_override(){
	CurrentSegment=SS;
	StackSegment=SS;
	reset_segment=0;
	segment_override=1;
}



/*
Basic string functions for open86.
By: Alboin, 2-26-07

TODO: Test this file! It is in need of
testing of any kind.
*/

#include "../_base.h"

SPEED_UP void advance_string_pointer8() {
if(flags->df==0){
gregs16[SI]++;
gregs16[DI]++;
}else{
gregs16[SI]--;
gregs16[DI]--;
}
}

SPEED_UP void advance_string_pointer16() {
if(flags->df==0){
gregs16[SI]++;
gregs16[SI]++;
gregs16[DI]++;
gregs16[DI]++;
}else{
gregs16[SI]--;
gregs16[SI]--;
gregs16[DI]--;
gregs16[DI]--;
}
}

/*****************************
MOVS
*****************************/

/* Opcode: 0xA4 */
void movs_m8(){
unsigned char tmp;

/* Read place in RAM. */
MemRead8(CurrentSegment,gregs16[SI],&tmp);

/* Write to the correct place. */
MemWrite8(ES,gregs16[DI],&tmp);

/* Advance pointer. */
advance_string_pointer8();

}

/* Opcode: 0xA5 */
void movs_m16(){
unsigned short tmp;

/* Read place in RAM. */
MemRead16(CurrentSegment,gregs16[SI],&tmp);

/* Write to the correct place. */
MemWrite16(ES,gregs16[DI],&tmp);

/* Advance pointer. */
advance_string_pointer16();
}

/*****************************
LODS
*****************************/

/* Opcode: 0xAC */
void lods_m8(){
unsigned char tmp;

MemRead8(CurrentSegment,gregs16[SI],&tmp);
gregs8[AL] = tmp;

/* Advance pointer. */
advance_string_pointer8();
}

/* Opcode: 0xAD */
void lods_m16(){
unsigned short tmp;

MemRead16(CurrentSegment,gregs16[SI],&tmp);
gregs16[AX] = tmp;

/* Advance pointer. */
advance_string_pointer16();
}

/*****************************
SCAS
*****************************/

/* Opcode: 0xAE */
void scas_m8(){
unsigned char tmp;
compares=1;
MemRead8(ES,gregs16[DI],&tmp);
sub8(gregs8[AL],tmp);

/* Advance pointer. */
advance_string_pointer8();
}

/* Opcode: 0xAF */
void scas_m16(){
unsigned short tmp;
compares=1;
MemRead16(ES,gregs16[DI],&tmp);
sub16(gregs16[AX],tmp);

/* Advance pointer. */
advance_string_pointer16();
}

/*****************************
STOS
*****************************/

/* Opcode: 0xAA */
void stos_m8(){
/* Write to the correct place. */
MemWrite8(ES, gregs16[DI], gregs8[AL]);

/* Advance pointer. */
advance_string_pointer8();
}

/* Opcode: 0xAB */
void stos_m16(){
/* Write to the correct place. */
MemWrite16(ES, gregs16[DI], &gregs16[AX]);

/* Advance pointer. */
advance_string_pointer16();
}




/******MODRM templates!!!!
these are some simple templates to make using modrm instructions much easier to use
void template_modrm16_r16(){
    unsigned short *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_write16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
        //*ptr is the destination, gregs16[rm[0].extra] is the source/operand
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        //gregs16[rm[0].rm] is the destination, gregs16[rm[0].extra] is the source/operand
        return;
    }
    panic("errors not yet handled!!");
}

void template_modrm8_r8(){
    unsigned char *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_write8(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=ptr+(unsigned long)core;
        //*ptr is the destination, *gregs8[rm[0].extra] is the source/operand
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        //*gregs8[rm[0].rm] is the destination, *gregs8[rm[0].extra] is the source/operand
        return;
    }
    panic("errors not yet handled!!");
}


void template_r8_modrm8(){
    unsigned char *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read8(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=ptr+(unsigned long)core;
        //*gregs8[rm[0].extra] is the destination, *ptr is source/operand
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        //*gregs8[rm[0].extra] is the destination, *gregs8[rm[0].rm] is the source/operand
        return;
    }
    panic("errors not yet handled!!");
}

void template_r16_modrm16(){
    unsigned short *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
        //gregs16[rm[0].extra] is the destination, *ptr is source/operand
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        //gregs16[rm[0].extra] is the destination, gregs16[rm[0].rm] is the source/operand
        return;
    }
    panic("errors not yet handled!!");
}

void template_sreg16_modrm16(){
    unsigned short *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
        //sregs[rm[0].extra] is the destination, *ptr is source/operand
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        //gregs16 should USUALLY be the proper one, but being opcode specific this could be
        //other things..
        //sregs[rm[0].extra] is the destination, gregs16[rm[0].rm] is the source/operand
        return;
    }
    panic("errors not yet handled!!");
}


void template_modrm16_sreg16(){
    unsigned short *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_write16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
        //*ptr is the destination, sregs[rm[0].extra] is the source/operand
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        //gregs16 should USUALLY be the proper one, but being opcode specific this could be
        //other things..(including an invalid opcode exception)
        //gregs16[rm[0].rm] is the destination, sregs[rm[0].extra] is the source/operand
        return;
    }
    panic("errors not yet handled!!");
}


*/



