/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>Version 0.0.3 ALPHA<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>Shell\interface.c; Contains easy to use interfaces to rather hard to use C functions(user level, not development)<<<<*/




void sformat(void) /*provides an easy way to access the fdfs format function for the user*/
{
	keys t;
	char huh[20];
	byte *buffer;
	drive_params dp;
	byte num;
	printf("still in alpha testing stage");nl();printf("This interface for format will change a lot a bit later");nl();
	printf("Type the drive number: ");
	
	num = strtol(scanf(),huh,10);
	
	FDFS_format(0);
}