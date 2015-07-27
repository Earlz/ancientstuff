#include "_base.h"
#include <stdio.h>


void panic(char *msg){
	register char cmd;
	printf(">>PANIC!<<\n");
	printf(msg);
	printf("Opcode is: 0x%x and is at eip 0x%x",(unsigned char)*iptr,eip);
	printf("\n(K)ill,(S)tep over,(C)ontinue,(D)ump stack, registers, and code\n");
	cmd=getc(stdin);
	switch(cmd){
		case 'k':
		//for now no ending stuff
		exit(0);
		break;
		case 's':
		ip++;
		doeip;doiptr;
		break;
		case 'c':
		ip++;
		doeip;doiptr;
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
    printf(">>Warning!<<\n");
    printf("this is being displayed because something trivial has been done:\n");
    printf("Opcode is: 0x%x and is at eip 0x%x",(unsigned char)*iptr,eip);
    printf(msg);
}










