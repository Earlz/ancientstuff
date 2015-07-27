#include "../_base.h"

/**This file contains all the push and pop functions**/

/**These functions are NOT opcodes!**/
SPEED_UP unsigned int push(unsigned short val){
    unsigned short *ptr;
    gregs16[SP]--;gregs16[SP]--; //maybe we can get an optimization out of it..
    //ptr=core+gregs16[SP]+sregs[SS];
    return MemWrite16(SS,gregs16[SP],&val);
    //*ptr=val;
}

SPEED_UP unsigned short pop(){
    unsigned short tmp;
    MemRead16(SS,gregs16[SP],&tmp);
    gregs16[SP]++;gregs16[SP]++;
    return tmp;
}

/**These are the opcodes**/

void push_r16(){
    unsigned char reg,tmp;
    MemRead8(CS,ip,&tmp);
    reg=tmp&(0x0F); //isolate the bottom nibble
    if(reg==SP){ //the 80286 and above do this...
         if(push(gregs16[SP]+2)!=0){
             panic("no faulting!!");
         }
         return;
    }
    if(push(gregs16[reg])!=0){
        panic("no faulting!!");
    }
} //wow done already!!

void pop_r16(){
     unsigned char reg;
     unsigned char tmp;
     MemRead8(CS,ip,&tmp);
     reg=tmp&0x0F;
     reg=reg-8;
     gregs16[reg]=pop();
}

void push_imm16(){
     unsigned short imm;
     ip++;
     MemRead16(CS,ip,&imm);
     if(push(imm)!=0){
         panic("no faulting!!");
     }
     ip++;
     //doeip;doiptr;  --don't need this with our new system
}

//no pop for imm's

void push_imm8(){
    unsigned char tmp;
     ip++;
     MemRead8(CS,ip,&tmp);
     if(push((unsigned char)tmp)!=0){
         panic("no faulting!!");
     }
}



void push_es(){
    if(push(sregs[ES])!=0){
        panic("no faulting!!");
    }
}
void push_cs(){
    if(push(sregs[CS])!=0){
        panic("no faulting!!");
    }
}
void push_ds(){
    if(push(sregs[DS])!=0){
        panic("no faulting!!");
    }
}
void push_ss(){
   if(push(sregs[SS])!=0){
       panic("no faulting!!");
   }
}


void pop_es(){
    sregs[ES]=pop();
}
void pop_ds(){
    sregs[DS]=pop();
}
void pop_ss(){
    sregs[SS]=pop();
}
















