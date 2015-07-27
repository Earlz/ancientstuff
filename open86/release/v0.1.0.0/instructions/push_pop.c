#include "../_base.h"

//This file has all push and pop operations


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



void push_r16(){
    unsigned char reg;
    reg=*iptr&(0x0F); //isolate the bottom nibble
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
     reg=*iptr&0xF;
     reg=reg-8;
     (unsigned short)gregs16[reg]=pop();
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


//guess I'll consider call and ret under push and pops
SPEED_UP void near_call(unsigned short ptr){
     if(push(ip)!=0){
         panic("no faulting!!");
     }
     no_advance=1;
     ip=ptr;
}
SPEED_UP void far_call(unsigned short ptr,unsigned short seg){
     if(push(sregs[CS])!=0 || push(ip)!=0){
         panic("no faulting!!");
     }
     no_advance=1;
     ip=ptr;
     sregs[CS]=seg;
}

SPEED_UP void far_ret(){
     ip=pop();
     sregs[CS]=pop();
     no_advance=1;
     doeip;doiptr;
}

SPEED_UP void near_ret(){
     ip=pop();
     no_advance=1;
     doeip;doiptr;
}



void call_rel16(){
     signed short tmp;
     ip++;
     MemRead16(CS,ip,&tmp);
     ip++;ip++;
     near_call((signed short)ip+tmp);
}

void retn0(){
     near_ret();
}
void retf0(){
     far_ret();
}
void call_far_ptr16(){//this is actually a call seg:offset
     unsigned short off;
     unsigned short seg;
     ip++;
     MemRead16(CS,ip,&seg);
     ip++;ip++;
     MemRead16(CS,ip,&off);
     //*seg--;
     far_call(seg,off); //oops set them backwards... ohwell
    // panic("debug #1\n");
}












