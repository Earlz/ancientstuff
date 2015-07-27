#include <k_JouleOS.h>

unsigned char farjmp_template[]={
    0x58,0x0, //pop ax
    0x58,0x0,
    0x58,0x0,
    //0x58,0,
    //0x58,0,
    //0x58,0,
    0x58,0x0, //pop 6 values.. and for args...one for call to this template, then to the 2 args and the call to farjmp
    0xEA, //jmp ptr16:off32
    0,0, //segment
    0,0,0,0}; //offset


void farjmp(unsigned short segment,unsigned int offset){ //this only pops enough stuff to get rid of the overhead of this function!
    unsigned short *tmp;
    unsigned int *tmp2;
    void (*tmp3)(void);
    tmp=&farjmp_template[9];
    *tmp=segment;
    tmp++;
    tmp2=tmp;
    *tmp2=offset;
    for(;;){}
    tmp3=farjmp_template;
    tmp3();
    for(;;){} //shouldn't get to here...


}









