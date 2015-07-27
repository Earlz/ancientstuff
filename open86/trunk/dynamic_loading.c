#include <stdlib.h>
#include <stdio.h>
#include "_base.h"
/**This file contains all of the device related things**/



/*functions for everything is...
void *handle=LoadExLibrary(char *name)  --load an external library such as dll,elf or whatrever
void *address=GetExSymbol(char *name,void *ExLibrary) --Get the address of a symbol in an
external library; should return 0 if the symbol doesn't exist
int error=UnloadExLibrary(void *ExLibrary) --this unloads an external library; should return 0 for error


*/

/**for right now it just Segfaults...**/


#ifdef WIN32 //this is the win32 version
#define GOOD_OS
#include <windows.h>

void *LoadExLibrary(char *name){
    void *handle;
    handle=LoadLibrary(name);
    if(handle==NULL){
        panic("Error during LoadLibrary\n");
    }
    return handle;
}
void *GetExSymbol(char *name,void *ExLibrary){
    void *address;
    address=GetProcAddress(ExLibrary,name);
    return address;
}
int UnloadExLibrary(void *handle){
    int error;
    error=FreeLibrary(handle);
    return error;
}

#endif

#ifdef __unix__ //For UNIX
#define GOOD_OS
#include <dlfcn.h>

void *LoadExLibrary(char *name){
    void *handle;
    handle = dlopen(name, RTLD_LAZY);
    if(!handle)
    {
       panic("Error during dlopen()\n");
       return 0;
    }
    return handle;
}
void *GetExSymbol(char *name,void *ExLibrary){
    void *address;
    address = dlsym(ExLibrary, name);
    return address;
}
int UnloadExLibrary(void *handle){
    int error;
    error = dlclose(handle);
    return error;
}
#endif


#ifndef GOOD_OS //if your OS is not supported
#warning "WARNING!! no specific OS is specified so defaulting to UNIX style"

#include <dlfcn.h>

void *LoadExLibrary(char *name){
    void *handle;
    handle = dlopen(name, RTLD_LAZY);
    if(!handle)
    {
       panic("Error during dlopen()\n");
       return 0;
    }
    return handle;
}
void *GetExSymbol(char *name,void *ExLibrary){
    void *address;
    address = dlsym(ExLibrary, name);
    return address;
}
int UnloadExLibrary(void *handle){
    int error;
    error = dlclose(handle);
    return error;
}


#endif




void **Devices;
unsigned int dev_i=0; //index for devices

volatile GlobalDevice *GlobStruct;

device_handles *first_device=NULL;
device_handles *last_device=NULL;

// NOTE (Jordan#1#): here is where I left off
void InitExLib(char *fname){ //this will init a device
    O86Init func;
    device_handles *new_handle;
    new_handle=malloc(sizeof(device_handles));
    new_handle->device_handle=LoadExLibrary(fname);
    func=GetExSymbol("Open86Init",new_handle->device_handle);
    if(func==NULL){panic("Failed to find the Init symbol\n");}
    if(first_device==NULL){ //if first_device isn't initialized
        first_device=new_handle;
        first_device->next=first_device; //loop it
        first_device->prev=first_device; //and loop again
        last_device=first_device;
    }else{
        last_device->next=new_handle;
        new_handle->prev=last_device;
        last_device=new_handle;
        last_device->next=first_device; //loop it back to first device
    }

    func(GlobStruct);
}




/***helper functions for external devices***/
/**The cpu must be locked when doing things like loading registers or memory from external devices**/
void LockCPU(){  //This will lock the cpu
	if(GlobStruct->cpu_lock!=NO_LOCK){
		while((volatile)GlobStruct->cpu_lock!=NO_LOCK){}
		//wait until the cpu lock is lifted(let the other device finish)
	}
	GlobStruct->cpu_lock=NEED_LOCK;
	while(GlobStruct->cpu_lock==NEED_LOCK){} //wait until the cpu finishes..
}

void UnlockCPU(){
	GlobStruct->cpu_lock=NO_LOCK;
}

char GotLock(){
	return GlobStruct->cpu_lock;
}

//note that these dev_ one's don't check ANYTHING
//also note that with all of these functions the cpu MUST be LOCKED(including those that aren't memory)
unsigned char dev_ReadMemory8(unsigned int offset){
	unsigned char *ptr;
	unsigned char tmp;
	//offset=offset&0xFFFFF; //make it a 20bit address space(will change eventually)
	ptr=core+offset;
	return *ptr;
}

unsigned short dev_ReadMemory16(unsigned int offset){
	unsigned short *ptr;
	unsigned short tmp;
	offset=offset&0xFFFFF;
	ptr=core+offset;
	return *ptr;
}

void dev_WriteMemory8(unsigned int offset,unsigned char write){
	unsigned char *ptr;
	offset=offset&0xFFFFF;
	ptr=core+offset;
	*ptr=write;
}

void dev_WriteMemory16(unsigned int offset,unsigned short write){
	unsigned short *ptr;
	offset=offset&0xFFFFF;
	ptr=core+offset;
	*ptr=write;
}


unsigned char ReadGreg8(unsigned char reg){
	unsigned char tmp;
	return *gregs8[reg];

}

unsigned short ReadGreg16(unsigned char reg){
	return gregs16[reg];
}

//note! writing registers should usually not be required, but are here just in case..
void WriteGreg8(unsigned char reg,unsigned char write){
	*gregs8[reg]=write;
}

void WriteGreg16(unsigned char reg,unsigned short write){
	gregs16[reg]=write;
}

void WriteSreg(unsigned char reg,unsigned short write){
	sregs[reg]=write;
}
unsigned short ReadSreg(unsigned char reg){
	return sregs[reg];
}


void WriteFlags(unsigned short write){
	flagreg=write;
}
unsigned short ReadFlags(){
	return flagreg;
}

void WriteIP(unsigned short write){
	ip=write;
}
unsigned short ReadIP(){
	return ip;
}



/**Messaging system**/

volatile unsigned int pending_messages=0; //This is how many messages are pending(only provided for speed really)


volatile Open86MessageList *latest_message;
volatile unsigned int doing_messages=0; //This is set when things are being done with messages
//it should be clear before messing with messages, then it should be set while doing things with them
Open86MessageList *first_message;
open86_message_event *MessageEvents; //This is allocated by malloc
//This is an array of function pointers sorted by the device type and is called when a message is pushed

void DoNothing(Open86Message msg){
    return;
}

void CoreMessageEvent(Open86Message *msg){
	printf("MSG:target=%i message=%i extra=%i return_info=0x%x extra_ptr=0x%x\n",msg->target,msg->message,msg->extra,msg->return_info,msg->extra_ptr);
    printf("nothing here..yet..\n");
    if(msg->message==KILL_OPEN86){
    	printf("\n\nShutting Down Open86\n\n");
    	panic("Open86 ended\n");
    }
}

void tkeyboard(Open86Message *msg){
	if(msg->message==KEY_DOWN){
		printf("scan: 0x%02x",msg->extra);
	}
}

//#define NUMBER_OF_DEVICES_CURRENTLY 7

void Open86InitMessageSystem(){ //This initializes the mesaging system
	unsigned int i;
    first_message=malloc(sizeof(Open86MessageList));
    if(first_message==NULL){printf("Can't allocate memory for messaging system!");exit(1);}
    latest_message=first_message;
    pending_messages=0;
    first_message->previous=NULL; //just in case someone stupid does something..
    first_message->message.target=FIRST_MESSAGE;
    first_message->message.extra=FIRST_MESSAGE;
    first_message->message.message=FIRST_MESSAGE;
    first_message->message.extra_ptr=FIRST_MESSAGE;
    first_message->message.CallBack=NULL;
    first_message->message.DoneCallBack=NULL;
    first_message->message.return_info=NULL;

    InitEventList();

}


SPEED_UP unsigned int Open86PutMessage(unsigned int target,unsigned int message, unsigned int extra,void *return_info, Open86MessageCallBack callback,void *extra_ptr, Open86MessageCallBack done_callback ){
    void *tmp;
    tmp=latest_message; //save this for later //this is stack, so it's safe for threads
    while(doing_messages!=0){} //wait for messaging to be idle
    doing_messages=1;
    pending_messages++;
    latest_message=malloc(sizeof(Open86MessageList));
    if(latest_message==NULL){return 1;}
    latest_message->message.target=target;
    latest_message->message.return_info=return_info;
    //latest_message->message.done=done_var;
    latest_message->message.CallBack=callback;
    latest_message->message.DoneCallBack=done_callback;
    latest_message->message.message=message;
    latest_message->message.extra=extra;
    latest_message->message.extra_ptr=extra_ptr;
    latest_message->previous=tmp;
    doing_messages=0;
}

//SPEED_UP unsigned int Open86SendAll(unsigned int message,unsigned int extra, void *return_info,Open86MessageCallBack callback, void *extra_ptr, Open86MessageCallBack done_callback){


SPEED_UP Open86Message Open86PopMessage(){
    Open86Message ret;void *tmp;
    if(latest_message==first_message){
        return first_message->message;
    }
    while(doing_messages!=0){}
    doing_messages=1;
    ret=latest_message->message;
    tmp=latest_message;
//    *latest_message->message.done=1;
    latest_message=latest_message->previous; //set latest to prvious
    free(tmp);
    doing_messages=0;
    return ret;
}

SPEED_UP Open86Message Open86PeekMessage(){
    Open86Message ret;
    if(latest_message==first_message){
        return first_message->message;
    }
    //because we only read the message, we don't need to lock the message system
    ret=latest_message->message;

    return ret;
}


inline void CheckMessages_dep(){ //NOTE! This should only be called by the Process function!(thread)
    unsigned char tmp;
    volatile Open86Message use_this;
    //Open86MessageCallBack callback;
    /**Note that inside the message event, you shouldn't lock the cpu, as this will cause an infinite loop**/
    if(pending_messages==0){return;}
    while((volatile)pending_messages!=0){ //while were at it, just clear ALL messages...
		use_this=Open86PopMessage();
		if(MessageEvents[use_this.target]!=NULL){ //if the device is marked as installed
			MessageEvents[use_this.target](&use_this);
		}
		if(use_this.CallBack!=NULL){
			use_this.CallBack(&use_this);
		}
		if(use_this.DoneCallBack!=NULL){
			use_this.DoneCallBack(&use_this);
		}
		pending_messages--;
    }

}

inline void CheckMessages(){ //NOTE! This should only be called by the Process function!(thread)
    unsigned char tmp;
    volatile Open86Message use_this;
    //Open86MessageCallBack callback;
    /**Note that inside the message event, you shouldn't lock the cpu, as this will cause an infinite loop**/
    if(pending_messages==0){return;}
    while((volatile)pending_messages!=0){ //while were at it, just clear ALL messages...
		use_this=Open86PopMessage();
		tmp=GetEventIndex(use_this.target);
		if(tmp!=0){ //if the device is marked as installed
			EventFuncList[tmp].func(&use_this);
		}
		if(use_this.CallBack!=NULL){
			use_this.CallBack(&use_this);
		}
		if(use_this.DoneCallBack!=NULL){
			use_this.DoneCallBack(&use_this);
		}
		pending_messages--;
    }

}



/*
void InstallMessageEvent(unsigned int target,open86_message_event func){
	while(doing_messages!=0){}
    doing_messages=1;
	MessageEvents[target]=func;
	doing_messages=0;
}

void UninstallMessageEvent(unsigned int target){
	while(doing_messages!=0){}
    doing_messages=1;
	MessageEvents[target]=NULL;
	doing_messages=0;
}

typedef struct{
	unsigned int target;
	open86_message_event *Event;
	void *next,*prev;
}Event_LinkedList;

Event_LinkedList *FirstEvent;
Event_LinkedList *LastEvent;

/*MessageEvents[FIRST_MESSAGE]=DoNothing;
    MessageEvents[OPEN86_CORE]=CoreMessageEvent;
    MessageEvents[KEYBOARD]=tkeyboard;*
*/

open86_event_func_list *EventFuncList;
unsigned int EventFuncList_size=0;
unsigned int EventFuncList_i=0;

void InitEventList(){
	EventFuncList=malloc(sizeof(open86_event_func_list)*3);
	EventFuncList_size=sizeof(open86_event_func_list)*3;
	EventFuncList_i=3;
	EventFuncList[0].target=FIRST_MESSAGE;
	EventFuncList[0].func=DoNothing;
	EventFuncList[1].target=OPEN86_CORE;
	EventFuncList[1].func=CoreMessageEvent;
    EventFuncList[2].target=KEYBOARD;
    EventFuncList[2].func=tkeyboard;
}

/**How we cache events is we put everything in a big array..
 caching is done using just a open86_message_event *array[]; but if the
 pointer in the array is NULL, then the next value will be an unsigned int(32bit) telling what
 target number is next

 the first value is what number to start at..
 **/




unsigned char InstallMessageEvent(unsigned int target,open86_message_event func){
	while(doing_messages!=0){}
    doing_messages=1;
	if(EventInstalled(target)==1){return;}
	EventFuncList=realloc(EventFuncList,EventFuncList_size+sizeof(open86_event_func_list));
	if(EventFuncList==NULL){panic("Couldn't allocate memory for function lists");}
	EventFuncList[EventFuncList_i].target=target;
	EventFuncList[EventFuncList_i].func=func;
	EventFuncList_i++;
	doing_messages=0;
}

void UninstallMessageEvent(unsigned int target){
	//no need for an if, it won't hurt anything...
	unsigned int tmp;
	tmp=GetEventIndex(target);
	EventFuncList[tmp].target=EMPTY_MESSAGE;
	EventFuncList[tmp].func=DoNothing;
}


unsigned int GetNumberOfEvents(){
	return EventFuncList_i;
}

unsigned int GetEventIndex(unsigned int target){
	unsigned int i;
	for(i=0;i<EventFuncList_i;i++){
		if(EventFuncList[i].target==target){
			return i;
		}
	}
	return 0;
}


unsigned char EventInstalled(unsigned int target){
	unsigned int i;
	for(i=0;i<EventFuncList_i;i++){
		if(EventFuncList[i].target==target){return 1;}
	}
	return 0;
}







/*


//ok now then.. do what the load file says..
void StartExLib(){
     char *str;
     unsigned int x;
     unsigned char *going;
     unsigned int i;
     FILE *load;
     load=fopen("ExLibs/Load.txt","r");
     //str=malloc(255+64+1); //max_name_size+max_dev_size+null
     printf("z");
     if(load==NULL){printf("Error opening Load.txt\n");return 1;}
     for(;;){
          going=fgets(str,load,255+64+1);
          printf("str: %s",str);
          //read through and replace #, nl, and = into null
          x=strlen(str);
          for(i=0;i<x;i++){
               switch(str[i]){
                    case '\0': //if it's null then exit this stuff
                    i=0xFFFFFFFF;
                    break;
                    case '\n': //it's a nl so we replace it with nul
                    str[i]='\0';
                    break;
                    case '#': //we replace this with null because it's the comment character
                    str[i]='\0';
                    break;
                    case '=':
                    str[i]='\0';
                    break;
               }
          }

          if(strlen(str)!=0){
               printf(str);
          }

     }
     free(str);
}




*/

















