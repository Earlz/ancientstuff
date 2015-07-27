//See COPYRIGHT.txt for info on copyright
#define ADRW_16BIT 1
#define ADRW_32BIT 2
#define ADRW_64BIT 3

#define INDEX_USED 1
#define INDEX_DIR 2
#define INDEX_READ_ONLY 4
#define INDEX_SYSTEM 8
#define INDEX_HIDDEN 16
#define INDEX_ARCHIVE_READY 32
#define INDEX_MIRRORED 64


//using null terminated strings


typedef struct{
	char header[5]; //PCFS -null teminated
	char version[7]; //version of PCFS
	char name[16]; //mount point name
	//unsigned char name_size; //size of names in bytes
	unsigned char type_of_device; //what kindof device; 1 for floppy 2 for ramdisk
}PCFS_HEADER;

typedef struct{
	unsigned int num_sectors; //number of sectors
//	unsigned char address_width; --constant 4
	unsigned char sector_size; //sector size
//	unsigned int indexis; //number of indexis
     unsigned int first_index; //address
	unsigned int index_entries; //number of index entries
	unsigned int last_index; //address
}RAMDISK_ATTRIBS; //16bytes

typedef struct{
	char *filename;
	unsigned short sector;
	unsigned char mode;
	unsigned int create_timestamp;
	unsigned int edited_timestamp;
	unsigned long long size;
}PCFS_INDEX16;

typedef struct{
	char *filename;
	unsigned int sector;
	unsigned char mode;
	unsigned int create_timestamp;
	unsigned int edited_timestamp;
	unsigned int size;
}PCFS_INDEX32; //--what we use

typedef struct{
	char *filename;
	unsigned long long sector;
	unsigned char mode;
	unsigned int create_timestamp;
	unsigned int edited_timestamp;
	unsigned long long size;
}PCFS_INDEX64;