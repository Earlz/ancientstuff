#include "../_base.h"
//this file has all jmp instructions




void jmp_rel16(){
     signed short *ptr;
     ip++;
     doeip;doiptr;
     ptr=iptr;
     ip=ip+*ptr; //because ptr is signed it will work right
     ip++;
     doeip;doiptr;
     //no wories on advancing ip because we set ip in this
}



void jmp_rel8(){
     signed char *ptr;
     ip++;
     doeip;doiptr;
     ptr=iptr;
     ip=ip+*ptr; //because ptr is signed it will work right
     doeip;doiptr;
     //no wories on advancing ip because we set ip in this
}

void jmp_ptr16seg16(){ //yay working
     unsigned short *ptr;
     ip++;
     doeip;doiptr;
     ptr=iptr;
     ip=*ptr; //set the offset
     //IP IS NOW INVALID TO US
     iptr++;iptr++;
     ptr=iptr;
     sregs[CS]=*ptr;
     doeip;doiptr;
}



//rel8/short jcc stuff
void ja_rel8(){ //is also jnbe
    signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->cf==0 && flags->zf==0){
        ip=(unsigned)ip+(signed)*blah;
    }
}

void jnc_rel8(){ //is also jae and jnb
    signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->cf==0){
        ip=(unsigned)ip+(signed)*blah;
    }
}


void jbe_rel8(){ //is also jna
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->cf==1 || flags->zf==1){
        ip=(unsigned)ip+(signed)*blah;
    }
}

void jc_rel8(){ //is also jb and jnae
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->cf==1){
        ip=(unsigned)ip+(signed)*blah;
    }
}

void jz_rel8(){ //is also je
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->zf==1){
        ip=(unsigned)ip+(signed)*blah;
    }
}

void jnz_rel8(){ //is also jne
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->zf==0){
        ip=(unsigned)ip+(signed)*blah;
    }
}


void jp_rel8(){ //also jpe
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->pf==1){
        ip=(unsigned)ip+(signed)*blah;
    }
}

void jnp_rel8(){ //is also jpo
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->pf==0){
        ip=(unsigned)ip+(signed)*blah;
    }
}

void jg_rel8(){ //is also jnle
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->sf==flags->of && flags->zf==0){
        ip=(unsigned)ip+(signed)*blah;
    }
}


void jge_rel8(){ //is also jnl
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->sf==flags->of){
        ip=(unsigned)ip+(signed)*blah;
    }
}

void jle_rel8(){ //is also jng
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->sf!=flags->of || flags->zf==1){
        ip=(unsigned)ip+(signed)*blah;
    }
}

void jl_rel8(){ //is also jnge
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->sf!=flags->of){
        ip=(unsigned)ip+(signed)*blah;
    }
}


void jo_rel8(){
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->of==1){
        ip=(unsigned)ip+(signed)*blah;
    }
}

void jno_rel8(){
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->of==0){
        ip=(unsigned)ip+(signed)*blah;
    }
}

void js_rel8(){ //is negative
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->sf==1){
        ip=(unsigned)ip+(signed)*blah;
    }
}

void jns_rel8(){ //is  positive
     signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    if(flags->sf==0){
        ip=(unsigned)ip+(signed)*blah;
    }
}




void loop_rel8(){
    signed char *blah;
    ip++;
    doeip;doiptr;
    blah=iptr;
    gregs16[CX]--;
    if(gregs16[CX]!=0){ip=(unsigned)ip+(signed)*blah;}
}







