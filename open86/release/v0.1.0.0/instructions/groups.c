#include "../_base.h"



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
        (unsigned long)ptr=(unsigned long)ptr+(unsigned long)core;
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











