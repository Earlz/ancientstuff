#include "../_base.h"


void nop(){ //used as a debug function for now..
     printf("AX: %x\nCX: %x\nDX: %x\nBX: %x\nSP: %x\nBP: %x\nSI: %x\nDI: %x\n",gregs16[0],gregs16[1],gregs16[2],gregs16[3],gregs16[4],gregs16[5],gregs16[6],gregs16[7]);
     printf("ES: %x\nCS: %x\nSS: %x\nDS: %x\n",sregs[0],sregs[1],sregs[2],sregs[3]);
     printf("flags: %x\nIP: %x\nEIP(private): %x\n",flagreg,ip,eip);
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

void unknown(){ //unknown opcode
     panic("Unknown Opcode!!\n");
}

void hlt(){ //hlt
     panic("HALTED! interrupts not supported yet!\n");
}



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




void lods_m8(){
    unsigned char *bah2;
    bah2=(core+(ip+(sregs[CurrentSegment]<<4)));
    *gregs8[AL]=*bah2;
    if(flags->df==0){
        gregs16[SI]++;
    }else{
        gregs16[SI]--;
    }
}







