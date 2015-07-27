#include "../_base.h"
#include <stdio.h>
/**This file contains all in and out functions(including those for device side)**/

volatile Port **ports; //allocated by malloc

//an unused port should be set to NULL and a used port should be a valid function



inline void OutPort8(unsigned short num,unsigned char val){
    Port func;
    if(ports[num]==NULL){Warning("\nA port has been attempted to be accessed\nbut it has no device function associated with it\n\n..ignoring this opcode..\n");}
    else{
        func=ports[num]; //we have to have that first 0 because it is a ptr to an array of ptr's
        func(val,0,1);
    }
}

inline void OutPort16(unsigned short num,unsigned char val){
    Port func;
    if(ports[num]==NULL){Warning("\nA port has been attempted to be accessed\nbut it has no device function associated with it\n\n..ignoring this opcode..\n");}
    else{
        func=ports[num];
        func(val,0,2);
    }
}

inline unsigned char InPort8(unsigned short num){
        if(ports[num]==NULL){Warning("\nA port has been attempted to be accessed\nbut it has no device function associated with it\n\n..ignoring this opcode..\n");}
    else{
        return ports[0][num](0,1,1);
    }
}

inline unsigned short InPort16(unsigned short num){
        if(ports[num]==NULL){Warning("\nA port has been attempted to be accessed\nbut it has no device function associated with it\n\n..ignoring this opcode..\n");}
    else{
        return ports[0][num](0,1,2);
    }
}



void out_imm8_al(){
    unsigned char tmp;
    ip++;
    MemRead8(CS,ip,&tmp);
    OutPort8(tmp,*gregs8[AL]);
}

void out_imm8_ax(){
    unsigned char tmp;
    ip++;
    MemRead8(CS,ip,&tmp);
    OutPort16(tmp,gregs16[AX]);
}





















