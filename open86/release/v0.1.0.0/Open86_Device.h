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
    volatile unsigned char **_gregs8; //an array
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
	void (*WriteGReg16)(unsigned char reg,unsigned short val);
	void (*WriteGReg8)(unsigned char reg, unsigned char val);
	unsigned short (*ReadGReg16)(unsigned char reg);
	unsigned char (*ReadGReg8)(unsigned char reg);
	void (*WriteSReg)(unsigned char segment,unsigned short val);
	unsigned short (*ReadSReg)(unsigned char segment);
	unsigned short (*ReadFlags)();
	void (*WriteFlags)(unsigned short val);

    /****Standard Device Info Structs*******/
    struct{
        void *lib_handle;
        char *name_of_product;
        void (*OpenFddImage)(char *file_name);
        void (*ReadSector)(unsigned int sector_number,void *destination_buffer);
        char *args;
    }__attribute__((packed))std_FddController;
    struct{
        void *lib_handle;
        char *name_of_product;
        char *args;
    }__attribute__((packed))std_template;
    struct{
        void *lib_handle;
        char *name_of_product;
        void (*VideoRefresh)(void *DoneBuffer,unsigned int buffer_size);
        //This is called by std_Video whenever the gui should refresh the video display
        //the format given is pixels for even text modes, and std_Video should compensate for that
        //the gui can ALWAYS assume that the mode is graphical for things like GetaWidth

        char *args;
    }__attribute__((packed))std_Gui;


    struct{
        void *lib_handle;
        char *name_of_product;
        unsigned char (*SetResolution)(unsigned int width,unsigned int height,unsigned char bpp,unsigned char graphical); //graphical is 1 if it's a graphics mode, or 0 if it's text
        //returns 0 for success, else error
        void (*PutPoint)(unsigned int x,unsigned int y, unsigned int character_or_color);
        //character_or_color will be used as an unsigned short with the first byte being the character and the second being the color, if
        //it is in a text mode; if it's in a graphic mode then it is used as color
        unsigned int (*GetPoint)(unsigned int x,unsigned int y);
        //this returns the color or color:character at x,y...like the backwards of the above
        unsigned int (*GetaWidth)(void);
        unsigned int (*GetaHeight)(void);
        unsigned int (*GetaBpp)(void);
        //this block is for the gui, this is what it interprets text modes to
        unsigned int (*GetWidth)(void);
        unsigned int (*GetHeight)(void);
        unsigned int (*GetBpp)(void);
        //This block is for the actual emulation
        unsigned int (*GetGraphical)(void);
        char *args;
    }__attribute__((packed))std_Video;



}__attribute__((packed))GlobalDevice;





#endif















