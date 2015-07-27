/*
BCD Opcodes for Open86
By: Alboin, 3-16-07
*/

#include "../_base.h"  

/*****************************
	      AAA
*****************************/

/* Opcode: 0x37 */
void aaa() {
	if((*gregs8[AL] & 0x0f) > 9 || flags->af == 1) {
		*gregs8[AL] += 6;
		*gregs8[AH]++;
		flags->af = 1;
		flags->cf = 1;	
	}
	else {
		flags->af = 0;
		flags->cf = 0;	
	}
	*gregs8[AL] = *gregs8[AL] & 0x0f;
}

/*****************************
	      DAA
*****************************/

/* Opcode: 0x27 */
void daa() {
	if((*gregs8[AL] & 0x0f) > 9 || flags->af == 1) {
		*gregs8[AL] += 6;
		*gregs8[AH]++;
	}
	else
		flags->af = 0;
	
	if(*gregs8[AL] > 0x9f || flags->cf == 1) {
			*gregs8[AL] = *gregs8[AL] + 0x60;
			flags->cf = 1;
	}
	else
		flags->cf = 0;
}

/*****************************
	      DAS
*****************************/

/* Opcode: 0x2F */
void das() {
	if((*gregs8[AL] & 0x0f) > 9 || flags->af == 1) {
		*gregs8[AL] -= 6;
		flags->af = 1;
	}
	else
		flags->af = 0;
	
	if(*gregs8[AL] > 0x9f || flags->cf == 1) {
			*gregs8[AL] -= 0x60;
			flags->cf = 1;
	}
	else
		flags->cf = 0;
}

/*****************************
	      AAS
*****************************/

/* Opcode: 0x3F */
void aas() {
	if((*gregs8[AL] & 0x0f) > 9 || flags->af == 1) {
		*gregs8[AL] -= 6;
		*gregs8[AH]--;
		flags->af = 1;
		flags->cf = 1;	
	}
	else {
		flags->af = 0;
		flags->cf = 0;	
	}
	*gregs8[AL] = *gregs8[AL] & 0x0f;
}
