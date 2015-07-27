#include "../_base.h"
/**This file contains all add and sub type functions(including cmp and cmps)**/
/**ALL 8086 add and sub instructions are coded, implemented, and tested!  (note some are in groups.c)**/
/**ALL 8086 cmp instructions are coded, implemented and basically tested! (note some are in groups.c)**/


/**Add and Sub helper functions(not opcodes)**/
// TODO (Jordan#4#): Get AF stuff working in this!!
SPEED_UP unsigned char sub8(unsigned char base,unsigned char subt){
    signed char result;
    if(subt>base){flags->cf=1;}else{flags->cf=0;}
    if((signed int)base-subt>127 || (signed int)base-subt<-127){
            flags->of=1;
        }else{
            flags->of=0;
        }
    result=base-subt;
    if(result==0){flags->zf=1;}else{flags->zf=0;}
    CalculatePF8(result); //do pf
    CalculateSF8(result); //do sf
    base=base&0xF;
    subt=subt&0xF; //isolate bottom nibble
    if((signed int)base-subt>15 || (signed int)base-subt<(-15)){
        flags->af=1;
    }else{
        flags->af=0;
    }
    return result;
}

SPEED_UP unsigned short sub16(unsigned short base,unsigned short subt){
    signed short result;
    unsigned short mirror;
    if(subt>base){flags->cf=1;}else{flags->cf=0;}
    if((signed int)base-subt>32767 || (signed int)base-subt<(-32767)){
            flags->of=1;
        }else{
            flags->of=0;
        }
    mirror=base-subt;
    result=mirror;
    if(result==0){flags->zf=1;}else{flags->zf=0;}
    CalculatePF16(result); //do pf
    CalculateSF16(result); //do sf
    base=base&0xF;
    subt=subt&0xF; //isolate bottom nibble
    if(((unsigned int)base-subt)>15 || ((unsigned int)base-subt)<-15){
        flags->af=1;
    }else{
        flags->af=0;
    }
    return mirror;
}


SPEED_UP unsigned char add8(unsigned char base,unsigned char adder){
    signed char result;
    if((unsigned int)adder+(unsigned int)base>(unsigned int)255){flags->cf=1;}else{flags->cf=0;}
    if((signed int)base+adder>127 || (signed int)base+adder<-127){
            flags->of=1;
        }else{
            flags->of=0;
        }
    result=base+adder;
    if(result==0){flags->zf=1;}else{flags->zf=0;}
    CalculatePF8(result); //do pf
    CalculateSF8(result); //do sf
    base=base&0xF;
    adder=adder&0xF; //isolate bottom nibble
    if((signed int)base+adder>15 || (signed int)base+adder<(-15)){
        flags->af=1;
    }else{
        flags->af=0;
    }
    return result;
}

SPEED_UP unsigned short add16(unsigned short base,unsigned short adder){
    signed short result;
    if((unsigned int)adder+(unsigned int)base>(unsigned int)65535){flags->cf=1;}else{flags->cf=0;}
    if((signed int)base+adder>32767 || (signed int)base+adder<-32767){
            flags->of=1;
        }else{
            flags->of=0;
        }
    result=base+adder;
    if(result==0){flags->zf=1;}else{flags->zf=0;}
    CalculatePF16(result); //do pf
    CalculateSF16(result); //do sf
    base=base&0xF;
    adder=adder&0xF; //isolate bottom nibble
    if((signed int)base+adder>15 || (signed int)base+adder<(-15)){
        flags->af=1;
    }else{
        flags->af=0;
    }
    return result;
}



/**These are opcodes**/
void sub_al_ib(){
    unsigned char tmp;
    ip++;
    MemRead8(CS,ip,&tmp);
    *gregs8[AL]=sub8(*gregs8[AL],tmp);

}

void sub_ax_iw(){
    unsigned short iw;
    ip++;
    MemRead16(CS,ip,&iw);
    gregs16[AX]=sub16(gregs16[AX],iw);
    ip++;
}


void add_al_ib(){
    unsigned char tmp;
    ip++;
    MemRead8(CS,ip,&tmp);
    *gregs8[AL]=add8(*gregs8[AL],tmp);
}

void add_ax_iw(){
    unsigned short iw;
    ip++;
    MemRead16(CS,ip,&iw);
    gregs16[AX]=add16(gregs16[AX],iw);
    ip++;
}



void sub_modrm8_r8(){
    unsigned char *ptr;
    unsigned char *ptr2;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read8(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=ptr+(unsigned long)core;
        *ptr=sub8(*ptr,*gregs8[rm[0].extra]); //only flags are affected!
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        *gregs8[rm[0].rm]=sub8(*gregs8[rm[0].rm], *gregs8[rm[0].extra]);
        return;
    }
    panic("errors not yet handled!!");
}

void add_modrm8_r8(){
    unsigned char *ptr;
    unsigned char *ptr2;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read8(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=ptr+(unsigned long)core;
        *ptr=add8(*ptr,*gregs8[rm[0].extra]); //only flags are affected!
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        *gregs8[rm[0].rm]=add8(*gregs8[rm[0].rm], *gregs8[rm[0].extra]);
        return;
    }
    panic("errors not yet handled!!");
}


void sub_modrm16_r16(){
    unsigned short *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        //printf("tmp: %i\n",ptr);
        ptr=(unsigned long)ptr+(unsigned long)core;
        //gotta remeber this retarded pointer addition crap..
        *ptr=sub16(*ptr,gregs16[rm[0].extra]); //only flags are affected!
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        gregs16[rm[0].rm]=sub16(gregs16[rm[0].rm], gregs16[rm[0].extra]);
        return;
    }
    panic("errors not yet handled!!");
}

void add_modrm16_r16(){
    unsigned short *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        //printf("tmp: %i\n",ptr);
        ptr=(unsigned long)ptr+(unsigned long)core;
        //gotta remeber this retarded pointer addition crap..
        *ptr=add16(*ptr,gregs16[rm[0].extra]); //only flags are affected!
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        gregs16[rm[0].rm]=add16(gregs16[rm[0].rm], gregs16[rm[0].extra]);
        return;
    }
    panic("errors not yet handled!!");
}

void cmp_modrm8_r8(){
    unsigned char *ptr;
    unsigned char *ptr2;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read8(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=ptr+(unsigned long)core;
        sub8(*ptr,*gregs8[rm[0].extra]); //only flags are affected!
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        sub8(*gregs8[rm[0].rm], *gregs8[rm[0].extra]);
        return;
    }
    panic("errors not yet handled!!");
}

void cmp_modrm16_r16(){
    unsigned short *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        //printf("tmp: %i\n",ptr);
        ptr=(unsigned long)ptr+(unsigned long)core;
        //gotta remeber this retarded pointer addition crap..
        sub16(*ptr,gregs16[rm[0].extra]); //only flags are affected!
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        sub16(gregs16[rm[0].rm], gregs16[rm[0].extra]);
        return;
    }
    panic("errors not yet handled!!");
}


void cmp_r8_modrm8(){
    unsigned char *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read8(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=ptr+(unsigned long)core;
        //*gregs8[rm[0].extra] is the destination, *ptr is source/operand
        sub8(*gregs8[rm[0].extra],*ptr);
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        //*gregs8[rm[0].extra] is the destination, *gregs8[rm[0].rm] is the source/operand
        sub8(*gregs8[rm[0].extra],*gregs8[rm[0].rm]);
        return;
    }
    panic("errors not yet handled!!");
}

void cmp_r16_modrm16(){
    unsigned short *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=ptr+(unsigned long)core;
        //*gregs8[rm[0].extra] is the destination, *ptr is source/operand
        sub16(gregs16[rm[0].extra],*ptr);
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        //*gregs8[rm[0].extra] is the destination, *gregs8[rm[0].rm] is the source/operand
        sub16(gregs16[rm[0].extra],gregs16[rm[0].rm]);
        return;
    }
    panic("errors not yet handled!!");
}


void cmp_al_imm8(){
	unsigned char tmp;
	ip++;
	MemRead8(CS,ip,&tmp);
	sub8(*gregs8[AL],tmp);
}

void cmp_ax_imm16(){
	unsigned short tmp;
	ip++;
	MemRead16(CS,ip,&tmp);
	ip++;
	sub16(gregs16[AX],tmp);
}




void cmps_m8(){
	unsigned char tmp,tmp2;
	unsigned char error;
	compares=1;
	MemRead8(CurrentSegment,gregs16[SI],&tmp);
	//if(error!=0){panic("errors not yet handled!\n");}
	MemRead8(ES,gregs16[DI],&tmp2);
	sub8(tmp,tmp2);
	if(flags->df==0){ //which direction
		gregs16[SI]++;
		gregs16[DI]++;
	}else{
		gregs16[SI]--;
		gregs16[DI]--;
	}
}

void cmps_m16(){
	unsigned short tmp,tmp2;
	unsigned char error;
	compares=1;
	MemRead16(CurrentSegment,gregs16[SI],&tmp);
	//if(error!=0){panic("errors not yet handled!\n");}
	MemRead16(ES,gregs16[DI],&tmp2);
	sub16(tmp,tmp2);
	if(flags->df==0){ //determine direction
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



