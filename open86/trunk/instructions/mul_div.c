#include "../_base.h"
/**This file contains all MUL, IMUL, IDIV, and DIV instructions**/


SPEED_UP unsigned short mul8(unsigned char op1,unsigned char op2){
    unsigned short result;
    result=op1*op2;
    if((result&0xFF00)>0){ //if tophalf of result has anything in it
        flags->cf=1;
        flags->of=1;
    }else{
        flags->cf=0;
        flags->of=0;
    }
    return result;
}


SPEED_UP unsigned int mul16(unsigned short op1,unsigned short op2){
    unsigned int result;
    result=op1*op2;
    if((result&0xFFFF0000)>0){
        flags->cf=1;
        flags->of=1;
    }else{
        flags->cf=0;
        flags->of=0;
    }
    return result;
}



/**Everything currently supported for mul is in groups.c**/




/* --forgot this was in a group!

void mul8_modrm8(){ //greg8 is stored in rm->extra!
    unsigned char *ptr;
    //unsigned short to_use;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read8(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
       // printf("ptr: 0x%x rm: 0x%x",*ptr, gregs16[rm[0].extra]);

        gregs16[AX]=mul8(*gregs8[AL],*ptr);
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        gregs16[AX]=mul8(*gregs8[AL],*gregs8[rm[0].rm]);
        //*gregs8[rm[0].extra]=*gregs8[rm[0].rm];
        return;
    }
    panic("errors not yet handled!!");
}

void mul16_modrm16(){ //greg8 is stored in rm->extra!
    unsigned short *ptr;
    unsigned int to_use;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=(unsigned long)ptr+(unsigned long)core;
       // printf("ptr: 0x%x rm: 0x%x",*ptr, gregs16[rm[0].extra]);

        to_use=mul16(gregs16[AX],*ptr);
        gregs16[DX]=to_use&0xFFFF0000;
        gregs16[AX]=to_use&0xFFFF; //distribute through to DX:AX
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        to_use=mul16(*gregs8[AL],*gregs8[rm[0].rm]);
        gregs16[DX]=to_use&0xFFFF0000;
        gregs16[AX]=to_use&0xFFFF;
        //*gregs8[rm[0].extra]=*gregs8[rm[0].rm];
        return;
    }
    panic("errors not yet handled!!");
}

*/
//end comment block!


