#include "../_base.h"


void mov_8greg_ib(){ //completely working
     unsigned char tmp;
     unsigned char tmp2;
     //tmp=*iptr;
     MemRead8(CS,ip,&tmp);
     tmp=tmp&0x0F; //isolate register bits
     ip++;
     MemRead8(CS,ip,&tmp2);
     *gregs8[tmp]=*iptr; //mov the byte
}

void mov_16greg_iw(){
     unsigned char tmp;unsigned short tmp2;
     MemRead8(CS,ip,&tmp);
     tmp=tmp&0x0F;
     tmp=tmp-8; //because we don't use greater than 7 and it adds 7 automatically
     ip++;
     MemRead16(CS,ip,&tmp2);
     gregs16[tmp]=tmp2;
     ip++; //we have an extra byte unaccounted for since this is a word
}

//keep on forgetting you have to use gregs8 with a *...
void mov_rm8_greg8(){ //greg8 is stored in rm->extra!
    unsigned char *ptr;
    unsigned char *ptr2;
    unsigned short pip,pip2;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_write8(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        ptr=ptr+(unsigned long)core;
        *ptr=*gregs8[rm[0].extra];
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        *gregs8[rm[0].rm]=*gregs8[rm[0].extra];
        return;
    }
    panic("errors not yet handled!!");
}


   // ptr=core;
   // ptr=ptr+0x300;
   // printf("tmp: %x\n",*ptr);





void mov_rm16_greg16(){ //greg8 is stored in rm->extra!
    unsigned short *ptr;
    unsigned char *ptr2;
    unsigned short pip,pip2;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_write16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        (unsigned long)ptr=(unsigned long)ptr+(unsigned long)core; //ugly typecasting...
       // printf("ptr: 0x%x rm: 0x%x",*ptr, gregs16[rm[0].extra]);
        *ptr=gregs16[rm[0].extra];
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        gregs16[rm[0].rm]=gregs16[rm[0].extra];
        return;
    }
    panic("errors not yet handled!!");
}



void mov_greg8_rm8(){ //greg8 is stored in rm->extra!
    unsigned char *ptr;
    unsigned char *ptr2;
    unsigned short pip,pip2;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read8(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        (unsigned long)ptr=(unsigned long)ptr+(unsigned long)core;
       // printf("ptr: 0x%x rm: 0x%x",*ptr, gregs16[rm[0].extra]);
        *gregs8[rm[0].extra]=*ptr;
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        *gregs8[rm[0].extra]=*gregs8[rm[0].rm];
        return;
    }
    panic("errors not yet handled!!");
}



void mov_greg16_rm16(){ //greg8 is stored in rm->extra!
    unsigned short *ptr;
    unsigned char *ptr2;
    unsigned short pip,pip2;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        (unsigned long)ptr=(unsigned long)ptr+(unsigned long)core;
       // printf("ptr: 0x%x rm: 0x%x",*ptr, gregs16[rm[0].extra]);
        gregs16[rm[0].extra]=*ptr;
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        gregs16[rm[0].extra]=gregs16[rm[0].rm];
        return;
    }
    panic("errors not yet handled!!");
}




void mov_rm16_sreg(){ //greg8 is stored in rm->extra!
    unsigned short *ptr;
    unsigned char *ptr2;
    unsigned short pip,pip2;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_write16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        (unsigned long)ptr=(unsigned long)ptr+(unsigned long)core;
       // printf("ptr: 0x%x rm: 0x%x",*ptr, gregs16[rm[0].extra]);
        *ptr=sregs[rm[0].extra];
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        gregs16[rm[0].rm]=sregs[rm[0].extra];
        return;
    }
    panic("errors not yet handled!!");
}
    //*ptr=sregs[rm->extra];



void mov_sreg_rm16(){ //greg8 is stored in rm->extra!
    unsigned short *ptr;
    unsigned char *ptr2;
    unsigned short pip,pip2;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_read16(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        (unsigned long)ptr=(unsigned long)ptr+(unsigned long)core;
       // printf("ptr: 0x%x rm: 0x%x",*ptr, gregs16[rm[0].extra]);
        sregs[rm[0].extra]=*ptr;
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        sregs[rm[0].extra]=gregs16[rm[0].rm];
        return;
    }
    panic("errors not yet handled!!");
}


void mov_m8_imm8(){
    unsigned char *ptr;
    unsigned char *ptr2;
   // unsigned short pip,pip2;
    unsigned int tmp;
    mod_rm rm[1];
    ip++;
    MemRead8(CS,ip,rm); //store rm because we need 'extra'
    tmp=GetModRM_write8(&ptr);
    if(tmp==0){ //is normal and ptr contains the memory address
        (unsigned long)ptr=(unsigned long)ptr+(unsigned long)core;
       // printf("ptr: 0x%x rm: 0x%x",*ptr, gregs16[rm[0].extra]);
        MemRead8(CS,ip,ptr); //don't let this decieve you!
                             //this will read the immediant byte into the ptr we got from modrm
        return;
    }
    if(tmp==OPCODE_SPECIFIC){
        //gregs16[rm[0].rm]=sregs[rm[0].extra];
        panic("no faulting!! --undefined opcode\n");
        return;
    }
    panic("errors not yet handled!!");
}




















