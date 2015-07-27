#include "../_base.h"

/**This file has all of the bitwise instructions.
This includes and, or, xor, shifts, not, and more
**/


/**Functions that make things a bit simpler: these are NOT opcodes!**/
SPEED_UP unsigned char and8(unsigned char op1,unsigned char op2){
    unsigned char result;
    result=op1&op2;
    if(result==0){flags->zf=0;}
    CalculatePF8(result);
    CalculateSF8(result);
    flags->cf=0;
    flags->of=0;
    return result;
}

SPEED_UP unsigned short and16(unsigned short op1,unsigned short op2){
    unsigned short result;
    result=op1&op2;
    if(result==0){flags->zf=0;}
    CalculatePF16(result);
    CalculateSF16(result);
    flags->cf=0;
    flags->of=0;
    return result;
}

SPEED_UP unsigned char or8(unsigned char op1,unsigned char op2){
    unsigned char result;
    result=op1|op2;
    if(result==0){flags->zf=0;}
    CalculatePF8(result);
    CalculateSF8(result);
    flags->cf=0;
    flags->of=0;
    return result;
}

SPEED_UP unsigned short or16(unsigned short op1,unsigned short op2){
    unsigned short result;
    result=op1|op2;
    if(result==0){flags->zf=0;}
    CalculatePF16(result);
    CalculateSF16(result);
    flags->cf=0;
    flags->of=0;
    return result;
}

SPEED_UP unsigned char xor8(unsigned char op1,unsigned char op2){
    unsigned char result;
    result=op1^op2;
    if(result==0){flags->zf=0;}
    CalculatePF8(result);
    CalculateSF8(result);
    flags->cf=0;
    flags->of=0;
    return result;
}


SPEED_UP unsigned short xor16(unsigned short op1,unsigned short op2){
    unsigned short result;
    result=op1^op2;
    if(result==0){flags->zf=0;}
    CalculatePF16(result);
    CalculateSF16(result);
    flags->cf=0;
    flags->of=0;
    return result;
}

SPEED_UP unsigned char not8(unsigned char op1){
    unsigned char result;
    result=~op1;
    //if(result==0){flags->zf=0;}
    //CalculatePF8(result);
    //CalculateSF8(result);
    //flags->cf=0;
    //flags->of=0;
    return result;
}

SPEED_UP unsigned short not16(unsigned short op1){
    unsigned short result;
    result=~op1;
    //if(result==0){flags->zf=0;}
    //CalculatePF8(result);
    //CalculateSF8(result);
    //flags->cf=0;
    //flags->of=0;
    return result;
}

/* This sets some common flags. */
void set_flags8(unsigned char res) {
	CalculatePF8(res);
   	CalculateSF8(res);
   	if(res==0)
		flags->zf=1;
	else
		flags->zf=0;
}

/* More flagage. */
void set_flags16(unsigned short res) {
	CalculatePF16(res);
   	CalculateSF16(res);
   	if(res==0)
		flags->zf=1;
	else
		flags->zf=0;
}

/*****************************
	      SHR
*****************************/

inline unsigned char shr8(unsigned char op1,unsigned char op2) {
	unsigned char tmp_count = op2 & 0x1f;
	unsigned char tmp_dest = op1;

	while(tmp_count > 0) {
		op1 = op1 >> 1;
		unsigned char lsb = tmp_dest & 0x01;
		flags->cf = lsb;
		tmp_count--;
	}

	if(op2 == 1) {
		unsigned char msb = tmp_dest & 0x80;
		flags->of = msb;
	}

	set_flags8(op1);
	return op1;
}

inline unsigned short shr16(unsigned short op1,unsigned char op2) {
	unsigned char tmp_count = op2 & 0x1f;
	unsigned short tmp_dest = op1;

	while(tmp_count > 0) {
		op1 = op1 >> 1;
		unsigned char lsb = op1 & 0x01;
		flags->cf = lsb;
		tmp_count--;
	}

	if(tmp_count == 1) {
		unsigned short msb = tmp_dest & 0x8000;
		flags->of = msb;
	}

	set_flags16(op1);
	return op1;
}

/*****************************
	      SHL
*****************************/

inline unsigned char shl8(unsigned char op1,unsigned char op2) {
	unsigned char tmp_count = op2 & 0x1f;
	unsigned char tmp_dest = op1;

	while(tmp_count > 0) {
		op1 = op1 << 1;
		unsigned char msb = tmp_dest & 0x8000;
		flags->cf = msb;
		tmp_count--;
	}

	if(tmp_count == 1)
		flags->of = tmp_dest^flags->cf;

	set_flags8(op1);
	return op1;
}

inline unsigned short shl16(unsigned short op1,unsigned char op2) {
	unsigned char tmp_count = op2 & 0x1f;
	unsigned short tmp_dest = op1;

	while(tmp_count > 0) {
		op1 = op1 << 1;
		unsigned short msb = op1 & 0x8000;
		flags->cf = msb;
		tmp_count--;
	}

	if(tmp_count == 1)
		flags->of = tmp_dest^flags->cf;

	set_flags16(op1);
	return op1;
}



/**These are the actual instructions**/
void and_modrm8_r8(){
    unsigned char *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_write8(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=ptr+(unsigned long)core;
        //*ptr is the destination, *gregs8[rm[0].extra] is the source/operand
        *ptr=and8(*ptr,*gregs8[rm[0].extra]);
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        //*gregs8[rm[0].rm] is the destination, *gregs8[rm[0].extra] is the source/operand
        *gregs8[rm[0].rm]=and8(*gregs8[rm[0].rm],*gregs8[rm[0].extra]);
        return;
    }
    panic("errors not yet handled!!");
}

void and_modrm16_r16(){
    unsigned short *ptr;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_write16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
        //*ptr is the destination, gregs16[rm[0].extra] is the source/operand
        *ptr=and16(*ptr,gregs16[rm[0].extra]);
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        //gregs16[rm[0].rm] is the destination, gregs16[rm[0].extra] is the source/operand
        gregs16[rm[0].rm]=and16(gregs16[rm[0].rm],gregs16[rm[0].extra]);
        return;
    }
    panic("errors not yet handled!!");
}




/*******merged from shift.c*********/
/*
Supprts shl/shr in open86
By: Alboin, 2-24-07

*/




void group_c0_through_d3() { //This function is still buggy(very)
	mod_rm rm[1];
	unsigned char *ptr;
	unsigned char *ptr2;
	unsigned char *to_be_used;
	unsigned int tmp;
	unsigned char tmp_cf;
	unsigned char op;
	unsigned int bit_type;

	/* Get the opcode and modrm. */
	MemRead8(CS,ip,&op);
	ip++;
	MemRead8(CS,ip,rm);

	/* Determine what bit type we are. */
	if(op == 0xD0 || op == 0xD2 || op == 0xC0)
		bit_type = 8;
	else //(op == 0xD1 || op == 0xD3 || op == 0xC1)
		bit_type = 16;

	if(bit_type == 8 )
		tmp=GetModRM_write8(&ptr);
	else
		tmp=GetModRM_write16(&ptr);

	/* Find out where we should point to. */
	if(tmp==0) {
		ptr=(unsigned long)ptr+(unsigned long)core;
		to_be_used=ptr;
	}
	if(tmp==OPCODE_SPECIFIC){
		if(bit_type == 8)
			to_be_used = gregs8[rm[0].rm];
		if(bit_type == 16)
			to_be_used = &gregs16[rm[0].rm];
	}

	/* Get the other operand. */
	unsigned char iw;
	ip++;
	MemRead8(CS,ip,&iw);

	/* do opcode specific stuff... */
	switch(rm[0].extra){
		/* rol */
		case 0:
			break;
		/* ror */
		case 1:
			break;
		/* rcl */
		case 2:
			break;
		/* rcr */
		case 3:
			break;
		/* shl */
		case 4:
			if(op == 0xD0) {
				ip--;
				if(bit_type == 8)
					*to_be_used = shl8(*to_be_used, 1);
				else if(bit_type == 16)
					*to_be_used = shl16((unsigned short)*to_be_used, 1);
			}
			else {
				if(bit_type == 8)
					*to_be_used = shl8(*to_be_used, iw);
				else if(bit_type == 16)
					*to_be_used = shl16((unsigned short)*to_be_used, iw);
			}
			break;
		/* shr */
		case 5:
			if(op == 0xD0) {
				ip--;
				if(bit_type == 8)
					*to_be_used = shr8(*to_be_used, 1);
				else if(bit_type == 16)
					*to_be_used = shr16((unsigned short)*to_be_used, 1);
			}
			else {
				if(bit_type == 8)
					*to_be_used = shr8(*to_be_used, iw);
				else if(bit_type == 16)
					*to_be_used = shr16((unsigned short)*to_be_used, iw);
			}
			break;
		/* sal */
		case 6:
			break;
		/* sar */
		case 7:
			break;
		/* Error */
		default:
			panic("Unsupported opcode! at rm\n");
			break;
	}
}












