//See COPYRIGHT.txt for info on copyright
#include <fdd.h>
#include <k_JouleOS.h>
#include <mytypes.h>
#include <k_misc.h>
#include <debug.h>
#include <stdio.h>

#define BOCHS_COMPAT 1 //0 to do things how they should. 1 to do things so they work in bochs
unsigned char status[7];
unsigned char sr0;
unsigned char fdd_track;
BOOL fdd_dchange;
void FDD_MotorOn(drive){
	wait(100);
	if(drive==0){
		outportb(DOR,0x1C);
	}else{
		outportb(DOR,0x2D);
	}

	wait(500);
}

void FDD_MotorOff(drive){
	wait(100);
	if(drive==0){
		outportb(DOR,8);
	}else{
		outportb(DOR,9);
	}
	//wait(500);
}

unsigned char FDD_ResetController(unsigned char drive){
	unsigned int delay,i;
	//wait(250);
     outportb(DOR,0);
	//wait(250);
	//outportb(DOR,1); //shut off motors and reset both of them
	wait(1000); //wait for motor off
	outportb(CCR,0); //precomponsition enabled - 500kbs rate
	//wait(250);
	outportb(DOR,drive|0xC); //drive and dma enabled, controller enabled
	//wait(250);
	//for(i=0;i<4;i++){ //4 interrupts
	delay=timer_ticks+2000;
     while ((volatile)fdd_int_done<1){
     	if(delay<timer_ticks){printf("err");return 0;} //time-out
     }
	//
	//for(i=1;i<4;i++){ //4 dummy reads -sense interrupt
		FDD_SendByte(0x08);
		FDD_GetByte(); //trash it
		FDD_GetByte(); //and this one
	//
     FDD_SendByte(0x03);
     FDD_SendByte(0xDF);
     FDD_SendByte(0x02);

     return 1;
}

volatile BOOL fdd_int_done=0;
volatile unsigned char fdd_int_error=0;
volatile unsigned char fdd_int_type=0; //0 is read sector, 1 is write sector
unsigned char *fdd_int_buffer=1024;
unsigned int fdd_int_counter;

void FDD_Handler(regs *r){
	fdd_int_done=1;

}


unsigned char InitFDD(BOOL on_boot){ //1 will make it print messages
     unsigned char tmp;
     if(on_boot){
     	printf("FDD Controller:");}
     tmp=FDD_ResetController(0);
     if(tmp!=1){

          if(on_boot==1){
               ProcessConsole->curx=50;ProcessConsole->color=RED;printf("[ResetController Failed]");
               ProcessConsole->cury++;ProcessConsole->curx=0;ProcessConsole->color=0x0F;
          }
          return 0;
     }else{
          if(on_boot==1){
               ProcessConsole->curx=60;ProcessConsole->color=GREEN;printf("[OK]");
               ProcessConsole->cury++;ProcessConsole->curx=0;ProcessConsole->color=0x0F;
          }
     }
     printf(".");
    // tmp=RecalibrateDrive_01(0);
     if(tmp==0){printf("ERROR");}
     printf(".");
     //tmp=tmp&SeekTrack_01(0,1);
     if(tmp!=1){
          if(on_boot==1){printf("FDD0:");
               ProcessConsole->curx=50;ProcessConsole->color=RED;printf("[Recalibrate Failed]");
               ProcessConsole->cury++;ProcessConsole->curx=0;ProcessConsole->color=0x0F;
          }
          }else{
               if(on_boot==1){printf("FDD0:");
                    ProcessConsole->curx=60;ProcessConsole->color=GREEN;printf("[OK]");
                    ProcessConsole->cury++;ProcessConsole->curx=0;ProcessConsole->color=0x0F;
               }
          }
          //tmp=RecalibrateDrive_01(1);
          tmp=0;
     if(tmp!=1){
          if(on_boot==1){printf("FDD1:");
               ProcessConsole->curx=50;ProcessConsole->color=YELLOW;printf("[Recalibrate Disabled]");
               ProcessConsole->cury++;ProcessConsole->curx=0;ProcessConsole->color=0x0F;
          }
          }else{
               if(on_boot==1){printf("FDD1:");
                    ProcessConsole->curx=60;ProcessConsole->color=GREEN;printf("[OK]");
                    ProcessConsole->cury++;ProcessConsole->curx=0;ProcessConsole->color=0x0F;
               }
          }
          return 0;
     }

unsigned char WriteSector_01(unsigned char drive,unsigned char sector,unsigned char track,unsigned char head, unsigned char *buffer){ //used for error checking
     unsigned int i;
     unsigned char error;
     for(i=0;i<3;i++){
     	printf("recal: ");
     	FDD_ResetController(0);
     	error=RecalibrateDrive_01(drive);
     	if(error==0){FDD_MotorOff(0);return 0;}
     	if(!SeekTrack_01(0,track)){FDD_MotorOff(0);return 0;}
     	printf("Write: ");
     	error=_WriteSector_01(drive,sector,track,head,buffer);
     	if(error==1){return 1;}
     	//recalibrate
     }
     FDD_MotorOff(0);
     return error;
}


unsigned char _WriteSector_01(unsigned char drive,unsigned char sector,unsigned char track,unsigned char head, unsigned char *buffer){ //w00t working mostly
     unsigned char ST0,ST1,ST2;
     unsigned int delay;
     //add diskchange detection and recalibrate if changed
     //call this function from another function for error tolerance
     FDD_MotorOn(drive);
     outportb(FDC_CCR,0);
     if(!FDD_SendByte(0x45)){return 0;} //if 1 fails they all fail!
     if(!FDD_SendByte((drive&3)|((head&4)<<3))){return 0;}

     FDD_SendByte(track);
     FDD_SendByte(head);
     FDD_SendByte(sector);
     FDD_SendByte(0x02); //sector size
     FDD_SendByte(18); //track length
     FDD_SendByte(27); //GAP3 length -27
     fdd_int_done=0;
     fdd_int_buffer=1024; //must be in the lower 16mb so...
     memcpy(fdd_int_buffer,buffer,512);
     dma_xfer(2,fdd_int_buffer,512,1);
     if(!FDD_SendByte(0xFF)){return 0;} //Data length --ignored because of sector size
     //now transfer the data!
     //wait(1);
     if(!WaitFdc(FALSE)){printf("er");return 0;}
     printf("3");

     //error checking... blah blah blah
     //results phase:

     ST0=FDD_GetByte();
     if(ST0==0xFD){return 0;}
     if(ST0==0x80){return 0;}
     printf("5");
     ST1=FDD_GetByte();
     ST2=FDD_GetByte();
     printf("7");

     //all this sector id crap is ignored
     FDD_GetByte(); //track
     if(FDD_GetByte()==0xFD){return 0;} //head
     FDD_GetByte(); //sector number
     if(FDD_GetByte()==0xFD){return 0;} //sector size
     printf("6");
     //error checking.....
     //for now returns 0 if a drive not ready error or such
     //and returns 2 if write protected
     if((ST0&0xC0)!=0){return 0;} //interrupt code
     if((ST0&16)!=0){return 0;} //unit check
     if((ST0&8)!=0){return 0;} //drive not ready
     printf("7");
    // if((ST1&2)!=0){return 2;} //write protected -do not copy for read
     if((ST1&16)!=0){return 0;} //time-out
     if((ST1&32)!=0){return 0;} //data error
     printf("8");
     if((ST1&128)!=0){return 0;} //sector number exceeds track
     if((ST1&4)!=0){return 0;}
     //nothign in ST2
     printf("2");
     FDD_MotorOff(drive);
     return 1;
}



unsigned char ReadSector_01(unsigned char drive,unsigned char sector,unsigned char track,unsigned char head, unsigned char *buffer){ //used for error checking
     unsigned int i;
     unsigned char error;
     for(i=0;i<3;i++){
     	printf("Recal: ");
     	FDD_ResetController(0);
     	error=RecalibrateDrive_01(drive);
     	if(error==0){FDD_MotorOff(0);return 0;}
     	if(!SeekTrack_01(0,track)){FDD_MotorOff(0);return 0;}
     	printf("Read: ");
     	error=_ReadSector_01(drive,sector,track,head,buffer);
     	if(error==1){return 1;}
     	//recalibrate
     }
     FDD_MotorOff(0);
     return error;
}


unsigned char _ReadSector_01(unsigned char drive,unsigned char sector,unsigned char track,unsigned char head, unsigned char *buffer){
     unsigned char ST0,ST1,ST2;
     unsigned int delay;
     //add diskchange detection and recalibrate if changed
     //call this function from another function for error tolerance
     FDD_MotorOn(drive);
     outportb(FDC_CCR,0);
     if(!FDD_SendByte(0x46)){return 0;} //if 1 fails they all fail!
     if(!FDD_SendByte((drive&3)|((head&4)<<3))){return 0;}

     FDD_SendByte(track);
     FDD_SendByte(head);
     FDD_SendByte(sector);
     FDD_SendByte(0x02); //sector size
     FDD_SendByte(18); //track length
     FDD_SendByte(27); //GAP3 length -27
     fdd_int_done=0;
     fdd_int_buffer=1024; //must be in the lower 16mb so...
     memset(fdd_int_buffer,0,512);
     dma_xfer(2,fdd_int_buffer,512,0);
     if(!FDD_SendByte(0xFF)){return 0;} //Data length --ignored because of sector size
     //now transfer the data!
     //wait(1);
     if(!WaitFdc(FALSE)){printf("er");return 0;}
     printf("3");
     memcpy(buffer,fdd_int_buffer,512);
     //error checking... blah blah blah
     //results phase:

     ST0=FDD_GetByte();
     if(ST0==0xFD){return 0;}
     if(ST0==0x80){return 0;}
     printf("5");
     ST1=FDD_GetByte();
     ST2=FDD_GetByte();
     printf("7");

     //all this sector id crap is ignored
     FDD_GetByte(); //track
     if(FDD_GetByte()==0xFD){return 0;} //head
     FDD_GetByte(); //sector number
     if(FDD_GetByte()==0xFD){return 0;} //sector size
     printf("6");
     //error checking.....
     //for now returns 0 if a drive not ready error or such
     //and returns 2 if write protected
     if((ST0&0xC0)!=0){return 0;} //interrupt code
     if((ST0&16)!=0){return 0;} //unit check
     if((ST0&8)!=0){return 0;} //drive not ready
     printf("7");
    // if((ST1&2)!=0){return 2;} //write protected -do not copy for read
     if((ST1&16)!=0){return 0;} //time-out
     if((ST1&32)!=0){return 0;} //data error
     printf("8");
     if((ST1&128)!=0){return 0;} //sector number exceeds track
     if((ST1&4)!=0){return 0;}
     //nothign in ST2
     printf("2");
     FDD_MotorOff(drive);
     return 1;
}







unsigned char RecalibrateDrive_01(unsigned char drive){
	unsigned char i,error;
	for(i=0;i<3;i++){
		printf("1");
		error=_RecalibrateDrive_01(drive);
		if(error==1){return 1;}
	}
	return 0;
}



unsigned char _RecalibrateDrive_01(unsigned char drive){
	unsigned char sector,track,head,ST0,ST1,ST2;
	unsigned int delay;
	unsigned char i;
	FDD_MotorOn(drive);
	printf("2");
	fdd_int_done=0;
	if(!FDD_SendByte(7)){return 0;}
	if(!FDD_SendByte(0)){return 0;}
	printf("3");
	//if(!FDD_SendByte(0)){return 0;}
     i=WaitFdc(TRUE);
     FDD_MotorOff(drive);
     if(i==0){return 0;}
     return 1;
     //GetSectorID_01(drive,&sector,&track,&head,&ST0,&ST1,&ST2);
    // wait(250);
     //track=inportb(STATA);
    // track=(track&16);
   //  if(track==0){return 0;}else{return 1;}
} //no result phase






void GetSectorID_01(unsigned char drive,unsigned char *sector,unsigned char *track,unsigned char *head,unsigned char *ST0,unsigned char *ST1, unsigned char *ST2){
     unsigned int delay;
     fdd_int_done=0;
     FDD_MotorOn(drive);
     FDD_SendByte(0x4A);
	FDD_SendByte(drive);
	delay=timer_ticks+2000; //5second time out
     while ((volatile)fdd_int_done<1){
     	if(delay<timer_ticks){return 0;} //time-out
     }
     *ST0=FDD_GetByte();
     *ST1=FDD_GetByte();
     *ST2=FDD_GetByte();
     *track=FDD_GetByte();
     *head=FDD_GetByte();
     *sector=FDD_GetByte();
     FDD_GetByte(); //sector size not used

}

unsigned char SeekTrack_01(unsigned char drive,unsigned char track){
	unsigned int i;
	unsigned char error;
	for(i=1;i<4;i++){
		error=_SeekTrack_01(drive,track);
		if(error==1){return 1;}
	}
	return error;
}



unsigned char _SeekTrack_01(unsigned char drive,unsigned char track){
	unsigned char trash,temp_track;
	unsigned int delay;
     FDD_MotorOn(drive);
	FDD_SendByte(0xF);
	FDD_SendByte(drive);
	FDD_SendByte(track);
	if(WaitFdc(TRUE)==FALSE){
		return 0;
	}
	FDD_MotorOff(drive);
	return 1;
     if ((sr0 == 0x20) || (fdd_track != track)){
     return FALSE;
    }else{
     return TRUE;
    }
     //delay=timer_ticks+2000; //5second time out
     //while ((volatile)fdd_int_done<1){
     //	if(delay<timer_ticks){return 0;} //time-out
     //
     //GetSectorID_01(drive,&trash,&temp_track,&trash,&trash,&trash,&trash);
    // if(track!=temp_track){return 0;}
     return 1;
}





//Some uber nifty functions/macro type things

/*because I will recalibrate every wrte and read if it does not recalibrate
right then theres a problem with the fdd but if it times out in wait fdc it may just
be bochs
*/


BOOL WaitFdc(BOOL sensei){
	unsigned int delay;
	unsigned int i;
     (volatile)timer1=2000; //2 second timeout
     printf("2");
     while ((volatile)fdd_int_done==0){
     	if((volatile)timer1==0){printf("8");break;} //time-out
     }
     printf("3");
     if(sensei){
     while ((i <= 7) && (inportb(FDC_MSR) & (1<<4))) {
          status[i++] = FDD_GetByte();
     }
          FDD_SendByte(CMD_SENSEI);
           sr0 = FDD_GetByte();
           fdd_track = FDD_GetByte();
     }
     fdd_int_done=0;
     if((volatile)timer1!=0){
     	if (inportb(FDC_DIR) & 0x80){
     		fdd_dchange=1;
     		printf("6");
               return FALSE;
     	}else{
     		printf("4");
     		return TRUE;
     	}
     }else{
     printf("e");
     #ifdef BOCHS_COMPAT
          return 1;
     #else
          return 0;
     #endif
     }
}









unsigned char FDD_GetByte(){
	if(!FDD_ReadyGet()){return 0xFD;}
	return inportb(DATA_REG);
}

unsigned char FDD_SendByte(unsigned char data){
	unsigned char tmp;
     tmp=FDD_ReadySend();
	outportb(DATA_REG,data);
	return tmp;
}


unsigned char FDD_ReadyGet(){
	unsigned int delay;
	wait(40);
	delay=timer_ticks+1000; //2second timeout
	while(delay>timer_ticks){
          if((inportb(MAIN_STAT)&0xC0)==0xC0){return 1;}
	}
	return 0xFD; //FF means ignored so
}

unsigned char FDD_ReadySend(){
	unsigned int delay;
	wait(40);
	delay=timer_ticks+1000; //2second timeout
	while(delay>timer_ticks){
		if((inportb(MAIN_STAT)&0xC0)==0x80){return 1;}
	}
	return 0xFD;
}



/*
BOOL DriveNotReady(){
	if(inportb(STAT0)&8==0){
		return 0;
	}else{
		return 1;
	}
}
*/
/*
unsigned char CurrentFDD(){
	if(inportb(STAT0)&1==0){
		return 0;
	}else{
		return 1;
	}
}

BOOL FDD_ReadOnly(){
	if(inportb(STAT1)&2==0){
		return 0;
	}else{
		return 1;
	}
}

BOOL FDD_DoubleSided(){
	if(inportb(STAT3)&8==0){
		return 0;
	}else{
		return 1;
	}
}

unsigned char FDD_CurrentHead(){
	if(inportb(STAT3)&4==0){
		return 0;
	}else{
		return 1;
	}
}
*/
BOOL FDDChanged(){
	if(inportb(DIR)&128==0){
		return 0;
	}else{
		return 1;
	}
}

unsigned char FDDDrives(){
	if(inportb(STATA)&64==0){
		return 2;
	}else{
		return 1;
	}
}


































