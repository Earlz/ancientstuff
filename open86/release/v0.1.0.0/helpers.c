//this has all sorts of helper functions for simplifying...
#include "_base.h"


SPEED_UP unsigned int MemRead8(unsigned char segment,unsigned short offset, unsigned char *buffer){
    unsigned char *ptr;
    unsigned long tmp;
    ptr=offset+(sregs[segment]<<4); //calculate effective address
   // if(ptr>0xFFFFF && segment!=SS){return GPF;}
   // if(ptr>0xFFFFF && segment==SS){return STACK_FAULT;}  //this is impossible to happen!
    tmp=core;
    ptr=ptr+tmp;
    *buffer=*ptr;
    return 0;
}


SPEED_UP unsigned int MemWrite8(unsigned char segment,unsigned short offset, unsigned char *buffer){
    unsigned char *ptr;
    if(segment==CS){return GPF;} //not sure if GPF is the proper error..
    ptr=offset+(sregs[segment]<<4);
    if(ptr>0xC0000 && segment!=SS){return GPF;}
    if(ptr>0xC0000 && segment==SS){return STACK_FAULT;}
    ptr=ptr+(unsigned long)core; //add emulator offset
    *ptr=*buffer;
    return 0;
}

SPEED_UP unsigned int MemRead16(unsigned char segment,unsigned short offset, unsigned short *buffer){
    unsigned short *ptr;
    ptr=offset+(sregs[segment]<<4); //calculate effective address
    //if(ptr>(0xFFFFF-1) && segment!=SS){return GPF;}
    //if(ptr>(0xFFFFF-1) && segment==SS){return STACK_FAULT;}
    (unsigned long)ptr=(unsigned long)ptr+(unsigned long)core;
    *buffer=*ptr;
    return 0;
}


SPEED_UP unsigned int MemWrite16(unsigned char segment,unsigned short offset, unsigned short *buffer){
    unsigned short *ptr;
    if(segment==CS){return GPF;} //not sure if GPF is the proper error..
    ptr=offset+(sregs[segment]<<4);
    if(ptr>(0xC0000-1) && segment!=SS){return GPF;}
    if(ptr>(0xC0000-1) && segment==SS){return STACK_FAULT;}
    (unsigned long)ptr=(unsigned long)ptr+(unsigned long)core; //add emulator offset
    *ptr=*buffer;
    return 0;
}

SPEED_UP unsigned int MemCheck_write8(unsigned char segment,unsigned short offset){
    unsigned char *ptr;
    if(segment==CS){return GPF;} //not sure if GPF is the proper error..
    ptr=offset+(sregs[segment]<<4);
    if(ptr>0xC0000 && segment!=SS){return GPF;}
    if(ptr>0xC0000 && segment==SS){return STACK_FAULT;}
    return 0;
}

SPEED_UP unsigned int MemCheck_write16(unsigned char segment,unsigned short offset){
    unsigned char *ptr;
    if(segment==CS){return GPF;} //not sure if GPF is the proper error..
    ptr=offset+(sregs[segment]<<4);
    if(ptr>(0xC0000-1) && segment!=SS){return GPF;}
    if(ptr>(0xC0000-1) && segment==SS){return STACK_FAULT;}
    return 0;
}

SPEED_UP unsigned int MemCheck_read8(unsigned char segment,unsigned short offset){
    //unsigned char *ptr;
    //if(segment==CS){return GPF;} //not sure if GPF is the proper error..
    //ptr=offset+(sregs[segment]<<4);
    //if(ptr>0xFFFFF && segment!=SS){return GPF;}
    //if(ptr>0xFFFFF && segment==SS){return STACK_FAULT;}
    return 0;
}

SPEED_UP unsigned int MemCheck_read16(unsigned char segment,unsigned short offset){
    //unsigned char *ptr;
    //if(segment==CS){return GPF;} //not sure if GPF is the proper error..
    //ptr=offset+(sregs[segment]<<4);
    //if(ptr>(0xFFFFF-1) && segment!=SS){return GPF;}
    //if(ptr>(0xFFFFF-1) && segment==SS){return STACK_FAULT;}
    return 0;
}



//this file has tons of warnings but if I do stuff like converting them all to (unsigned int)
//then it makes the code look super ugly and bulky, so I had rather live with wanrings



SPEED_UP void CalculatePF8(unsigned char val){
    unsigned int i;
    unsigned int count=0;
    for(i=0;i<=7;i++){
        if((val&(0|(1<<i)))!=0){count++;}
    }
    if((count%2)==0){flags->pf=1;}else{flags->pf=0;}
}

SPEED_UP void CalculatePF16(unsigned short val){
    unsigned int i;
    unsigned int count=0;
    for(i=0;i<=15;i++){
        if((val&(0|(1<<i)))!=0){count++;}
    }
    if((count%2)==0){flags->pf=1;}else{flags->pf=0;}
}

//these calculate SF for the given operand size
SPEED_UP void CalculateSF8(unsigned char val){
    if((val&0x80)==0){flags->sf=0;}else{flags->sf=1;}
}

SPEED_UP void CalculateSF16(unsigned short val){
    if((val&0x8000)==0){flags->sf=0;}else{flags->sf=1;}
}

unsigned int GetModRM_template(void* *buffer,unsigned char *uses_ss_var){ //this will interpret a 16bit mod r/m byte into a core usable address
    mod_rm rm[1];
    unsigned short *tmp;
    unsigned char *tmp2;
    unsigned char buffer8[1];
    unsigned short buffer16[1];
    unsigned char uses_ss=0; //this is set if it uses SS(unless theirs segment overrides..)
    unsigned int ptr;
    //ip++;
    MemRead8(CS,ip,rm);
    switch(rm->mod){
        case 0: //no normal description... see comments by case's below
        switch(rm->rm){
            case 0: //bx+si
            ptr=gregs16[BX]+gregs16[SI]+(sregs[CurrentSegment]<<4);
            break;
            case 1: //bx+di
            ptr=gregs16[BX]+gregs16[DI]+(sregs[CurrentSegment]<<4);
            break;
            case 2: //bp+si
            ptr=gregs16[BP]+gregs16[SI]+(sregs[StackSegment]<<4);
            uses_ss=1;
            break;
            case 3: //bp+di
            ptr=gregs16[BP]+gregs16[DI]+(sregs[StackSegment]<<4);
            uses_ss=1;
            break;
            case 4: //si
            ptr=core+gregs16[SI]+(sregs[CurrentSegment]<<4);
            break;
            case 5: //di
            ptr=gregs16[DI]+(sregs[CurrentSegment]<<4);
            break;
            case 6: //disp16
            ip++;
            if(MemRead16(CS,ip,buffer8)!=0){return MemRead8(CS,ip,buffer8);}
            tmp=buffer8;
            ptr=core+*tmp+(sregs[CurrentSegment]<<4);
            ip++;

            break;
            case 7: //bx
           // printf("shell");
            ptr=gregs16[BX]+(sregs[CurrentSegment]<<4);
            break;
        }
        break;
        case 1: //just [regs]+disp8
        switch(rm->rm){
            case 0: //bx+si+disp8
            ip++;
            MemRead8(CS,ip,buffer8);
            tmp2=buffer8;
            ptr=gregs16[BX]+gregs16[SI]+*tmp2+(sregs[CurrentSegment]<<4);
            break; //no extra advancement because this is just 1 byte disp
            case 1: //bx+di+disp8
            ip++;
            MemRead8(CS,ip,buffer8);
            tmp2=buffer8;
            ptr=gregs16[BX]+gregs16[DI]+*tmp2+(sregs[CurrentSegment]<<4);
            break;
            case 2: //bp+si+disp8
            ip++;
            MemRead8(CS,ip,buffer8);
            tmp2=buffer8;
            ptr=gregs16[BP]+gregs16[SI]+*tmp2+(sregs[StackSegment]<<4);
            uses_ss=1;
            break;
            case 3: //bp+di+disp8
            ip++;
            MemRead8(CS,ip,buffer8);
            tmp2=buffer8;
            ptr=gregs16[BP]+gregs16[DI]+*tmp2+(sregs[StackSegment]<<4);
            uses_ss=1;
            break;
            case 4: //si+disp8
            ip++;
            MemRead8(CS,ip,buffer8);
            tmp2=buffer8;
            ptr=gregs16[SI]+*tmp2+(sregs[CurrentSegment]<<4);
            break;
            case 5: //si+disp8
            ip++;
            MemRead8(CS,ip,buffer8);
            tmp2=buffer8;
            ptr=gregs16[DI]+*tmp2+(sregs[CurrentSegment]<<4);
            break;
            case 6: //bp+disp8
            ip++;
            MemRead8(CS,ip,buffer8);
            tmp2=buffer8;
            ptr=gregs16[BP]+*tmp2+(sregs[StackSegment]<<4);
            uses_ss=1;
            break;
            case 7: //bx+disp8
            ip++;
            MemRead8(CS,ip,buffer8);
            tmp2=buffer8;
            ptr=gregs16[BX]+*tmp2+(sregs[CurrentSegment]<<4);

            break;
        }
        //*buffer=ptr;
       // return 0;
        break;
        case 2: //[regs]+disp16
        switch(rm->rm){
            case 0://bx+si+disp16
            ip++;
            MemRead16(CS,ip,buffer8);
            tmp=buffer8;
            ptr=gregs16[BX]+gregs16[SI]+*tmp+(sregs[CurrentSegment]<<4);
            break;
            //ip advancement for the disp16 will be at the end..
            case 1://bx+di+disp16
            ip++;
            MemRead16(CS,ip,buffer8);
            tmp=buffer8;
            ptr=gregs16[BX]+gregs16[DI]+*tmp+(sregs[CurrentSegment]<<4);
            break;
            case 2://bp+si+disp16
            ip++;
            MemRead16(CS,ip,buffer8);
            tmp=buffer8;
            ptr=gregs16[BP]+gregs16[SI]+*tmp+(sregs[StackSegment]<<4);
            uses_ss=1;
            break;
            case 3://bp+di+disp16
            ip++;
            MemRead16(CS,ip,buffer8);
            tmp=buffer8;
            ptr=gregs16[BP]+gregs16[DI]+*tmp+(sregs[StackSegment]<<4);
            uses_ss=1;
            break;
            case 4://si+disp16
            ip++;
            MemRead16(CS,ip,buffer8);
            tmp=buffer8;
            ptr=gregs16[SI]+*tmp+(sregs[CurrentSegment]<<4);
            break;
            case 5://di+disp16
            ip++;
            MemRead16(CS,ip,buffer8);
            tmp=buffer8;
            ptr=gregs16[DI]+*tmp+(sregs[CurrentSegment]<<4);
            break;
            case 6://bp+disp16
            ip++;
            MemRead16(CS,ip,buffer8);
            tmp=buffer8;
            ptr=gregs16[BP]+*tmp+(sregs[StackSegment]<<4);
            uses_ss=1;
            break;
            case 7://bx+disp16
            ip++;
            MemRead16(CS,ip,buffer8);
            tmp=buffer8;
            ptr=gregs16[BX]+*tmp+(sregs[CurrentSegment]<<4);
            break;
        }
        ip++; //do the advancement for disp16

        break;
        case 3: //just a standard opcode size register
        *buffer=rm->rm; //if it is opcode specific, then we set buffer to something that might be useful
        //ptr=gregs8[rm->rm]; //we just get the ptr of OUR register
        return OPCODE_SPECIFIC;
        break;
    }
    *uses_ss_var=uses_ss;
    *buffer=ptr; //this is actually more the return value...
    return 0;
}





inline unsigned int GetModRM_read8(void* *buffer){ //this will interpret a 16bit mod r/m byte into a core usable address
    unsigned char uses_ss=0; //this is set if it uses SS(unless theirs segment overrides..)
    unsigned int ptr;
    if(GetModRM_template(buffer,&uses_ss)!=0){return OPCODE_SPECIFIC;} //no other choice so..
    ptr=*buffer;
    if(uses_ss==1){
        return MemCheck_read8(StackSegment,(ptr-(sregs[StackSegment]<<4))); //returns the error code of memcheck
    }else{
        return MemCheck_read8(CurrentSegment,(ptr-(sregs[CurrentSegment]<<4)));
    }
    return 0; //..just in case
}

inline unsigned int GetModRM_read16(void* *buffer){ //this will interpret a 16bit mod r/m byte into a core usable address
    unsigned char uses_ss=0; //this is set if it uses SS(unless theirs segment overrides..)
    unsigned int ptr;
    if(GetModRM_template(buffer,&uses_ss)!=0){return OPCODE_SPECIFIC;} //no other choice so..
    ptr=*buffer;
    if(uses_ss==1){
        return MemCheck_read16(StackSegment,(ptr-(sregs[StackSegment]<<4))); //returns the error code of memcheck
    }else{
        return MemCheck_read16(CurrentSegment,(ptr-(sregs[CurrentSegment]<<4)));
    }
    return 0; //..just in case
}

inline unsigned int GetModRM_write8(void* *buffer){ //this will interpret a 16bit mod r/m byte into a core usable address
    unsigned char uses_ss=0; //this is set if it uses SS(unless theirs segment overrides..)
    unsigned int ptr;
    if(GetModRM_template(buffer,&uses_ss)!=0){return OPCODE_SPECIFIC;} //no other choice so..
    ptr=*buffer;
    if(uses_ss==1){
        return MemCheck_write8(StackSegment,(ptr-(sregs[StackSegment]<<4))); //returns the error code of memcheck
    }else{
        return MemCheck_write8(CurrentSegment,(ptr-(sregs[CurrentSegment]<<4)));
    }
    return 0; //..just in case
}

inline unsigned int GetModRM_write16(void* *buffer){ //this will interpret a 16bit mod r/m byte into a core usable address
    unsigned char uses_ss=0; //this is set if it uses SS(unless theirs segment overrides..)
    unsigned int ptr;
    if(GetModRM_template(buffer,&uses_ss)!=0){return OPCODE_SPECIFIC;} //no other choice so..
    ptr=*buffer;
    if(uses_ss==1){
        return MemCheck_write16(StackSegment,(ptr-(sregs[StackSegment]<<4))); //returns the error code of memcheck
    }else{
        return MemCheck_write16(CurrentSegment,(ptr-(sregs[CurrentSegment]<<4)));
    }
    return 0; //..just in case
}


