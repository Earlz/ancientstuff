/*>>>>JuleOS<<<<*/
/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>Version 0.0.3 ALPHA<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>raw_RW.h; Contains low level disk functions also includes vital cluster operations<<<<*/
/*>>>>FDFS Version 0.0.4*/
/*may contain simplified interrupts*/
/*need write_byte commands badly*/
/*starting over since this was full of crap*/
/*starting out right*/


byte read_diskN(drive_params t,char* buffer) /*reads the disk near(only in the current segment)>uses biosdisk>working*/
{
	byte error;
	error=biosdisk(2,t.drive,t.head,t.track,t.sector,t.nsecs,buffer);
	return error;
}

byte write_diskN(drive_params t,char* buffer)/*working*/
{
	byte error;
	error=biosdisk(3,t.drive,t.head,t.track,t.sector,t.nsecs,buffer);
	return error;
}
byte read_diskF(drive_params t,word segment,word offset){ /*has a sttrange condition so therefor is not working*/
	byte error;
	int sector;
	int head;
	int drive;
	int track;
	byte nsecs;
	sector=t.sector;
	head=t.head;
	drive=t.drive;
	track=t.track;
	nsecs=t.nsecs;
	asm pusha
	asm push es
	asm mov al,nsecs
	asm mov ah,02h
	asm mov ch,track
	asm mov cl,sector
	asm mov dh,head
	asm mov dl,drive
	asm mov bx,segment
	asm mov es,bx
	asm mov bx,offset
	asm int 13h
	asm mov error,ah
	asm pop es
	asm popa
	return error;
}

byte write_diskF(drive_params t,word segment,word offset){/*same as above*/
	byte error;
	int sector;
	int head;
	int drive;
	int track;
	byte nsecs;
	sector=t.sector;
	head=t.head;
	drive=t.drive;
	track=t.track;
	nsecs=t.nsecs;
	asm pusha
	asm push es
	asm mov al,nsecs
	asm mov ah,03h
	asm mov ch,track
	asm mov cl,sector
	asm mov dh,head
	asm mov dl,drive
	asm mov bx,segment
	asm mov es,bx
	asm mov bx,offset
	asm int 13h
	asm mov error,ah
	asm pop es
	asm popa
	return error;
}

drive_params sectadr(word LBA){/*working from what i can see*/
		word scratch;
		drive_params buf;
	/*d.sector+(d.track+1)*18*(d.head+1)=secadr; /*dang got to reverse caclulate all that junk*/
	/*got no frikkin idea *_*  36 */
	#define SPT 81
	#define Heads 2
buf.sector = (LBA%SPT)+1;
buf.track = (LBA/SPT)/Heads;
buf.head = (LBA/SPT)%Heads;
	return buf;
}

byte abswriteN(byte drive,byte num,word secadr,char* buffer){ /*working*/
	drive_params t;
	byte error;
	t=sectadr(secadr);
	t.drive=drive;
	t.nsecs=num;
	error=write_diskN(t,buffer);
	return error;
}

byte absreadN(byte drive,byte num,word secadr,char* buffer){/*working*/
	drive_params t;
	byte error;
	t=sectadr(secadr);
	t.drive=drive;
	t.nsecs=num;
	error=read_diskN(t,buffer);
	return error;
}

byte absreadF(byte drive,byte num,word secadr,word segment,word offset){ /*far disk commands must be fixed first*/
	drive_params t;
	byte error;
	t=sectadr(secadr);
	t.drive=drive;
	t.nsecs=num;
	error=read_diskF(t,segment,offset);
	return error;
}

/*all of the RAW disk operations I can think of and I have enough to build an indexing system*/
















	