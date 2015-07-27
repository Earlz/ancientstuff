#include "../_base.h"
/**This file contains all jumping and conditioning instructions**/




void jmp_rel16(){
     signed short tmp;
     ip++;
     MemRead16(CS,ip,&tmp);
     ip=(unsigned)ip+(signed)tmp;
     ip++;
     //no wories on advancing ip because we set ip in this
}



void jmp_rel8(){
     signed char ptr;
     ip++;
     MemRead8(CS,ip,&ptr);
     ip=(unsigned)ip+(signed)ptr;
     //no wories on advancing ip because we set ip in this
}

void jmp_ptr16seg16(){ //yay working
     unsigned short off,seg;
     ip++;
     MemRead16(CS,ip,&off);
     ip++;
     ip++;
     MemRead16(CS,ip,&seg);
     ip=(unsigned)ip+(signed)off;
     sregs[CS]=(unsigned)sregs[CS]+(signed)seg;
}



/**JCC instructions**/
void ja_rel8(){ //is also jnbe
    signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    if(flags->cf==0 && flags->zf==0){
        ip=(unsigned)ip+(signed)blah;
    }
}

void jnc_rel8(){ //is also jae and jnb
    signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    if(flags->cf==0){
        ip=(unsigned)ip+(signed)blah;
    }
}


void jbe_rel8(){ //is also jna
     signed char blah;
     ip++;
    MemRead8(CS,ip,&blah);
    if(flags->cf==1 || flags->zf==1){
        ip=(unsigned)ip+(signed)blah;
    }
}

void jc_rel8(){ //is also jb and jnae
     signed char blah;
     ip++;
    MemRead8(CS,ip,&blah);
    if(flags->cf==1){
        ip=(unsigned)ip+(signed)blah;
    }
}

void jz_rel8(){ //is also je
     signed char blah;
     ip++;
    MemRead8(CS,ip,&blah);
    if(flags->zf==1){
        ip=(unsigned)ip+(signed)blah;
    }
}

void jnz_rel8(){ //is also jne
     signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    if(flags->zf==0){
        ip=(unsigned)ip+(signed)blah;
    }
}


void jp_rel8(){ //also jpe
     signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    if(flags->pf==1){
        ip=(unsigned)ip+(signed)blah;
    }
}

void jnp_rel8(){ //is also jpo
     signed char blah;
	ip++;
    MemRead8(CS,ip,&blah);
    if(flags->pf==0){
        ip=(unsigned)ip+(signed)blah;
    }
}

void jg_rel8(){ //is also jnle
     signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    if(flags->sf==flags->of && flags->zf==0){
        ip=(unsigned)ip+(signed)blah;
    }
}


void jge_rel8(){ //is also jnl
     signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    if(flags->sf==flags->of){
        ip=(unsigned)ip+(signed)blah;
    }
}

void jle_rel8(){ //is also jng
     signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    if(flags->sf!=flags->of || flags->zf==1){
        ip=(unsigned)ip+(signed)blah;
    }
}

void jl_rel8(){ //is also jnge
     signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    if(flags->sf!=flags->of){
        ip=(unsigned)ip+(signed)blah;
    }
}


void jo_rel8(){
     signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    if(flags->of==1){
        ip=(unsigned)ip+(signed)blah;
    }
}

void jno_rel8(){
     signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    if(flags->of==0){
        ip=(unsigned)ip+(signed)blah;
    }
}

void js_rel8(){ //is negative
     signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    if(flags->sf==1){
        ip=(unsigned)ip+(signed)blah;
    }
}

void jns_rel8(){ //is  positive
     signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    if(flags->sf==0){
        ip=(unsigned)ip+(signed)blah;
    }
}



/**Loop instructions**/
void loop_rel8(){
    signed char blah;
    ip++;
    MemRead8(CS,ip,&blah);
    gregs16[CX]--;
    if(gregs16[CX]!=0){ip=(unsigned)ip+(signed)blah;}
}




/**Call instructions**/
SPEED_UP void near_call(unsigned short ptr){
     if(push(ip)!=0){
         panic("no faulting!!");
     }

     ip=ptr-1;
}
SPEED_UP void far_call(unsigned short ptr,unsigned short seg){
     if(push(sregs[CS])!=0 || push(ip)!=0){
         panic("no faulting!!");
     }

     ip=ptr-1;
     sregs[CS]=seg;
}

SPEED_UP void far_ret(){
     ip=pop();
     sregs[CS]=pop();
     no_advance=1;
}

SPEED_UP void near_ret(){
     ip=pop();
     no_advance=1;
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

