/*working so far*/
void nhelp(void);
void nwrite(void);
void nread(void);
void Scheck_scroll(void);
void nset(void);
byte Ndrive=0;
byte Nsect=80;
void notes(void){
	byte buffer[16];
	byte didit;
	/*8cacheindex(0);*/
	printf("welcome to Notes beta");nw
	printf("This is quite unstable but demonstrates index's and is more of a diagnostic utility");nw
		printf("type help for list");nw
	for (;;){

	input(&buffer);
	if (streq(buffer,"help")==1){nhelp(); didit=1;}
	if (streq(buffer,"read")==1){nread(); didit=1;}
	if (streq(buffer,"write")==1){nwrite(); didit=1;}
	if (streq(buffer,"set")==1){nset();didit=1;}
	if (streq(buffer,"exit")==1){return 0; didit=1;}
	Scheck_scroll();
}	
}



void nhelp(void){
	printf("read -reads an index entry");nw
	printf("write -writes an index entry");nw
	printf("help -displays this");nw
	printf("new -creates a new index, not working");nw
	printf("exit -exits Notes"); nw
}

void nread(void)
{
	byte buffer[512];
	byte cmd[16];
	drive_params t;
	byte *ptr;
	t=sectadr(90);
	t.drive=0;t.nsecs=1;
	printf("This is what was stored on the floppy");nw
	absreadN(Ndrive,1,Nsect,&buffer);
	printf(&buffer);

}
void nwrite(void){
		byte buffer[512];
	byte cmd[16];
	byte *ptr;
	drive_params t;
	t=sectadr(90);
	t.drive=0;t.nsecs=1;
	printf("type what you want stored on the floppy ");nw
	input(&buffer);
	abswriteN(Ndrive,1,Nsect,&buffer);
	
}

void nset(void){
	int n;
	
	printf("drive number(default is 0: ");n=inputn();Ndrive=n;
	nw
	printf("abssector number(default is 80): ");n=inputn();Nsect=n;
}
