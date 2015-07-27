//See COPYRIGHT.txt for info on copyright
/*check out pcfs.txt in the documentation folder for complete specs*/
//This is for PCFS(Portable Customizable FileSystem)
//What about the idea that there will be an "always there" file for accessing the first sector
#include <ramdisk.h>
#include <stdio.h>
#include <string.h>
#include <mytypes.h>
#include <driver_drives.h>


typedef struct{
     DRIVE_PARAMS params;
     unsigned char *ptr;
     unsigned int drive_id;

}RAMDISK;


RAMDISK ramdisks[32];
unsigned int ramdisk_i;


BOOL MountDrive_100(unsigned int given_drive_id,BOOL *partionable){
     *partionable=0;
}

unsigned int DriveID_i;
unsigned int RequestDriveID(){ //only temporary
     return DriveID_i++;
}






BOOL CreateRamDisk(unsigned int size,char *mount){
     ramdisks[ramdisk_i].ptr=malloc(size);
     if(ramdisks[ramdisk_i].ptr==NULL){return 0;} //couldn't allocate enough memory
     ramdisks[ramdisk_i].params.sector_size=512; //sector size is 512
     ramdisks[ramdisk_i].params.total_size=size;
     ramdisks[ramdisk_i].params.devid=100; //device id is 100
     ramdisks[ramdisk_i].params.ready=1; //ready for quick access, always
     ramdisks[ramdisk_i].params.seqentual=0; //random access
     ramdisks[ramdisk_i].params.has_format=0; //no special formatting
     ramdisks[ramdisk_i].params.readonly=0;







}















































/*
void WriteSector_100(RAMDISK_HANDLE *ramdisk,unsigned char *buffer,unsigned long long sector,unsigned long long num_sectors){
	unsigned int i,j;
	unsigned char *ptr;

	ptr=ramdisk->ptr;
	for(j=0;j<=num_sectors-1;j++){
		for(i=0;i<=(ramdisk->attribs->sector_size*8);i++){
			ptr[(j*(ramdisk->attribs->sector_size*8))+(sector*(ramdisk->attribs->sector_size*8))+i]=buffer[(j*(ramdisk->attribs->sector_size*8))+i];
               if(j+sector>=ramdisk->attribs->num_sectors){return 0;}
		}
	}
     return 1;
}

void ReadSector_100(RAMDISK_HANDLE *ramdisk,unsigned char *buffer,unsigned long long sector,unsigned long long num_sectors){
	unsigned int i,j;
	unsigned char *ptr;

	ptr=ramdisk->ptr;
	for(j=0;j<=num_sectors-1;j++){
		for(i=0;i<=(ramdisk->attribs->sector_size*8);i++){
			buffer[(j*(ramdisk->attribs->sector_size*8))+i]=ptr[(j*(ramdisk->attribs->sector_size*8))+(sector*(ramdisk->attribs->sector_size*8))+i];
               if(j+sector>=ramdisk->attribs->num_sectors){return 0;}
		}
	}
     return 1;
}


void FormatRamDisk(RAMDISK_INFO *ramdisk){
	char *ptr;char *ptr2;
	ptr=ramdisk->ptr;
	ptr=ptr+ramdisk->attribs.sector_size; //remeber that we start filesystem info on 2
	memcpy(ptr,&ramdisk->header,sizeof(PCFS_HEADER));
	ptr=ptr+sizeof(PCFS_HEADER);
	memcpy(ptr,&ramdisk->attribs,sizeof(RAMDISK_ATTRIBS));
	ptr=ptr+sizeof(RAMDISK_ATTRIBS);

	ptr=ramdisk->ptr; //reset ptr
	ptr2=ptr+(ramdisk->attribs.sector_size*(ramdisk->attribs.first_index-1)); //goto first index sector


	ptr=ptr+ramdisk->attribs.sector_size*2; //advances ptr to sector 3
     memset(ptr,0,(unsigned int)ptr2-(unsigned int)ptr); //clears sector tables
     (unsigned char)*ptr=1|2|4; //allocates first 3 sectors
     //do for index





}



unsigned long long FindFreeSector(RAMDISK_HANDLE *ramdisk){
	unsigned long long sector;unsigned int *ptr;unsigned int i;
	unsigned char which;
	ptr=ramdisk->ptr;
	ptr=ptr+(ramdisk->attribs->sector_size*2);
     while(ptr[i]==0xFFFFFFFF){
		if(i>=ramdisk->header->bsize/8){return 0;}
		i++;
	}

	which=find_0_bit(ptr[i]);

	ptr[i]=ptr[i]|pwr(2,which);
	return which+(i*32);
}



*/














