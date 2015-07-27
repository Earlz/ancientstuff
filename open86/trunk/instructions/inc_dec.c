#include "../_base.h"

/**This file has support for inc and dec**/

/*
Adds support for inc\dec instructions for Open86
By: Alboin, 2-23-07
*/




/* Opcode: 0x40, 0x41, ..., 0x47 */
void inc_r16() {
	unsigned char reg;
	unsigned char tmp_cf;

	MemRead8(CS,ip,&reg);
	reg=reg&0x0f;

	tmp_cf=flags->cf;
	gregs16[reg]=add16(gregs16[reg],1);
	flags->cf=tmp_cf;
}

/* Opcode: 0x48, 0x49, ..., 0x4f */
void dec_r16() {
	unsigned char reg;
	unsigned char tmp_cf;

	MemRead8(CS,ip,&reg);
	reg=reg&0x0f;
	reg-=8;

	tmp_cf=flags->cf;
	gregs16[reg]=sub16(gregs16[reg],1);
	flags->cf=tmp_cf;
}

/* Opcode: FE r/8 *
void inc_dec_r8() {
	unsigned char reg;
	unsigned char flag;
	unsigned char tmp_cf;

	/* Get what we are: inc\dec *
	ip++;
	MemRead8(CS,ip,&reg);
	flag=reg&0x08;

	/* Find the register. *
	reg=reg&0x07;

	/* inc *
	if(flag == 0) {
		tmp_cf=flags->cf;
		*gregs8[reg]=add8(*gregs8[reg],1);
		flags->cf=tmp_cf;
	}
	/* dec *
	else if(flag == 8) {
		tmp_cf=flags->cf;
		*gregs8[reg]=sub8(*gregs8[reg],1);
		flags->cf=tmp_cf;
	}
}
*/
