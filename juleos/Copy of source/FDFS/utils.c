/*>>>>JuleOS<<<<*/
/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>Version 0.0.3 ALPHA<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>utilities.c; includes disk utilities such as defrag, format, ect<<<<*/
/*>>>>FDFS Version 0.0.4*/

/*this file will also be compiled for standalone use to work from windows/dos and include a few functions from operations*/

byte FDFS_format(byte drive)
{
	byte buffer[512];
	byte huh[20];
	byte *ptr;
	byte counter;
	byte error;
	drive_params dpar;
	dpar.drive=drive;
	dpar.track=0x00;
	dpar.head=0x00;
	dpar.sector=1;
	*buffer=0;
	for (;;){
		if (dpar.track==80) {
			dpar.track=0;
			dpar.head++;
			if (dpar.head==2) { break;}
		}
		
		error=biosdisk(3,dpar.drive,dpar.head,dpar.track,1,18,&buffer);
				if (error>0x00) {printf("error @format");return error; }
				dpar.track++;
	}
	
	*buffer=0x16C800; buffer[5]='0';buffer[6]='9';buffer[7]=1;buffer[8]=4; buffer[10]=2;buffer[12]=1; buffer[30]=0;/*header*/
	dpar.sector=4;
	/*error=biosdisk(3,dpar.drive,dpar.head,dpar.track,dpar.sector,1,&buffer);*/
	/*write_disk2(18,dpar.track,dpar.sector,dpar.head,dpar.drive,0x5000,0);*/
	if (error>0x00) {printf("error @writing");return error; }
	return 0;
}
	









	/*
	drive_params dpar;
	byte *buffer[512];
	byte *empty[512];
	byte error;
	byte counter;
	dpar.drive=drive;
	dpar.track=0x00;
	dpar.head=0x00;
		dpar.sector=1;
		for (;;) {
			printf(".");

		if (dpar.sector==19) {
			dpar.sector=1;
			dpar.track++;
		}
		if (dpar.track==80) {
			dpar.track=0;
			dpar.head++;
		}
		if (dpar.head==2) { break;}
		error=write_disk(dpar,*empty);
		if (error>0x00) {return error;}
		dpar.sector++;
	}
	/*write to be made bootsector +1sec =1*/
	/*write to be made bootloader +2secs =3/
	/*write header at sector 4(nto useful nwo btu will be used widely in an hdd +1sec=4 so start index at 5*
	 	dpar.track=0x00;
	dpar.head=0x00;
		dpar.sector=4;
		*buffer="FDFS 0.0.4"; /*the header*

		error=write_disk(dpar,*buffer);
		if (error>0x00) { /*if error then break*
		return error;
		}
		dpar.sector=6;
		*buffer=""; /*clear buffer*
		*buffer=3; /*dont need anything but the header byte*
		write_disk(dpar,*buffer);
		dpar.sector++;
		write_disk(dpar,*buffer); /*2 sectors reserved for index more added if needed(maybe...)*
		/*info> it reserves 29 characters for a name +1 null character 1 byte to tell if its a dir or file 1 byte to tell what sector it is located at*
*/









		



