#include "../_base.h"


volatile unsigned short pic_short=0; //this tells what interrupts are pending to be called
volatile struct PIC_storage *pic_struct=&pic_short;
volatile unsigned char port21h=0;
//PIC stuff...
unsigned int PIC_port_21h(unsigned int data,unsigned char in_or_out,unsigned char size){
    if(in_or_out==0){
        port21h=(unsigned char)data;
    }else{
        return port21h;
    }
}


inline void CheckPIC(){
    if(pic_short==0){return;} //if there are no interrupts to be called..
    if((port21h&0x20)!=0){return;} //if the pic is in the middle of an interrupt
    if((pic_short&1)!=0){
        //first interrupt(zero) needs to be called..
        CallInt(8+0);
        return;
    }
    if((pic_short&2)!=0){
        CallInt(8+1);
        return;
    }
    if((pic_short&4)!=0){
        CallInt(8+2);
        return;
    }
    if((pic_short&8)!=0){
        CallInt(8+3);
        return;
    }
    if((pic_short&16)!=0){
        CallInt(8+4);
        return;
    }
    if((pic_short&32)!=0){
        CallInt(8+5);
        return;
    }
    if((pic_short&64)!=0){
        CallInt(8+6);
        return;
    }
    if((pic_short&128)!=0){
        CallInt(8+7);
        return;
    }
    if((pic_short&255)!=0){
        CallInt(8+8);
        return;
    }
    if((pic_short&512)!=0){
        CallInt(8+9);
        return;
    }
    if((pic_short&1024)!=0){
        CallInt(8+10);
        return;
    }
    if((pic_short&2048)!=0){
        CallInt(8+11);
        return;
    }
    if((pic_short&4096)!=0){
        CallInt(8+12);
        return;
    }
    if((pic_short&8192)!=0){
        CallInt(8+13);
        return;
    }
    if((pic_short&16384)!=0){
        CallInt(8+14);
        return;
    }
    if((pic_short&32768)!=0){
        CallInt(8+15);
        return;
    }


}





SPEED_UP void CallInt(unsigned char num){
    unsigned short seg,off;
    unsigned short *ptr;
    if(push(flagreg)!=0){ //pushf
        panic("no faulting!!\n");
    }
    flags->tf=0;
    flags->_if=0;
    //far call
    (unsigned char *)ptr=core+(num*4);
    seg=*ptr;
    if(push(sregs[CS])!=0){
        panic("no faulting!!\n");
    }
    (unsigned char *)ptr=core+(num*4)+2;
    off=*ptr;
    if(push(ip)!=0){
        panic("no faulting!!\n");
    }
    sregs[CS]=seg;
    ip=off;
    ip--;
    //doeip;doiptr; //just in case...
}

SPEED_UP void iret(){ //this instruction CAN be used without being emulated and also as an opcode!!
    ip=pop();
    sregs[CS]=pop();
    flagreg=pop();
    //doeip;doiptr;
}


void int_normal(){
    unsigned char tmp;
    ip++;
    MemRead8(CS,ip,&tmp);
    CallInt(tmp);
}

void int3(){
    CallInt(3);
}

void into(){
    if(flags->of==1){
        CallInt(4);
    }
}





















