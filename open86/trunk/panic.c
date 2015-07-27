#include "_base.h"
#include <stdio.h>
/**This file contains some very crude and simple panic and warning functions**/

void panic(char *msg){
	char cmd,tmp; /* Cannot get address of registers. */
	printf(">>PANIC!<<\n");
	printf(msg);
	MemRead8(CS,ip,&tmp);
	printf("Opcode is: 0x%x and is at eip 0x%x",tmp,(sregs[CS]<<4)+ip);
	printf("\n(K)ill,(S)tep over,(C)ontinue,(D)ump stack, registers, and code\n");
	cmd=getc(stdin);
	switch(cmd){
		case 'k':
		//for now no ending stuff
		exit(0);
		break;
		case 's':
		ip++;

		break;
		case 'c':
		ip++;

		break;
		case 'd':
		nop();
		panic("nothing to do!");
		break;
		default:
		printf("normally will default to \'k\'\n");
		break;
	}
}



void Warning(char *msg){
	unsigned char tmp;
    printf(">>Warning!<<\n");
    printf("this is being displayed because something trivial has been done:\n");
	MemRead8(CS,ip,&tmp);
	printf("Opcode is: 0x%x and is at eip 0x%x",tmp,(sregs[CS]<<4)+ip);
    printf(msg);
}










