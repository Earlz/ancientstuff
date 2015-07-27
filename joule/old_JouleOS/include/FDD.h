//See COPYRIGHT.txt for info on copyright
#ifndef FDD_H
#define FDD_H

#include "mytypes.h"

//ports
#define STATA 0x3F0
#define STATB 0x3F1
#define DOR 0x3F2
#define MAIN_STAT 0x3F4
#define DATA_REG 0x3F5
#define DIR 0x3F7
#define CCR 0x3F7


//DOR stuff
#define FDD0 0
#define FDD1 1
#define FDD_RESET 4
#define FDD_DMA 8
#define FDD0_MOTOR_ON 16
#define FDD1_MOTOR_ON 32

//main status register
#define FDD0_MOVING 1
#define FDD1_MOVNG 2
#define FDD_BUSY 16
#define NO_DMA 32
#define DATA_IN_OR_OUT 64
#define FDD_DATA_READY 128


//ST0
#define FDD1_SELECTED 1
#define FDD0_SELECTED 0
#define FDD0_HEAD_NOT_ON 0
#define FDD1_HEAD_NOT_ON 4
#define FDD_NOT_READY 8
#define DRIVE_FAULT 16
#define SEEK_END  32
#define ABNORMAL_TERMINATION 64
#define INVALID_COMMAND 128
#define DRIVE_BECAME_NOT_READY 192

//ST1
#include <k_JouleOS.h>



unsigned char FDD_ReadyGet();
unsigned char FDD_ReadySend();
unsigned char FDD_GetByte();
unsigned char FDD_SendByte(unsigned char data);


extern volatile BOOL fdd_int_done;
extern volatile unsigned char fdd_int_error;
extern volatile unsigned char fdd_int_type; //0 is read sector, 1 is write sector
extern unsigned char *fdd_int_buffer;
extern unsigned int fdd_int_counter;

unsigned char RecalibrateDrive_01(unsigned char drive);
unsigned char _RecalibrateDrive_01(unsigned char drive);
void GetSectorID_01(unsigned char drive,unsigned char *sector,unsigned char *track,unsigned char *head,unsigned char *ST0,unsigned char *ST1, unsigned char *ST2);
unsigned char _WriteSector_01(unsigned char drive,unsigned char sector,unsigned char track,unsigned char head, unsigned char *buffer);
unsigned char WriteSector_01(unsigned char drive,unsigned char sector,unsigned char track,unsigned char head, unsigned char *buffer);
void FDD_Handler(regs *r);
unsigned char FDD_ResetController(unsigned char drive);
void FDD_MotorOff(drive);
void FDD_MotorOn(drive);
unsigned char _SeekTrack_01(unsigned char drive,unsigned char track);
unsigned char SeekTrack_01(unsigned char drive,unsigned char track);
unsigned char _ReadSector_01(unsigned char drive,unsigned char sector,unsigned char track,unsigned char head, unsigned char *buffer);
unsigned char ReadSector_01(unsigned char drive,unsigned char sector,unsigned char track,unsigned char head, unsigned char *buffer);

BOOL WaitFdc(BOOL sensei);











#endif






/*
/* datatypes *

/* drive geometry *
typedef struct DrvGeom {
   BYTE heads;
   BYTE tracks;
   BYTE spt;     /* sectors per track *
} DrvGeom;

/* drive geometries *
#define DG144_HEADS       2     /* heads per drive (1.44M) *
#define DG144_TRACKS     80     /* number of tracks (1.44M) *
#define DG144_SPT        18     /* sectors per track (1.44M) *
#define DG144_GAP3FMT  0x54     /* gap3 while formatting (1.44M) *
#define DG144_GAP3RW   0x1b     /* gap3 while reading/writing (1.44M) *

#define DG168_HEADS       2     /* heads per drive (1.68M) *
#define DG168_TRACKS     80     /* number of tracks (1.68M) *
#define DG168_SPT        21     /* sectors per track (1.68M) *
#define DG168_GAP3FMT  0x0c     /* gap3 while formatting (1.68M) *
#define DG168_GAP3RW   0x1c     /* gap3 while reading/writing (1.68M) *

/* IO ports */
#define FDC_DOR  (0x3f2)   /* Digital Output Register */
#define FDC_MSR  (0x3f4)   /* Main Status Register (input) */
#define FDC_DRS  (0x3f4)   /* Data Rate Select Register (output) */
#define FDC_DATA (0x3f5)   /* Data Register */
#define FDC_DIR  (0x3f7)   /* Digital Input Register (input) */
#define FDC_CCR  (0x3f7)   /* Configuration Control Register (output) */

/* command bytes (these are 765 commands + options such as MFM, etc) **/
#define CMD_SPECIFY (0x03)  /* specify drive timings */
#define CMD_WRITE   (0xc5)  /* write data (+ MT,MFM) */
#define CMD_READ    (0xe6)  /* read data (+ MT,MFM,SK) */
#define CMD_RECAL   (0x07)  /* recalibrate */
#define CMD_SENSEI  (0x08)  /* sense interrupt status */
#define CMD_FORMAT  (0x4d)  /* format track (+ MFM) */
#define CMD_SEEK    (0x0f)  /* seek track */
#define CMD_VERSION (0x10)  /* FDC version */


/* function prototypes *

void init_floppy();
/*void deinit(void); *

BOOL reset(void);
BOOL diskchange(void);
void motoron(void);
void motoroff(void);
BOOL recalibrate(void);
BOOL seek(int track);
BOOL log_disk(DrvGeom *g);
BOOL read_block(int block,BYTE *blockbuff,unsigned long nosectors);
BOOL write_block(int block,BYTE *blockbuff,unsigned long nosectors);
BOOL format_track(BYTE track,DrvGeom *g);

#endif /* FDD_H */