/*>>>>JuleOS<<<<*/
/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>Version 0.0.3 ALPHA<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>operations.c; includes common file functions including copy, del, mkdir ect<<<<*/
/*>>>>FDFS Version 0.0.4*/

/*2880 sectors on 1 floppy*/

byte FDFS_new(fdfs_name a)
{
byte *buffer[512];
drive_params dpar;



	dpar.drive=a.drive;
	dpar.track=0;
	dpar.head=0;
	dpar.sector=5;
	buffer[0]=3;
	/*write_disk(dpar,*buffer);*
	*buffer=read_disk(dpar);*/
	if (buffer[0]!=3) {printf("error level 1");}

}

byte FDFS_load(fdfs_name a)
{
	
}

byte FDFS_del(fdfs_name a)
{
	
}


