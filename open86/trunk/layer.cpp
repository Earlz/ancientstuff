#include <iostream>
#include <io.h>
#include <stdio.h>
//#include <conio.h>
#include "x86lib.h"

using namespace std;
using namespace x86Lib;
extern "C"{
	extern char core[];
}

static const uint32_t ROM_START=0xC0000;
/**Memory Structures**/
PhysMemory::PhysMemory(){
	size=0xFFFFF;
//	cout << hex << "size: 0x" << size<< endl;
	ptr=(uint8_t*)core;
	//memset(ptr,0,size); //initialize it all to 0
	//I don't know C++ streams...yay...lol
	//Load bios...
	FILE *blah;
    blah=fopen("bios.bin","r"); //Load ROM(must be done internally)
	if(blah==NULL){for(;;){}}
	uint32_t i=0;
	while(feof(blah)==0){
	   ptr[0xF0000+i]=fgetc(blah);
	    i++;
	}
	fclose(blah);
}
PhysMemory::~PhysMemory(){
	delete []ptr;
}

uint8_t PhysMemory::ReadByte(uint32_t off){
	off&=0xFFFFF; //A20 Wrap Around
	if(off>(size)){
		throw Mem_excp(off);
	}
	return ptr[off];
}

uint16_t PhysMemory::ReadWord(uint32_t off){
	off&=0xFFFFF;
	if(off>(size-1)){
		throw Mem_excp(off);
	}
	return *(uint16_t*)&ptr[off];
}

uint32_t PhysMemory::ReadDword(uint32_t off){
	off&=0xFFFFF;
	if(off>(size-3)){
		throw Mem_excp(off);
	}
	return *(uint32_t*)&ptr[off];
	//just so I don't forget...
	//this will get the address of ptr[off] as a uint8 and then put that address
	//into a uint32 pointer, and then the * in front will dereference the new 32bit pointer
}

void PhysMemory::WriteByte(uint32_t off,uint8_t val){
	off&=0xFFFFF;
	if(off>(ROM_START)){
		throw Mem_excp(off);
	}
	ptr[off]=val;
}

void PhysMemory::WriteWord(uint32_t off,uint16_t val){
	off&=0xFFFFF;
	if(off>(ROM_START-1)){
		throw Mem_excp(off);
	}
	*(uint16_t*)&ptr[off]=val;
}

void PhysMemory::WriteDword(uint32_t off,uint32_t val){
	off&=0xFFFFF;
	if(off>(ROM_START-3)){
		throw Mem_excp(off);
	}
	*(uint32_t*)&ptr[off]=val;
}

uint32_t PhysMemory::GetSize(){
	return size;
}

bool PhysMemory::CanRead(uint32_t off){
	off&=0xFFFF;
	if(off<=(size-1)){
		return true;
	}else{
		return false;
	}
}

bool PhysMemory::CanWrite(uint32_t off){
	off&=0xFFFF;
	if(off<=(ROM_START-1)){
		return true;
	}else{
		return false;
	}
}

PhysMemory memory;
x86CPU *cpu_ctrl;
x86Ports ports_;

void WritePort(uint16_t port,uint32_t val){
	/*Not going to try to emulate actual hardware...but rather just give us some useful
	functions to try out...*/
	switch(port){
		case 0: //print asci char of val
		cout << (char) val << flush;
		break;
		case 1: //print value of byte
		cout << hex << (int)(uint8_t)val;
		break;
		case 2: //print value of word
		cout << hex << (int)(uint16_t)val;
		break;
		case 3: //print value of dword
		cout << hex << (int)(uint32_t)val;
		break;
		case 4: //cause an interrupt
		break;

		case 0xF0: //exit with val
		exit(val);
		break;
		case 0xF1: //dump small bit of memory
		cout << "debug info...Dumping 0:0-to-0:200" << endl;
		cout << "0: "<< hex << memory.ReadDword(0) << endl;
		cout << "4: "<< hex << memory.ReadDword(4) << endl;
		cout << "8: "<< hex << memory.ReadDword(8) << endl;
		cout << "B: "<< hex << memory.ReadDword(12) << endl;
		cout << "10: "<< hex << memory.ReadDword(16) << endl;
		cout << "14: "<< hex << memory.ReadDword(20) << endl;
		cout << "18: "<< hex << memory.ReadDword(24) << endl;
		cout << "1B: "<< hex << memory.ReadDword(28) << endl;
		cout << "20: "<< hex << memory.ReadDword(32) << endl;
		break;
		case 0xF2: //a two word command; first is segment, second is offset; prints value of memory there
		static uint16_t seg_temp;
		static bool flip_flop=0;
		if(flip_flop==1){
			flip_flop=0;
			cout <<hex << memory.ReadDword((seg_temp<<4)|val) << endl;
		}else{
			flip_flop=1;
			seg_temp=val;
		}
		break;
		case 0xF20F: //clear flip-flop of F2
		flip_flop=0;
		break;

		default:
		cout << "undefined port" << endl;
		break;
	}


}

x86Ports::x86Ports(){
	cout << "a" << endl;
}

x86Ports::~x86Ports(){
	cout << "e" << endl;
}

void x86Ports::WriteByte(uint16_t port,uint8_t val){
	WritePort(port,val);
}

void x86Ports::WriteWord(uint16_t port,uint16_t val){
	WritePort(port,val);
}

void x86Ports::WriteDword(uint16_t port,uint32_t val){ //not used yet

}

uint8_t x86Ports::ReadByte(uint16_t port){
	return 0;
}

uint16_t x86Ports::ReadWord(uint16_t port){
	return 0;
}

uint32_t x86Ports::ReadDword(uint16_t port){ //not used yet
	return 0;
}





extern "C" {
void x86Init(){
	cpu_ctrl=new x86CPU(&memory,&ports_,0,0);
}
void DoCycle(){
		try{
		cpu_ctrl->Cycle();
	}
	catch(CpuPanic_excp err){
		cout << "CPU Panic!" <<endl;
		cout << "Message: " << err.desc << endl;
		cout << "Code: 0x" << hex << err.code << endl;
		cout << "Opcode: 0x" << hex << (int)op_cache[0] << endl;
		cpu_ctrl->DumpState();

		for(;;){}
	}
	catch(Default_excp err){
		cout << "!!Undefined Error!!" << endl;
		cout << "File: " << err.file << endl;
		cout << "Function: " << err.func << "()" <<endl;
		cout << "Line: " << err.line << endl;
		for(;;){}
	}
}

}


