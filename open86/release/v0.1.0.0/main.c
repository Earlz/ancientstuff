#include <stdio.h>
#include <stdlib.h>
#include "_base.h"

void *core; //this is where our memory begins


volatile unsigned short gregs16[8];
volatile unsigned char *gregs8[8]={&gregs16[AX],&gregs16[CX],
&gregs16[DX],&gregs16[BX], //All the lows
&gregs16[AX],&gregs16[CX],&gregs16[DX],&gregs16[BX]}; //all the highs are up 1 byte so they use the same memory
//I'm using this super messy initialization to be able to easily go *gregs8[AH] and it work
volatile unsigned short sregs[7]; //segment registers
volatile unsigned short flagreg=0;
volatile unsigned short ip;
volatile unsigned int eip; //this is only for our use and is with CS calculated
volatile unsigned char *iptr; //this is with all offsets included
volatile unsigned char no_advance=0; //this is set to 1 to disable advancing ONE time(useful for jmps and calls
volatile FLAGS *flags=&flagreg;
unsigned char reset_segment=1;
volatile opcode *Opcodes;
opcode *opcodes_16bit;
opcode *opcodes_disasm;
opcode *opcodes_debug;
unsigned char StackSegment=SS;
volatile unsigned char CurrentSegment=DS;
unsigned int cpu_level=X086;

char *open86_dir;
char *config_dir;
char *dev_dir;
char *tmp_filename;

void DoCPU(){
     for(;;){
         //CheckPIC();
         if(GlobStruct->cpu_lock==NEED_LOCK){
         	GlobStruct->cpu_lock=GOT_LOCK;
         	while((volatile)GlobStruct->cpu_lock==GOT_LOCK){
         	}
         	GlobStruct->cpu_lock=NO_LOCK;
         }
         if(CurrentSegment!=DS){StackSegment=CurrentSegment;}
         if(reset_segment!=1){CurrentSegment=DS;StackSegment=SS;}else{reset_segment--;}
          Opcodes[*iptr](); //straight simple and to the point..
          if(no_advance==0){
          ip++;
          doeip;doiptr;}else{no_advance=0;}

     }
}

inline void InstallOp(unsigned char num,void *func){
     Opcodes[num]=func;
}

void UninstallOp(unsigned char num){ //actually just sets the opcode to unknown
     Opcodes[num]=unknown;
}
//void unknown();
void InitOpcodes(){
     unsigned int i;
          Opcodes=opcodes_16bit; //set our opcode map to normal 16bit
          for(i=0;i<=0xFF;i++){
               InstallOp(i,unknown); //set all opcodes to unknown..
          }
          InstallOp(0xF4,hlt);
          InstallOp(0x90,nop);
          for(i=0xB0;i<0xB8;i++){ //mov al,ib for example
               InstallOp(i,mov_8greg_ib);
          }
          for(i=0xB8;i<=0xBF;i++){
               InstallOp(i,mov_16greg_iw);
          }
          InstallOp(0xE9,jmp_rel16);
          InstallOp(0xEB,jmp_rel8);
          InstallOp(0xEA, jmp_ptr16seg16);
          InstallOp(0x88,mov_rm8_greg8);
          for(i=0x50;i<=0x58;i++){
               InstallOp(i,push_r16);
          }
          for(i=0x58;i<=0x5F;i++){
               InstallOp(i,pop_r16);
          }
          InstallOp(0x68,push_imm16);
          InstallOp(0x6A,push_imm8);
          InstallOp(0xE8,call_rel16);
          InstallOp(0xC3,retn0);
          InstallOp(0x9A,call_far_ptr16);
          InstallOp(0x06,push_es);
          InstallOp(0x0E,push_cs);
          InstallOp(0x16,push_ss);
          InstallOp(0x1E,push_ds);
          if(cpu_level>=X286){
			InstallOp(0xFF,group2_ff);
          }
          InstallOp(0x2C,sub_al_ib);
          InstallOp(0x2D,sub_ax_iw);
          InstallOp(0x77,ja_rel8);
          InstallOp(0x70,jo_rel8);
          InstallOp(0x71,jno_rel8);
          InstallOp(0x72,jc_rel8);
          InstallOp(0x73,jnc_rel8);
          InstallOp(0x74,jz_rel8);
          InstallOp(0x75,jnz_rel8);
          InstallOp(0x77,jbe_rel8);
          InstallOp(0x78,js_rel8);
          InstallOp(0x79,jns_rel8);
          InstallOp(0x7A,jp_rel8);
          InstallOp(0x7B,jnp_rel8);
          InstallOp(0x7C,jl_rel8);
          InstallOp(0x7D,jge_rel8);
          InstallOp(0x7E,jle_rel8);
          InstallOp(0x7F,jg_rel8);
          InstallOp(0xD6,salc);
          InstallOp(0xF5,cmc);
          InstallOp(0xF8,clc);
          InstallOp(0xFC,cld);
          InstallOp(0xFA,cli);
          InstallOp(0xE6,out_imm8_al);
          InstallOp(0xE2,loop_rel8);
          InstallOp(0xE7,out_imm8_ax);
          InstallOp(0x8A,mov_greg8_rm8);
          InstallOp(0x8B,mov_greg16_rm16);
          InstallOp(0x8C,mov_rm16_sreg);
          InstallOp(0x8E,mov_sreg_rm16);
          InstallOp(0x89,mov_rm16_greg16);
          InstallOp(0xCD,int_normal);
          InstallOp(0xCC,int3);
          InstallOp(0xCE,into);
          InstallOp(0xCF,iret);
          InstallOp(0x04,add_al_ib);
          InstallOp(0x05,add_ax_iw);
          InstallOp(0x07,pop_es);
          InstallOp(0x17,pop_ss);
          InstallOp(0x1F,pop_ds);
          InstallOp(0xAC,lods_m8);




}

unsigned int EndMePort(unsigned int junk,unsigned char junk2,unsigned char junk3){
    exit(junk);
}


unsigned int PrintMePort(unsigned int chr,unsigned char trash,unsigned char size){
    char *buf[3];
    buf[1]=0;buf[2]=0;
    if(size==1){buf[0]=chr;}else{
        (unsigned short)buf[0]=(unsigned short)chr;
    }
    printf(buf);
}

unsigned int PrintMemory(unsigned int chr,unsigned char trash,unsigned char size){
    unsigned int *ptr;unsigned int i;
    switch(chr){
            case 0:
            ptr=core;
            for(i=0;i<=32;i++){
                printf("%i: %x\n",i,*ptr);
                ptr++;
            }
            break;
    }

}

void LoadComFile(char *ffile){
    FILE *blah;unsigned char *tmp;
    blah=fopen(ffile,"r");
	if(blah==NULL){printf("error opening %s\n",ffile);panic(" ");return 1;}
	printf("Loading com file: \"%s\" Loaded at: 0x0050:0100\n",ffile);
	sregs[CS]=0x50; //now at 0x500
	ip=0x100;
	doeip;doiptr;
	tmp=iptr;
	while(feof(blah)==0){
	    *tmp=fgetc(blah);
	    tmp++;
	}
    printf("File loaded!\n");
}

void LoadBinFile(char *ffile){
    FILE *blah;unsigned char *tmp;
    blah=fopen(ffile,"r");
	if(blah==NULL){printf("error opening %s\n",ffile);panic(" ");return 1;}
	printf("Loading bin file: \"%s\" Loaded at: 0x0050:0000\n",ffile);
	sregs[CS]=0x50; //now at 0x500
	ip=0x00;
	doeip;doiptr;
	tmp=iptr;
	while(feof(blah)==0){
	    *tmp=fgetc(blah);
	    tmp++;
	}
	printf("File loaded!\n");

}

void SetLogFile(char *ffile){
    FILE *tmp;
    void **tmp2;
    tmp=fopen(ffile,"w");
    if(tmp==NULL){panic("error creating log file!");return 1;}
    printf("using log file \"%s\"",ffile);
    *stdout=*tmp;
    printf("---Open86 Log File---\n");
}


void GetOpen86Dir(char *arg){
    char *end=arg+strlen(arg);
    while(arg<end){
        if(*end=='/' || *end=='\\'){
            end++; //save the '/'
            *end='\0';
            strcpy(open86_dir,arg);
            return;
            }
        end--;
    }
    strcpy(open86_dir,"");
    printf("Warning!! Cannot figure out open86's directory--using \'./\'");
}



void GetConfigDir(char *arg){
    char *end=arg+strlen(arg);
    while(arg<end){
        if(*end=='/' || *end=='\\'){
            end++; //save the '/'
            *end='\0';
            strcpy(config_dir,arg);
            return;
            }
        end--;
    }
    FILE *tmp;
    tmp=fopen(arg,"r");
    if(tmp!=NULL){
        config_dir[0]='/0';
        return;
    }
    panic("cannot figure out the config file directory!!\n");
}

void DoArgs(int argcount,char **arg){
    char *current;
    char *tmp;
    unsigned int i,j=1;
    current=arg[1];
    argcount--;
    //printf(current);
    //printf("\n%i\n",argcount);
    if(*arg[1]!='?'){ParseConfigFile(arg[1]);argcount--;j=2;}
    while(argcount!=0){
        tmp=current;
        if(tmp[0]!='?'){panic("Arg does not start with \'?\'");}
        for(i=0;i<strlen(current);i++){if(tmp[i]==':'){tmp[i]=0; break;}}
        if(strcmp(tmp,"?f")==0){printf("\nTTT\n");ParseConfigFile(tmp+strlen(tmp)+1);}  //debugging purposes...
        if(strcmp(tmp,"?L_com")==0){LoadComFile(tmp+strlen(tmp)+1);}  //load a .com file
        if(strcmp(tmp,"?log")==0){SetLogFile(tmp+strlen(tmp)+1);}  //use a log file
        if(strcmp(tmp,"?L_bin")==0){LoadBinFile(tmp+strlen(tmp)+1);} //load binary file
        if(strcmp(tmp,"?L_dev")==0){printf("bah");InitExLib(tmp+strlen(tmp)+1);} //load device
       // if(strcmp(tmp,"?bios")==0){LoadBios(tmp+strlen(tmp)+1);}
        j++;
        current=arg[j];
        argcount--;
        }


}


int main(int argc, char **argv){
    FILE *blah;unsigned char *tmp;
    //if(argc!=0){printf("no config info for me to follw\n");exit(-1);}
    if(argc==1){printf("Nothing to do\nArgs supported are: ?L_com:\"comfile.com\" --it will load a com file\n");
        printf("?L_bin:\"binfile.bin\" --it will load a bin file, the default \"tester\" file is in bin format\n");
        printf("?log:\"logfile.txt\"   --it will use this as the log file\n");
        exit(1);
    }

    printf("Starting Open86\n");
    opcodes_16bit=malloc(0xFF*sizeof(void *)); //trying to get ready for 64bit...lol
    opcodes_debug=malloc(0xFF*sizeof(void *));
    opcodes_disasm=malloc(0xFF*sizeof(void *));
    core=malloc(0x100000+0x1000); //alloc 1meg plus a bit for safe practice
    if(core==NULL){panic("Can't allocate enough memory for core");}
    gregs8[AH]++;
    gregs8[CH]++;
    gregs8[DH]++;
    gregs8[BH]++;
    memset(gregs16,0,2*8); //zero out the general registers
    memset(sregs,0,2*4); //zero segments
    sregs[CS]=0x20;
    ip=0;
    doeip;
    doiptr; //update those
    flagreg=0;
    printf("Cpu State Set!\n");
    tmp=iptr;
    open86_dir=malloc(512);
	config_dir=malloc(512);
	dev_dir=malloc(512);
    tmp_filename=malloc(512);

	 //now have the test code in memory
	//printf("now going to loader...\n");
	//StartExLib();
	//for(;;){}
	//if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
	//	panic("Error initializing SDL!!\n");
	//}
	Devices=malloc(sizeof(void *)*DEV_LIMIT);
	if(Devices==NULL){panic("Could not allocate enough memory for Devices!\n");}
    ports=malloc(sizeof(void *)*0xFFFF); //266k on 32bit machine
    if(ports==NULL){panic("Can't allocate enough memory for ports!\n");}
    memset(ports,0,sizeof(void *)*0xFFFF); //zero it out
    printf("Internal device structures setup!\n");
     InitOpcodes();
     printf("Opcodes initiated!\n");
     printf("setting up devices...\n");
     /**initialize GlobStruct!**/
     GlobStruct=malloc(sizeof(GlobalDevice));
     GlobStruct->_gregs16=gregs16;
     GlobStruct->_gregs8=gregs8;
     GlobStruct->_sregs=sregs;
     GlobStruct->_ip=&ip;
     GlobStruct->_eip=&eip;
     GlobStruct->_iptr=&iptr;
     GlobStruct->_flagreg=&flagreg;
     GlobStruct->_flags=&flags;
     GlobStruct->device_handles=Devices;
     GlobStruct->_dev_i=&dev_i;
     GlobStruct->_ports=ports;
     GlobStruct->_core=core;
     GlobStruct->cpu_lock=0;
     //CalculatePF8(0xAA);
     //if(flags->pf==1){printf("yay!");}
     //flags->cf=1;
     GetOpen86Dir(argv[0]);
	DoArgs(argc,argv);
    printf("Args and such parsed\n");
     ports[0xFF]=&EndMePort;
     ports[0xFA]=&PrintMePort;
     ports[0xFB]=&PrintMemory;
    printf("Devices initiated!\n");
    printf("Done initializing! Now executing instructions\n");
     DoCPU();

}








/*notes:
doing some testing and it seems as if the flags are set the same in
most every simple math instruction(like cmp)


Ok forget above...

--for add:--

the cf flag is set if the result is over the target size
for example add ah(255),bh(1) will set cf becausr the result should be 256

the of flag acts wierd.. it only does anything on signed numbers
for example -127+-12 will set OF and 127+12 sets it also... for some reason only
values of less than 127(signed) set it..
only way to distinguish I guess would be CF is only used for unsigned and OF is only used for signed

ZF is only set for unsigned overflow(along with cf) and it is set for signed
for the standard reasons like -12+12=0

PF is parity and is set if the result has even number of bits

SF is set if the top bit of the result is 1

AF is used just like CF exept it only checks for bit 3(carry or borrow)
not sure how you check for it since jcc doesn't use AF


--For sub/cmp:--

the cf is set for unsigned for somethign like 10-11=-1

the of flag is set for somethign like -127-1=-128 or 127--1=128

the zf flag is set if the result is zero, or the operands are the same(12-12=0)

PF is set if even number of set bits in result

SF is the top bit of the result

AF is used just like CF but only does it for bit 3
which would be like 14-15=-1 (unsigned)


--For And/Test:--
(this is also for xor, and or)

cf is always 0

of is always 0

af is left the same(I guess..)

pf is set for parity of result

zf is set if result is 0(or operands are the same)

sf is set for the sign(top bit of result)


--for mul:--

OF and CF are set to 0 if the top half of the result is 0, otherwise they are both set

other flags are undefined








--just need a notepad to think this out..--
checking memory...
for now we could do something like a
memory_limit=0xFFFFF -1 meg and
rom_start=0xC0000 -and above this is read-only









*/







