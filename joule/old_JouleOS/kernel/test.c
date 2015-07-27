//See COPYRIGHT.txt for info on copyright
#include <ramdisk.h>
#include <debug.h>
#include <event_drive.h>
#include <video.h>
#include <k_JouleOS.h>
volatile char buf[32];
volatile unsigned char read_test_buffer[5];
unsigned char tmp_reading=0;
extern volatile THREAD_STRUCT *testblah;
PROCESS_STRUCT *blahtest;
void Testy(regs *r){
     printf("yay!!");
}


void testfunc(){
     printf("blah and 1/2");
     start_console("multitasking!");
}

void testfunc2();
volatile char teststack[512];


void TestComRead(unsigned char trash,unsigned int dat,void *trash3){
	read_test_buffer[4]=0;
	read_test_buffer[tmp_reading]=(unsigned char)dat;
	tmp_reading++;
}
/*all sorts of goodies in this function*/
void test(char *argp){
	char *tmp;char *tmp2;drive_geometry g;
	CONSOLE *blah;
	//RAMDISK_HANDLE ram;

	unsigned int x1,x2,y1,y2;
	unsigned int i;

	char tmpxy[32];
	unsigned char tmp_char[256];
	//video_info draw1;
	printf("Hello!\n");
	if(_strcmp(argp,"t1")==1){scrolldowna(1);}
	if(_strcmp(argp,"t2")==1){
		tmp2=AllocCore(1);
          if (tmp!=4095){printf("its max");}
		FreeCore(tmp2,1);
		tmp=AllocCore(1);
		if (tmp==tmp2){printf("Error!");}
		if (tmp!=4095){printf("its max");}
		__asm("mov %0,%%edi": "=m"(tmp)); //a debugging thing
		*tmp=43;
		for(;;){}

     }
     if (_strcmp(argp,"t7")==1){
     	tmp2=sbrk(50);
     	tmp=sbrk(60);
     	if (tmp2>0xF000 || tmp>0xF000){printf("error");}
     	if (tmp2==tmp){printf("Error");}
     }     if (_strcmp(argp,"t8")==1){}
     if (_strcmp(argp,"t9")){
     }
     if (_strcmp(argp,"t10")){
     }
     if (_strcmp(argp,"t11")){
     }
     if (_strcmp(argp,"t12")){
     	printf("type 1083: ");
     	gets(buf);
     	printf(buf);
     	if (str2num(buf)==255){printf("yay it works");}
     }
     if (_strcmp(argp,"t13")){
     	//if(fdd_seek(200,0)==0){printf("yay");}
     	//if(fdd_seek(20,0)==1){printf("yay2");}
     	//if(fdd_recalibrate(0)==1){printf("yay3");}
     	//tmp=AllocCore(1);
     	g.head=1;g.track=20;g.sector=1;
          tmp=0x0000;
          memset(tmp,0,512);
     	//if(fdd_write(g,0,&tmpbufxy)==0){printf("not work");}


     	//memcpy(cwin->vptr,tmp,512);

     }
     if(_strcmp(argp,"t14")){
          g.head=1;g.track=20;g.sector=1;
          tmp=0x0000;
          memset(tmpbufxy,0,512);
     	//if(fdd_read(g,0,tmpbufxy)==0){printf("not work");}
     	printf(tmpbufxy);
     	//NewTask(&testtask,30,tmp,200);
     	//InitTasks();
     }
     if(_strcmp(argp,"t15")){
     	gets(tmpxy);
     	printf(tmpxy);
     }
     if (_strcmp(argp,"t16")){
     	test_cpp();
     }
     if(_strcmp(argp,"t17")){
          SetEDX(0xFFFFFFFF);
          if (GetEDX()==0xFFFFFFFF){em_printf("works\n");}
          testing12345:
          Push(0);
          SetCS(0x8,0);
          stopints();
          HLT();
     }
     if(_strcmp(argp,"t18")){

     }
     if(_strcmp(argp,"t19")){
          AllocCore(20);
     }
     if(_strcmp(argp,"t20")){

     }
     if(_strcmp(argp,"t21")){
     	putc(0xB3);
     }
     if(_strcmp(argp,"t22")){
     	for(i=1;i<255;i++){
     		tmp_char[i]=i;
     	}
     	tmp_char[256]=0;
     	printf(&tmp_char[180]);
     	printf("\n");
     	printf(&tmp_char[150]);
     }
     if(_strcmp(argp,"t21")){
     	//
     }
     if(_strcmp(argp,"t22")){
     	for(i=1;i<255;i++){
     		tmp_char[i]=i;
     	}
     	tmp_char[256]=0;
     	printf(&tmp_char[180]);
     	printf("\n");
     	printf(&tmp_char[150]);
     }
     if(_strcmp(argp,"t23")){

     }
     if(_strcmp(argp,"line")){

     }
     if(_strcmp(argp,"t24")){

     }

     if(_strcmp(argp,"t25")){
     	tmp=malloc(13);
     	memmove(tmp,"hello world!",13);
     	tmp2=malloc(4);
     	memmove(tmp2,"\nHi",4);
     	printf(tmp);
     	printf(tmp2);
     	free(tmp);
     	free(tmp2);
     }
     if(_strcmp(argp,"t26")){
     	//if(ResetFloppy()==0){printf("Error resetting..\n");}


     }
     if(_strcmp(argp,"t27")){
     	assert(0==1);
     }
     if(_strcmp(argp,"t28")){
     	gdt_set_gate(15,0,0xFFFFF,0xFB,0xCF); //testing code segment
     	gdt_set_gate(16,0,0xFFFFF,0xF3,0xCF);
          __asm(".intel_syntax noprefix\n"
          //"mov ds,0x80\n"
          "push 0x70\n"
          "push esp\n"
          "pushfd\n"
          "push 0x78\n"
          "push tmp\n"
          "iret\n"
          "tmp:\n"
          "hlt\n"
          ".att_syntax\n");
     }
     if(_strcmp(argp,"t29")){

     }

     if(_strcmp(argp,"t30")){
     	tests[0]=&c_info;
     	tests[1]=&help;
     	tests[0](0);
     	tests[1](0);
     }

     if(_strcmp(argp,"t31")){
     	tmp=malloc(512);
     	printf("1_");
     	memcpy(tmp,"Hello This is an uber 1337 test so hopefully it works",60);
     	printf("2_");
     	//write_block(100,tmp,1);
     	printf("3_\n");
     	printf(tmp);
     	printf("\n");
     	tmp2=malloc(512);
     	//read_block(100,tmp2,1);
     	printf(tmp2);
     	printf("\n");
     	free(tmp);free(tmp2);
     }
     if(_strcmp(argp,"t32")){
     	tmp=malloc(512);
     	tmp2=malloc(512*20);
     	//ram=malloc(sizeof(RAMDISK_HANDLE));

     }

     if(_strcmp(argp,"t33")){ //write works correctly


     }
     if(_strcmp(argp,"t34")){
     	if(SendByteCom1('h')){printf("error");}
     }
     if(_strcmp(argp,"t35")){

     }
     if(_strcmp(argp,"t36")){

     	printf("press any key when you got this running on both pc's\n");
     	waitkey();
     	printf("sending\n");
          if(SendByteCom1('t')){printf("error");}
     	if(SendByteCom1('e')){printf("error");}
     	if(SendByteCom1('s')){printf("error");}
     	if(SendByteCom1('t')){printf("error");}
     	printf("done\n");
     	printf("when you have sent the data on the other side press a key\n");
     	waitkey();
     	printf("printing received\n");
     	printf(read_test_buffer);
     	printf("\n");
     }

     if(_strcmp(argp,"t37")){
     	FDD_MotorOn(0);
     	waitkey();
     	FDD_MotorOff(0);
     	waitkey();
     	FDD_MotorOn(1);
     	waitkey();
     	FDD_MotorOff(1);
     }
     if(_strcmp(argp,"t38")){
     	tmp=malloc(512);
     	memcpy(tmp,"Hello World",12);
     	if(!WriteSector_01(0,1,0,0,tmp)){printf("error\n");}
     }
     if(_strcmp(argp,"t39")){
     	wait(500);
          waitkey();
     }
     if(_strcmp(argp,"t40")){
     	FDD_ResetController(0);
     	if(RecalibrateDrive_01(0)==0){printf("error\n");}
     }
     if(_strcmp(argp,"t41")){
     	InitTSS();
     }
     if(_strcmp(argp,"t42")){
     	//InitTaskMan();
     }
     if(_strcmp(argp,"t43")){
     	tmp=0x2004;
     	*tmp=(int)0;
     	tmp=0x2008;
     	*tmp=(int)1;
     	__asm(".intel_syntax noprefix\n"
     	"int 0x51\n"
     	".att_syntax\n");
     }
     if(_strcmp(argp,"t44")){
     	tmp=malloc(512);
     	memcpy(tmp,"hello everyone!! \0\0",512);
          if(!WriteSector_01(0,1,0,0,tmp)){printf("error");}
     	free(tmp);

     }
     if(_strcmp(argp,"t45")){
     	tmp2=malloc(512);
     	tmp=malloc(512);
     	if(!ReadSector_01(0,1,0,0,tmp)){printf("error");}
     	printf(tmp);
     	free(tmp);
     }
     if(_strcmp(argp,"t46")){
     	test_cpp();
     }
     if(_strcmp(argp,"t47")){
     	tmp=0x2000;
     	*tmp=51;
     	__asm(".intel_syntax noprefix\n"
     	"int 0x51\n"
     	".att_syntax\n");
     }
     if(_strcmp(argp,"t48")){
          printf("1");
          CreateInterrupt(33,&kbd_handler,0,0x10,0x8,IRQM_INTERRUPT);
          __asm(".intel_syntax noprefix\n"
     	//"int 67\n"
     	".att_syntax\n");
     }
     if(_strcmp(argp,"t49")){
          print_hex(0xFFFFFFFF);
          printf("\n");
          print_hex(0x12345678);
          printf("\n");
          print_hex(0xF3F2FAF);
     }
     if(_strcmp(argp,"t50")){
          cpu();
     }
     if(_strcmp(argp,"t51")){
          printf("hello 0x%s\ntest featuring!%s\n and now we have 0x%x and then 0x%x\n",argp,"blah",0xFAFBFCFD,0xA);
     }
     if(_strcmp(argp,"t52")){
          print_decu(12345);
          printf("\n");
          print_decu(1234567890);
          printf("\n");
          print_decu(0);
     }
     if(_strcmp(argp,"t53")){
          printf("hi this has been brought to you by: %u and also %u",284,20485432);
     }
     if(_strcmp(argp,"t54")){
          print_decs(-1234456784);
          printf("\n");
          print_decs(343);
     }
     if(_strcmp(argp,"t55")){
         printf("blah! 0x%x and 0x%x\n",0x0A0B,0);
     }
     if(_strcmp(argp,"t56")){
        //no_draw=1; //disable drawing
        blah=CreateConsole(USER_CONSOLE,0,"TEST",0);
        CurrentConsole->color=(RED<<4)+BLUE;
        cls();
        SwitchConsole(blah);
        putc('h');putc('i');
        RefreshConsole();
        waitkey();
       // no_draw=0;
     }
     if(_strcmp(argp,"t57")){
          printf("width: %u height: %u",ProcessConsole->width,CurrentConsole->height);
          printf("\nsize: %x  ptr: %x",ProcessConsole->height*((ProcessConsole->width)*2),((ProcessConsole->width*2)*1));
     }
     if(_strcmp(argp,"t58")){
          printf("Hi here!");
          waitkey();
          SwitchConsole(CurrentConsole->next);
          printf("also here...");
          waitkey();
          SwitchConsole(CurrentConsole->next);
          printf("almost made a circle!..");
          waitkey();
          SwitchConsole(CurrentConsole->next);
          printf("\nw00t were back here!");
          waitkey();
          SwitchConsole(CurrentConsole->next);
          waitkey();
          SwitchConsole(CurrentConsole->next);
          waitkey();
          //SwitchConsole(CurrentConsole->next);
     }
     if(_strcmp(argp,"t59")){
          //SwitchConsole(CurrentConsole->next);
          //waitkey();
          blahtest=NewProcess("test","blah",3,3,&testfunc,0xFFFF,malloc(4096),4096);
          (volatile)testblah=blahtest->Threads;
          printf("testblah: ss: 0x%x esp:0x%x eip: 0x%x testfunc: 0x%x\n",testblah->ss,testblah->esp,testblah->eip,&testfunc);
          __asm(".intel_syntax noprefix\n"
          "int 0x60\n"
          ".att_syntax\n");
     }
     if(_strcmp(argp,"t60")){
         blahtest=NewProcess("test","blah",3,3,&testfunc2,4096,malloc(4096),4096);
          (volatile)testblah=blahtest->Threads;
          printf("testblah: ss: 0x%x esp:0x%x eip: 0x%x testfunc: 0x%x\n",testblah->ss,testblah->esp,testblah->eip,&testfunc);
          __asm(".intel_syntax noprefix\n"
          "int 0x60\n"
          ".att_syntax\n");
          printf("??");
     }
     if(_strcmp(argp,"t61")){
         InitTasking();
     }
     if(_strcmp(argp,"math1")){
         unsigned int x=0,y=0;
	//unsigned int i=0;
	while(x<0xFFFFFF){
	    while(y<0xFFFFFFF){
            if(((x*x)-(y*y))==275){i++;}
           // if(y%0x1000000==0){printf("checkpoint\n");}
            y++;
        }
        y=0;
        printf("x++ i=%i\n",i);
        x++;
	}
	x=0;
	printf("done: i=%i\n",i);
	return 0;
    }


}










