//this file has everything you'll need for making a device for open86

#ifndef OPEN86_DEVICE_H
#define OPEN86_DEVICE_H

#ifndef _BASE_H

//16 bit register macros
#define AX 0
#define CX 1
#define DX 2
#define BX 3
#define SP 4
#define BP 5
#define SI 6
#define DI 7

//8 bit register macros
#define AL 0
#define CL 1
#define DL 2
#define BL 3
#define AH 4
#define CH 5
#define DH 6
#define BH 7

//segment registers
#define ES 0
#define CS 1
#define SS 2
#define DS 3
//#define FS 4   //only 8086 stuff!!
//#define GS 5


//extra macros
#define doeip eip=(ip+(sregs[1]<<4)) //this will update eip
#define doiptr iptr=(eip+core) //this will update iptr from eip

typedef struct{
    unsigned char cf:1;
    unsigned char r0:1;
    unsigned char pf:1;
    unsigned char r1:1;
    unsigned char af:1;
    unsigned char r2:1;
    unsigned char zf:1;
    unsigned char sf:1;
    unsigned char tf:1;
    unsigned char _if:1;
    unsigned char df:1;
    unsigned char of:1;
    unsigned char iopl:2; //not yet used
    unsigned char nt:1;
    unsigned char r3:1;
}__attribute__((packed))FLAGS; //this is a better representation of flags

typedef void (*opcode)(void);

typedef unsigned int(*Port)(unsigned int,unsigned char,unsigned char); //in=Port(out,in_or_out,size_of_data) --size of data is in number of bytes
//anything smaller than a 32bit value should be zero extended and in_or_out is 1 for 1 and 0 for out
extern volatile Port **ports;

#endif


typedef void (*Open86MessageCallBack)(void *open86_message);
typedef struct{
    unsigned int target; //This is what the message should be sent to(use the predefined macros!)
    unsigned int message; //This is the code of the actual message
    unsigned int extra; //This is extra data for use in the message(like an argument kindof)
    void *return_info; //This is provided so that the message can "return" things
    void *extra_ptr; //This is for extra data that won't fit in an int so should be pointed to
    Open86MessageCallBack CallBack; //This is called after the message is delivered
    Open86MessageCallBack DoneCallBack; //This is called after the above is called(useful for free())
    //when possible, extra should be used
}
    __attribute__((packed)) Open86Message;

typedef struct{
    //unsigned int msg_number; //This is the number of the message in the queue(0 for the last message)
    Open86Message message;
    void *previous; //We only need previous because of the way this works
}Open86MessageList; //no packing needed for this..

typedef void (*open86_message_event)(Open86Message *msg);



/**Messaging target defines**/
#define FIRST_MESSAGE 0 //This means to not do anything with it, if we free this message
                        //then it will screw up things..
#define OPEN86_CORE 1 //this is the target for open86 core
#define VIDEO_CARD 2
#define KEYBOARD 3
#define MOUSE 4
#define GUI 5
#define PIC_DEVICE 6
#define KEYBOARD 7
#define FLOPPY_CONTROLLER 8
#define DMA_DEVICE 9



#define EMPTY_MESSAGE 0xFFFFFFFF

/**Messaging Message_code Defines**/
//For target:OPEN86_CORE (1)
#define LOCK_CPU 0
#define UNLOCK_CPU 1
#define IS_CPU_LOCKED 2
#define KILL_OPEN86 3


//For target:GUI (5)
#define CHANGE_RES 0
#define DISPLAY_GRAPHICS 1

//for target:PIC_DEVICE(6)
#define SEND_INTERRUPT 0


//for target KEYBOARD(7)
#define KEY_DOWN 0
#define KEY_UP 1



//for all targets:
#define GET_PRODUCT_NAME 0xFFFF0001 //A name of the product, this can be something like "Open86 CGA display"
#define DEVICE_EMULATED 0xFFFF0002 //Specific name of the device emulated, can be something like "CGA"
#define OPEN86_VERSION_USED 0xFFFF0003 //What version of Open86 used in the device
#define WHAT_DEVICE_SUPPORTS 0xFFFF0004 //Gives info about what a device supports(pausing, saving_state, debugging, ect)
#define OPEN86_KILL 0xFFFF00005 //This is the terminate signal, it means to end your device
//now, depending on what the device supports, these should be used and supported
#define OPEN86_PAUSE 0xFFFF0010 //The pause signal..
#define OPEN86_RESUME 0xFFFF0011 //Resume from being paused
#define OPEN86_SAVE_KILL 0xFFFF0012 //this means open86 saves it's state and kills the machine
#define OPEN86_RESTORE 0xFFFF0013 //This is where it resumes from being saved above

//For CPU_BIOS stuff..
//video_card
#define SET_VIDEO 0xFF00FF01 //This will setup the Video display and using the struct below as extra_ptr
typedef struct{
	unsigned int graphics; //1 for graphics, 0 for text
	unsigned int width; //width for graphics, columns for text
	unsigned int height; //height for graphics, rows for text
	unsigned int video_memory; //ptr to where video memory should be
	unsigned int color_pallete; //this varies a lot according to the other properties..
	unsigned int colored; //1 for colored, 0 for black and white
	unsigned int color_depth; //or shade depth, 0x3 text mode would have this set at 4 (bits)
	unsigned int

}OPEN86_VIDEO_INFO_STRUCT;



typedef struct{
    void *prev;
    void *device_handle;
    char *filename;
    void *next;
}device_handles;


#define NO_LOCK 0
#define NEED_LOCK 2
#define GOT_LOCK 1


//many things we have to use a pointer for but if it's an array then it's not like the array pointer is going to change so..

typedef struct{ //the monster struct for global Open86 info
    //version info
    char high_version,mid_version,low_version; //so we don't rely on fpu
    //cpu state
    volatile unsigned short *_gregs16; //an array...
    volatile unsigned char **_gregs8; //an array of pointers
    unsigned short *_sregs; //an array
    unsigned short *_ip;
    unsigned int *_eip;
    unsigned char **_iptr;
    unsigned short *_flagreg;
    FLAGS **_flags;
    //global device stuff
    void **device_handles; //these are handles to other devices/plugins -an array
    unsigned int *_dev_i; //this is the device index to show where the next device allocated goes
    //port stuff
    Port **_ports; //pointer to an array of functions which has ports in it..
    void *_core; //this is the pointer to the emulation memory
    //extra's
    unsigned char cpu_lock; //this is set to NO_LOCK if the cpu is running, it is set to NEED_LOCK if the cpu
							//is running but will be locked soon, and it should be GOT_LOCK if the cpu is stopped

	/****Helper functions for doing things******/
	void (*LockCPU)();
	void (*UnlockCPU)();
	unsigned char (*MemRead8)(unsigned int offset);

	void (*WriteGReg16)(unsigned char reg,unsigned short val);
	void (*WriteGReg8)(unsigned char reg, unsigned char val);
	unsigned short (*ReadGReg16)(unsigned char reg);
	unsigned char (*ReadGReg8)(unsigned char reg);
	void (*WriteSReg)(unsigned char segment,unsigned short val);
	unsigned short (*ReadSReg)(unsigned char segment);
	unsigned short (*ReadFlags)();
	void (*WriteFlags)(unsigned short val);
	void (*SendInt)(unsigned int number);


	void (*InstallMessageEvent)(unsigned int target,open86_message_event func);
	void (*PutMessage)(unsigned int target,unsigned int message, unsigned int extra,void *return_info, Open86MessageCallBack callback,void *extra_ptr, Open86MessageCallBack done_callback);
	void (*UninstallMessageEvent)(unsigned int target);

	unsigned char *DevicesDone;



}__attribute__((packed))GlobalDevice;





#endif















