/*special utility to write the fdfs bootsector and write juleos kernel
1.0*/
#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
 #include <stdio.h>
 typedef signed	char	s8b	;
 typedef signed	int	s16b	;
 typedef signed long	s32b	;

 typedef unsigned char	u8b	;
 typedef unsigned int	u16b	;
 typedef unsigned long	u32b	;
 typedef unsigned char byte;
 typedef unsigned int word;
 typedef unsigned long dword;
typedef struct {
int sector;
	int head;
	int drive;
	int track;
}drive_params;
unsigned char read_disk(drive_params t) /*reads a single sector and returns it */
{
	byte ret[512];
	byte err;
	err=biosdisk(2,t.drive,t.head,t.track,t.sector,1,&ret);
	if (err>0){printf("error");}
		return *ret;
}

byte write_disk(drive_params t,byte *data[512]) /*writes a single sector*/
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

void clear(char* ptr,word size)
{
	word a;
	while (a<=size){
		*ptr=0;
		ptr++;
		a++;
	}}
	


void main(void)
{drive_params t;
	byte buffer[512*5];
	word a;
	FILE *in;
	int z;
	byte counter;
	t.drive=1;
	t.head=0;
	t.track=5;
	t.sector=1;

	if ((in=fopen("t.bmp","r+"))==0) {printf("error opening file");}
	
	                  while(!feof(in)) {
	                  	
                    z= fgetc(in);
                    buffer[a]=z;
                      printf("%c",buffer[a]);
                      

                      a++;
                  }
                  buffer[0]='t';
                  a=biosdisk(3,t.drive,t.head,t.track,t.sector,1,&buffer);
                  if (a>0){printf("error");}
                clear (&buffer,512);
                  fclose(in);
                   a=biosdisk(2,t.drive,t.head,t.track,t.sector,1,&buffer);
                    if (a>0){printf("error");}
                   if (buffer[0]!='t'){printf("dang");}else{printf("yay");}
}
	















	