//See COPYRIGHT.txt for info on copyright
#ifndef DRIVER_DRIVES_H
#define DRIVER_DRIVES_H
//this file is for making a drivers for drives
#include <mytypes.h>

typedef struct{
     unsigned int sector_size; //this is sector size
     unsigned int total_size; //total size of volume in bytes
     unsigned int devid; //this is returned because in the filesystem driver it doesn't do everything by symbols
     unsigned int ready; //this really means ready right now-- like if you read somethign it won't have to recalibrate and stuff
     BOOL seqentual; //if seqentual is 1 then it is a seqentual access device if 0 then it is a random access device
     BOOL has_format; //this should be 1 if the device has a special way to format; otherwise it just uses write sector
     BOOL readonly;
}DRIVE_PARAMS;

/*standard drive format is ..
you must have 2 functions in the form of BOOL ReadSector_DEVID(uint drive_id,  void *buffer, uint sector)
and BOOL WriteSector_DEVID(uint drive_id, void *buffer, uint sector)
also a function to fill the paremeter struct like this GetParam_DEVID(uint drive_id)
a mount function should also be included.. it is in
the format of  BOOL MountDrive_DEVID(uint given_drive_id, BOOL *partitioned) --partitioned should be a pointer and filled out
and given_drive_id is the drive_id the os gives the device

*/

typedef BOOL (*READ_SECTOR)(unsigned int,void *,unsigned int);
typedef BOOL (*WRITE_SECTOR)(unsigned int,void *,unsigned int);
typedef BOOL (*MOUNT_DRIVE)(unsigned int, BOOL *);
typedef DRIVE_PARAMS (*GET_PARAM)(unsigned int);


typedef struct{
     READ_SECTOR ReadSector;
     WRITE_SECTOR WriteSector;
     MOUNT_DRIVE MountDrive;
     GET_PARAM GetParam;
}DRIVE_CONTROL;










#endif