#include <stdlib.h>
#include <stdio.h>
#include "_base.h"
//this is to make this OS independent



/*functions for everything is...
void *handle=LoadExLibrary(char *name)  --load an external library such as dll,elf or whatrever
void *address=GetExSymbol(char *name,void *ExLibrary) --Get the address of a symbol in an
external library; should return 0 if the symbol doesn't exist
int error=UnloadExLibrary(void *ExLibrary) --this unloads an external library; should return 0 for error







*/


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
#warning "WARNING!! no specific OS is specified so defaulting to UNIX"

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






















