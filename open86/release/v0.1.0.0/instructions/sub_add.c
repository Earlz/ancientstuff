#include "../_base.h"


// TODO (Jordan#4#): Get AF stuff working in this!!
unsigned char sub8(unsigned char base,unsigned char subt){
    signed char result;
    if(subt>base){flags->cf=1;}else{flags->cf=0;}
    if((signed int)base-subt>127 || (signed int)base-subt<-127){
            flags->of=1;
        }else{
            flags->of=0;
        }
    result=base-subt;
    if(result==0){flags->zf=1;}else{flags->zf=0;}
    CalculatePF8(result); //do pf
    CalculateSF8(result); //do sf
    base=base&0xF;
    subt=subt&0xF; //isolate bottom nibble
    if((signed int)base-subt>15 || (signed int)base-subt<-15){
        flags->af=1;
    }else{
        flags->af=0;
    }
    return result;
}

unsigned short sub16(unsigned short base,unsigned short subt){
    signed short result;
    unsigned short mirror;
    if(subt>base){flags->cf=1;}else{flags->cf=0;}
    if((signed int)base-subt>32767 || (signed int)base-subt<-32767){
            flags->of=1;
        }else{
            flags->of=0;
        }
    mirror=base-subt;
    result=mirror;
    if(result==0){flags->zf=1;}else{flags->zf=0;}
    CalculatePF16(result); //do pf
    CalculateSF16(result); //do sf
    base=base&0xF;
    subt=subt&0xF; //isolate bottom nibble
    if(((unsigned int)base-subt)>15 || ((unsigned int)base-subt)<-15){
        flags->af=1;
    }else{
        flags->af=0;
    }
    return mirror;
}


unsigned char add8(unsigned char base,unsigned char adder){
    signed char result;
    if((unsigned int)adder+(unsigned int)base>(unsigned int)255){flags->cf=1;}else{flags->cf=0;}
    if((signed int)base+adder>127 || (signed int)base+adder<-127){
            flags->of=1;
        }else{
            flags->of=0;
        }
    result=base+adder;
    if(result==0){flags->zf=1;}else{flags->zf=0;}
    CalculatePF8(result); //do pf
    CalculateSF8(result); //do sf
    base=base&0xF;
    adder=adder&0xF; //isolate bottom nibble
    if((signed int)base+adder>15 || (signed int)base+adder<-15){
        flags->af=1;
    }else{
        flags->af=0;
    }
    return result;
}

unsigned short add16(unsigned short base,unsigned short adder){
    signed short result;
    if((unsigned int)adder+(unsigned int)base>(unsigned int)65535){flags->cf=1;}else{flags->cf=0;}
    if((signed int)base+adder>32767 || (signed int)base+adder<-32767){
            flags->of=1;
        }else{
            flags->of=0;
        }
    result=base+adder;
    if(result==0){flags->zf=1;}else{flags->zf=0;}
    CalculatePF16(result); //do pf
    CalculateSF16(result); //do sf
    base=base&0xF;
    adder=adder&0xF; //isolate bottom nibble
    if((signed int)base+adder>15 || (signed int)base+adder<-15){
        flags->af=1;
    }else{
        flags->af=0;
    }
    return result;
}





void sub_al_ib(){
    unsigned char tmp;
    ip++;
    MemRead8(CS,ip,&tmp);
    *gregs8[AL]=sub8(*gregs8[AL],tmp);

}

void sub_ax_iw(){
    unsigned short iw;
    ip++;
    MemRead16(CS,ip,&iw);
    gregs16[AX]=sub16(gregs16[AX],iw);
    ip++;
}


void add_al_ib(){
    unsigned char tmp;
    ip++;
    MemRead8(CS,ip,&tmp);
    *gregs8[AL]=add8(*gregs8[AL],tmp);
}

void add_ax_iw(){
    unsigned short iw;
    ip++;
    MemRead16(CS,ip,&iw);
    gregs16[AX]=add16(gregs16[AX],iw);
    ip++;
}







