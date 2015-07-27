/*this is a template driver source file for "dynamic linking driver" design #1
there pointer is a pointe at 4000h:0000 and is segment:base(4 bytes)
the pointer to pointer thing is only done for ease of use, and you can use the original pointer aS it will not change
for the running of juleos

the os will give you until you issue an int 31h(ah=0, al=error code, return from driver init) to perform any intitializations
al will be zero if there is no error on ALL drivers and is unsigned, al>0 means error and the os will treat as such

this template file is only as a design, built in turboc syntax*/
/*this is free to use as a template for anything, no strings attached*/
char* argspace;
char local[256]; /*a local buffer for ease of reading, limited to 256 bytes although pointers can be in those so it is limitless truly*/
char ect[127];
word ipptr;
void main(void) { /*due to the way most C/C++ compilers handle returning and getting args to main these should be void*/

	init();
	asm int 31h
	/*an int pushes the ip so it returns here*/
	asm return_here:
	
asm int 31h
/*DO 
}

void init(void){
	word seg;
	word offset;
	byte tmp1,tmp2;
	word i;
	peekb(0x4000,0x0000,&tmp1); /*get the pointer segment -low byte*/
	peekb(0x4000,0x0001,&tmp2); /*high byte*/
	asm push ax
	asm mov ah,tmp1
	asm mov al,tmp2
	asm mov seg,ax 
	asm pop ax
	/*do some byte to word conversion*/
	peekb(0x4000,0x0002,&tmp1); /*get the pointer offset -low byte*/
	peekb(0x4000,0x0003,&tmp2);/*high byte*/
	asm push ax
	asm mov ah,tmp1
	asm mov al,tmp2
	asm mov offset,ax 
	asm pop ax	
	while (i<=255){
		peekb(seg,offset+i,&local+i); /*read arg into local*/
		i++;
	}
	peekb(0x4000,0x0004,&ect+0); /*this is the counter for the driver return ip table*/
	asm push ax
	asm lea ax,return_here
	asm mov ipptr,ax
	/*get the return_here label ip, move it to ect[2]*/
	asm ax,ipptr
	asm mov tmp1,ah
	asm mov tmp2,al
	asm pop ax
	/*this puts the return me label ip onto the driver return ip table*/
	pokeb(0x40A0,ect[0],tmp1);
	pokeb(0x40a0,ect[0]+1,tmp2); /*and that writes the return_here offset to the driver return ip table at counter ect[0] */
	/*and thats just the basics of it all*/
	/*no more inits for now*/

}
