#include "../_base.h"
/**These are "group" opcodes--bytes that have an extra modrm byte to tell which
opcode to use, hence implementing 8 opcodes per byte
**/


unsigned int mul16(unsigned short op1,unsigned short op2);
unsigned short mul8(unsigned char op1,unsigned char op2);

void group2_ff(){
    mod_rm rm[1];
    unsigned short *ptr;
    unsigned char *ptr2;
    unsigned short pip,pip2;
    unsigned short to_be_used; //this is the contents of the interpreted modrm
    unsigned int tmp;
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
        to_be_used=*ptr;
    }
    if(tmp==OPCODE_SPECIFIC){
        to_be_used=gregs16[rm[0].rm];
    }

    //do opcode specific stuff...
    switch(rm[0].extra){ //check opcode
        case 6: //push m16
        push(to_be_used);
        break;
        default:
        panic("Unsupported opcode! at rm\n");
        break;
    }

}


void group_f6(){
    mod_rm rm[1];
    unsigned char *ptr;
    unsigned char *ptr2;
    unsigned short pip,pip2;
    unsigned char to_be_used; //this is the contents of the interpreted modrm
    unsigned int tmp;
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read8(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
        to_be_used=*ptr;
    }
    if(tmp==OPCODE_SPECIFIC){
        to_be_used=*gregs8[rm[0].rm]; //have to make sure to add that *!
    }

    //do opcode specific stuff...
    switch(rm[0].extra){ //check opcode
        case 4: //mul modrm8
        gregs16[AX]=mul8(*gregs8[AL],to_be_used);
        break;
        case 6: //div modrm8
        if(to_be_used==0){panic("divide by zero error! errors not yet handled!\n");}
        tmp=gregs16[AX]/to_be_used;
        //printf("ax: %i to_be_used: %i",gregs16[AX],to_be_used);
        if(tmp>0xFF){panic("divide by zero error(overflow)! errors not yet handled!\n");}
        *gregs8[AL]=tmp;
        *gregs8[AH]=gregs16[AX]%to_be_used;
        break;
        default:
        panic("Unsupported opcode! at rm\n");
        break;
    }

}


void group_f7(){
    mod_rm rm[1];
    unsigned short *ptr;
    unsigned int to_use;
   // unsigned short pip,pip2;
    unsigned short to_be_used; //this is the contents of the interpreted modrm
    unsigned int tmp;
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
        to_be_used=*ptr;
    }
    if(tmp==OPCODE_SPECIFIC){
        to_be_used=gregs16[rm[0].rm];
    }

    //do opcode specific stuff...
    switch(rm[0].extra){ //check opcode
        case 4: //mul modrm16
        to_use=mul16(gregs16[AX],to_be_used);
        gregs16[DX]=(to_use&0xFFFF0000)>>16;
        gregs16[AX]=to_use&0xFFFF; //distribute through to DX:AX
        break;
        case 6: //div modrm16
        if(to_be_used==0){panic("divide by zero error! errors not yet handled!\n");}
        tmp=((gregs16[DX]<<16)|gregs16[AX])/to_be_used;
        if(tmp>0xFFFF){panic("divide by zero error(overflow)! errors not yet handled!\n");}
        gregs16[AX]=tmp;
        gregs16[DX]=((gregs16[DX]<<16)|gregs16[AX])%to_be_used;
        break;
        default:
        panic("Unsupported opcode! at rm\n");
        break;
    }

}



void group_fe(){
    mod_rm rm[1];
    unsigned char *ptr;
    unsigned char *ptr2;
    unsigned char *to_be_used; //this is the contents of the interpreted modrm
    unsigned int tmp;
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'(sometimes)
    tmp=GetModRM_write8(&ptr); //note it isn't always write8 for each group!
    //pointer now contains the memory address that should be used(unless tmp==opcode_specific)
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
        to_be_used=ptr;
    }
    if(tmp==OPCODE_SPECIFIC){
        to_be_used=gregs8[rm[0].rm];
    }
	//actually you can ignore above this line!
    //do opcode specific stuff...
    switch(rm[0].extra){ //check opcode
    	case 0:
		//now you do your thing, except for instead of reg, you use *to_be_used;
		tmp=flags->cf;
		*to_be_used=add8(*to_be_used,1);
		flags->cf=tmp;
		//this one is inc!
		break;
		case 1:
		//now you do your thing with dec, except for instead of reg, you use *to_be_used;
		//you fill out this one!

		//this one is dec!
		break;
        default:
        panic("Unsupported opcode! at rm\n");
        break;
    }

}



void group_80(){
    mod_rm rm[1];
    unsigned short *ptr;
    unsigned char *ptr2;
    unsigned short pip,pip2;
    unsigned char *to_be_used; //this is the contents of the interpreted modrm
    unsigned int tmp;
    unsigned char tmp2;
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read8(&ptr);
    ip++;
    MemRead8(CS,ip,&tmp2);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
        to_be_used=ptr;
    }
    if(tmp==OPCODE_SPECIFIC){
        to_be_used=gregs8[rm[0].rm];
    }

    //do opcode specific stuff...
    switch(rm[0].extra){ //check opcode
        case 0: //add rm8,imm8
        *to_be_used=add8(*to_be_used,tmp2);
        break;
        case 5: //sub rm8,imm8
        *to_be_used=sub8(*to_be_used,tmp2);
        break;
        case 7: //cmp rm,imm8
        sub8(*to_be_used,tmp2);
        break;
        default:
        panic("Unsupported opcode! at rm\n");
        break;
    }

}

void group_81(){
    mod_rm rm[1];
    unsigned short *ptr;
    unsigned short *ptr2;
    unsigned short pip,pip2;
    unsigned short *to_be_used; //this is the contents of the interpreted modrm
    unsigned int tmp;
    unsigned short tmp2;
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    ip++;
    MemRead16(CS,ip,&tmp2);
    ip++;
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
        to_be_used=ptr;
    }
    if(tmp==OPCODE_SPECIFIC){
        to_be_used=&gregs16[rm[0].rm];
    }

    //do opcode specific stuff...
    switch(rm[0].extra){ //check opcode
        case 0: //add rm16,imm16
        *to_be_used=add16(*to_be_used,tmp2);
        break;
        case 5: //sub rm16,imm16
        *to_be_used=sub16(*to_be_used,tmp2);
        break;
        case 7: //cmp rm16,imm16
        sub16(*to_be_used,tmp2);
        break;
        default:
        panic("Unsupported opcode! at rm\n");
        break;
    }

}

void group_83(){
    mod_rm rm[1];
    unsigned short *ptr;
    unsigned short *ptr2;
    unsigned short pip,pip2;
    unsigned short *to_be_used; //this is the contents of the interpreted modrm
    unsigned int tmp;
    unsigned char tmp2;
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    ip++;
    MemRead8(CS,ip,&tmp2);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
        to_be_used=ptr;
    }
    if(tmp==OPCODE_SPECIFIC){
        to_be_used=&gregs16[rm[0].rm];
    }

    //do opcode specific stuff...
    switch(rm[0].extra){ //check opcode
        case 0: //add rm16,imm8
        *to_be_used=add16(*to_be_used,tmp2);
        break;
        case 5: //sub rm16, imm8
        *to_be_used=sub16(*to_be_used,tmp2);
        break;
        case 7: //cmp rm16,imm8
        sub16(*to_be_used,tmp2);
        break;
        default:
        panic("Unsupported opcode! at rm\n");
        break;
    }

}

/* Opcode: 0xD4 */
void group_d4() {
	unsigned char part;
	MemRead8(CS,ip,&part);
	
	switch(part) {
		/* AAM */
		case 0x0A:
			*gregs8[AH] = *gregs8[AL] / 10;
			*gregs8[AL] = *gregs8[AL] % 10;
			break;
	}
}

/* Opcode: 0xD5 */
void group_d5() {
	unsigned char part;
	MemRead8(CS,ip,&part);
	
	switch(part) {
		/* AAD */
		case 0x0A:
			*gregs8[AL] = *gregs8[AH] * 10 + *gregs8[AL];
			*gregs8[AH] = 0;
			break;			
	}
}

