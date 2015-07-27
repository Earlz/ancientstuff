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
void flush_index(byte);
void reset_disks(byte drive)
{
	asm mov ah,0x00
		asm mov dl,drive
		asm int 0x13
}

/*
void read_disk(word pdrive) /*send it the address*
{
word *cylinder;
word *sector;
	word sectora,cylindera;
	byte *head;
	byte *drive;
	byte heada,drivea;
cylinder=pdrive+2; /*generates error..wtf without the * *
sector=pdrive+3;
head=pdrive+4;
drive=pdrive+5;
sectora=*sector;
cylindera=*sector;
heada=*head;
drivea=*drive;
if (cylinder==1) {
		printf("yay!");
}

}
*/


/*USE BIOS.H, BIOSDISK() INSTEAD*/
/* DO NOT CHANGE THIS!*/
unsigned char read_disk(drive_params t) /*reads a single sector and returns it */
{
	byte ret[512];
	byte err;
	err=biosdisk(2,t.drive,t.head,t.track,t.sector,1,&ret);
	if (err>0){printf("error");}
		return *ret;
}

byte write_disk(drive_params t,byte *data) /*writes a single sector*/
{
	byte ret;
	byte temp;
		byte rety[512];
		*rety=*data;
	ret=biosdisk(3,t.drive,t.head,t.track,t.sector,1,&rety);

	if (ret>0){printf("error @write");}
	temp=ret;
	return ret;
}

/* to cache or not to cache?*/
/* obsolete below; now have the plan on using a cache rather than reading from disk all the time; flush to disk if something to the index is written; use less than 32 bytes for entries since this will cause outrageous disk and memory usage(cache)*
byte writeindex(byte drive, byte data[32],byte address) /*internal function; writes a 32byte entry into the index of a specified drive, warning will overwrite!, if address is 0 then it takes the first entry that is free*
{
	drive_params t;
	byte *buffer;
	byte adr;
	byte ret;
	
	t.drive=drive;

	/*add error checking here*
	if (address==0){

	
}else{
	t.sector=4;
	*buffer=read_disk(t);
	adr=address*32;
	if (adr=>512)
	*buffer[adr]=data;
	write_disk(t,*buffer)
}



}
*/
/* scratchpad :    80(0), 18(1),1(0)  2916 sectors in 1 floppy(hd)          formula: sector+(track+1)*18*(head+1)-theoretically      */

word sectoradr(drive_params d) /*>>>WORKING<<<*//*also know as absolute sector*/
{
	char *buffer;
	word ret;
	ret=d.sector+d.track*18+d.head*1458; /*now that wasnt so bad was it*/ /*theory wrong there can be duplicate abssectors using different drive_params*/
	return ret;
}
/*grr i hate math*/
drive_params dsadr(word secadr,drive_params buf) /*>>>WORKING<<<*/
{
	word scratch;
	/*d.sector+(d.track+1)*18*(d.head+1)=secadr; /*dang got to reverse caclulate all that junk*/
	/*got no frikkin idea *_*  36 */
	buf.head=0;buf.track=0;buf.sector=0;
	buf.head=secadr/1458;
	secadr=secadr-buf.head*1458;
	buf.track=secadr/18;
	secadr=secadr-buf.track*18;
	buf.sector=secadr;
	if (buf.sector==0){buf.sector=1;}
	if (buf.track==80){buf.track=0;buf.head=1;}
	return buf;
}

byte abswrite(byte drive,word secadr,byte* data) /*abs working*/
{
	drive_params y;
	byte error;
	byte buffer[512];
	*buffer=0;
	y=dsadr(secadr,y);
	y.drive=drive;
	error=biosdisk(3,y.drive,y.head,y.track,y.sector,1,&buffer);
return error;
}


byte* absread(byte drive,word secadr,byte *buffer)
{
	drive_params y;
	y=dsadr(secadr,y);
	y.drive=drive;
biosdisk(2,y.drive,y.head,y.track,y.sector,1,buffer);
	return buffer;
}


/*
void cacheindex(byte drive)
{
	byte buffer[512];
	drive_params t;
	word adr;
	word tmp;
	bytes em;
	adr=4;
	*buffer=absread(drive,adr,&buffer);
	em.high=buffer[16]; em.low=buffer[15];
	tmp=byte2word(em);
	if (tmp==0) { /*if there is no more index's*
	load2mem(index_seg,0,&buffer,512);
	}else{
		for(;;){
				adr=tmp;
				buffer=absread(drive,adr,&buffer);
				load2mem(index_seg,0,&buffer,512);
				em.high=buffer[16]; em.low=buffer[15];
				tmp=byte2word(em);
				if(tmp==0){break;}
			}
		}

}
*/
void cacheindex(byte drive){ /*populates gindex and caches the entire index to page 0x14ff*/
	byte buffer[512];
	bytes em;
	drive_params y;
	word tmp;
	byte cnt;
	byte error;
	y=dsadr(4,y);
	y.drive=drive;
	*buffer=0;
	printf("test line 181");
	error=biosdisk(2,y.drive,y.head,y.track,y.sector,1,&buffer);
	if (error>0){printf("error");}
	printf(".");
	load2mem(index_seg,0,&buffer,512);
	printf(".");
	gindex.number=buffer[12];
	gindex.lastptr=512*gindex.number-512;
	gindex.ver[0]=buffer[5];gindex.ver[1]=buffer[6];
	gindex.table1[0]=4; 
	cnt=1;
	gindex.lastadr=4;
	if (gindex.number!=1){

	while (cnt!=gindex.number-1){
		printf(itoa(cnt,buffer,10));
		printf(" ");
		em.low=buffer[15];em.high=buffer[16];
		tmp=byte2word(em);
		y=dsadr(tmp,y);
		y.drive=drive;
		error=biosdisk(2,y.drive,y.head,y.track,y.sector,1,&buffer);
		if (error>0){printf("error");}
		load2mem(index_seg,cnt*512,&buffer,512);
		gindex.table1[cnt]=tmp;
		if (cnt>=gindex.number-1){gindex.lastadr=tmp; return 0; printf("hmm"); getkey();}
		cnt++;
}
}

}

byte* get_index(word num)
{
	byte *index[512];
	word a;
	num=num-1;
	while (a<=512){
		
		index[a]=peekb(index_seg,num*512);
		a++;
	}
	return index;
}

byte* get_entry(word num,byte *buf){
	byte i;
	i=0;
	while (i<=16){
	peekb(index_seg,num*16-16+i,buf);
	buf++;
	i++;
}
return buf;	
}
byte getindex_num(word num){
	byte ret;
	ret=num/32;
	return ret+1;
}

void write_entry(word num,byte *buf){
	byte i;
	i=0;
	while (i<=16){
	pokeb(index_seg,num*16-16+i,*buf);
	buf++;
	i++;
}
printf(".");
flush_index(1);
}


void flush_index(byte num)
{
	drive_params y;
	word i;
	byte buffer[512];
	byte* ptr;
	byte error;
	*buffer=0;
		ptr=0;
	y=dsadr(gindex.table1[num],y);
	y.drive=0; /*change later*/
	i=0;
	num--;
			ptr=&buffer;
	while (i<=512){
		peekb(index_seg,num*512+i,ptr);
		printf(*ptr);
		i++;
		ptr++;
	}
	error=biosdisk(3,y.drive,y.head,y.track,y.sector,1,&buffer);
	if (error>0){printf("error");}
}
/*WORKING disk io code


/* DO NOT CHANGE THIS!
unsigned char read_disk(drive_params t) /*reads a single sector and returns it *
{
	byte ret[512];
	byte err;
	err=biosdisk(2,t.drive,t.head,t.track,t.sector,1,&ret);
	if (err>0){printf("error");}
		return *ret;
}

byte write_disk(drive_params t,byte *data[512]) /*writes a single sector*
{
	byte ret;
	byte temp;
		byte rety[512];
		*rety=*data;
	ret=biosdisk(3,t.drive,t.head,t.track,t.sector,1,&rety);

	if (ret>0){printf("error @write");}
	temp=ret;
	return ret;
void testing(void) /*just a reserved function for testing junk*
{
	dword tm;
	char *buffer[512];
	byte ret;
	drive_params tmp;
	buffer[0]='h';
tmp.drive=0;
	tmp.track=1;
	tmp.head=0;
	tmp.sector=1;
	write_disk(tmp,"h");
	*buffer=read_disk(tmp);
	if (buffer[0]=='h'){printf("yay");}
/*
write_disk(tmp,*buffer);
	buffer[0]=0;buffer[1]=0;

	printf(*buffer);
	if (buffer[0]=='h'){printf("yay");}
*

}

*/









	