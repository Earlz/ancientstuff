#include "../_base.h"
#include <stdio.h>
/**This file includes about everything that has to do with interrupts or exceptions**/

volatile unsigned short pic_short=0; //this tells what interrupts are pending to be called
volatile struct PIC_storage *pic_struct=&pic_short;
volatile unsigned char port20h=0;
volatile unsigned char busy_interrupts=0;
volatile unsigned char current_interrupt=17;

typedef void (*PIC_CALLBACK)(unsigned char current_int);
PIC_CALLBACK PicCallBack[17];
void PICMessageEvent(Open86Message *msg);
void error_pic(unsigned char current_int){
	Warning("Warning! EOI has been sent to port 20h, but it is not in an interrupt!");
}

//PIC stuff...

void InitPic(){
	unsigned int i;
	for(i=0;i<16;i++){
		PicCallBack[i]=NULL;
	}
	PicCallBack[16]=error_pic;
	InstallMessageEvent(PIC_DEVICE,PICMessageEvent);
}

unsigned int PIC_port_20h(unsigned int data,unsigned char in_or_out,unsigned char size){
    if(in_or_out==0){
        port20h=(unsigned short)data;
        if((port20h&0x20)!=0){
        	if(PicCallBack[current_interrupt]!=NULL){
				PicCallBack[current_interrupt](current_interrupt);
        	}
        }
        current_interrupt=17;
    }else{
        return port20h;
    }
}



inline void CheckPIC(){ //This function is called by process() and will call IRQ's
	unsigned int i;
    if(pic_short==0){return;} //if there are no interrupts to be called..
    if(flags->_if==0){return;}
    if((port20h&0x20)!=0){return;} //if the pic is in the middle of an interrupt
	for(i=0;i<=15;i++){
		//printf("pie");
		if((pic_short&(1<<i))!=0){
			CallInt(8+i);
			//printf("pie");
			current_interrupt=i;
			pic_short=pic_short&(1<<i);
			break;
		}
	}

}


void SendInterrupt(unsigned int number){ //This is used in GlobStruct to say that an IRQ is pending
    pic_short=pic_short|(1<<number);
}

void PICMessageEvent(Open86Message *msg){

	switch(msg->message){
		case 0: //send interrupt
		//printf("t");
		//SendInterrupt(msg->extra);
		PicCallBack[msg->extra]=msg->extra_ptr;
		pic_short=pic_short|(1<<(msg->extra-8));
		break;
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
    sregs[IS]=0;
    MemRead16(IS,(num*4),&seg);
    if(push(sregs[CS])!=0){
        panic("no faulting!!\n");
    }
    MemRead16(IS,(num*4)+2,&off);
    if(push(ip)!=0){
        panic("no faulting!!\n");
    }
    sregs[CS]=seg;
    ip=off;
    printf("num: %i,seg: 0x%x, off: 0x%x\n",num,seg,off);
    //ip--;
//    doeip;doiptr; //just in case..
    //nop();
}

SPEED_UP void iret(){ //this instruction CAN be used without being emulated and also as an opcode!!
    ip=pop();
    sregs[CS]=pop();
    flagreg=pop();
    //doeip;doiptr;
}


void int_normal(){ //This is the actual int opcode
    unsigned char tmp;
    ip++;
    MemRead8(CS,ip,&tmp);
    CallInt(tmp);
}

void int3(){ //int 3 is a one byte opcode!
    CallInt(3);
}

void into(){
    if(flags->of==1){
        CallInt(4);
    }
}





















