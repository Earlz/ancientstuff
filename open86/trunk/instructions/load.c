/*
LES, LDS, etc for Open86.
By: Alboin, 3-10-07
*/

#include "../_base.h" 

/*****************************
	      load_m16
*****************************/

SPEED_UP void load_m16(unsigned char what) {
	mod_rm rm[1];
	unsigned short addr, a1, a2;

	ip++;
	MemRead8(CS,ip,rm);
	ip++;
	MemRead16(CS,ip,&addr);	
	ip++;
	ip++;
	
	MemRead16(CS,addr,&a1);
	MemRead16(CS,addr+2,&a2);
	
	gregs16[rm[0].extra] = a1;
	sregs[what] = a2;
} 

/*****************************
	      LES
*****************************/

/* Opcode: 0xC4 */
void les_m16() {
	load_m16(ES);
}

/*****************************
	      LDS
*****************************/

/* Opcode: 0xC5 */
void lds_m16() {
	load_m16(DS);
}

