/*>>>>JuleOS<<<<*/
/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>Version 0.0.3 ALPHA<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>init.c; contains initializations,structs and defines<<<<*/
/*>>>>FDFS Version 0.0.4*/

#ifndef _FDFS
#define _FDFS
#define FDFS_VER 0.0.4
#define FDFS_VERs "0.0.4"
byte *DISK_STATUS_ERROR;
/*Disk Status Constant Error Values*/
#define DISK_NO_ERROR 0x00
#define DISK_INVALID 0x01
#define DISK_ADDRESS_MARK_NOT_EXIST 0x00
#define DISK_WRITE_PROTECT_ERROR 0x03
#define DISK_SECTOR_NOT_FOUND 0x04
#define DISK_RESET_FAILURE 0x05
#define DISK_CHANGE_LINE 0x06
#define DISK_DRIVE_PARAM_FAIL 0x07
#define DISK_DMA_OVERRUN 0x08
#define DISK_ DMA_OVER_64k 0x09
#define DISK_BAD_SECTOR 0x0A
#define DISK_BAD_CYLINDER 0x0B
#define DISK_NO_MEDIATYPE 0x0C
#define DISK_INVALID_SECTOR_NUM 0x0D
#define DISK_CTRL_ADDRESS_MARK 0x0E
#define DISK_DMA_OUT_RANGE 0x0F
#define DISK_CRC_ECC_DATA_ERROR 0x10
#define DISK_ECC_CORRECTED 0x11
#define DISK_CONTROLLER_FAIL 0x20
#define DISK_SEEK_FAIL 0x40
#define DISK_NOT_READY 0x80
#define DISK_UNDEFINED 0xBB
#define DISK_WRITE_FAULT 0xCC
#define DISK_STATUS_ERROR 0xE0
#define DISK_SENSE_FAIL 0xFF
/*end*/
#define SET 1

typedef struct {
	byte drive;
	byte*dir;
	byte*name;
}fdfs_name; 

typedef struct{
	char name[13];
	byte attrib;
	word lba;
}fdfs_indexn; /*normal index entry*/




typedef struct{
	fdfs_indexn[31];
}




word fstable[255];



#endif




/*

void FDFS_init(void){
drive_params h;
char *buffer[512];
byte error;
resetdisk(0x81); /*reset all*
h.sector=1;
	*buffer=read_disk(h);
	error=write_disk(h,*buffer);
	if (error>0){printf("an error has occured in the disk subsystem when writing");nl();}
	if (error==DISK_WRITE_PROTECT_ERROR){printf("The floppy disk in drive A: is write protected, FDFS currently can not");nl();printf("be used on write protected floppies");}



}



*/


byte fdfs_mount(byte drive){
	


}




