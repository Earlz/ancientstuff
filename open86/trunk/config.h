#ifndef CONFIG_H
#define CONFIG_H
/********these defines change the behaviour of Open86..****/
//by memory alerts I mean for example how when someone writes to 0xb800 it will alert the video device of the write
//#define LOW_MEMORY_USAGE  //you define this if you want to use the least amount of memory possible in exchange for slower performance
#define DEV_LIMIT 256 //this will eventually go away..


//#define MAYBE_OPTIMIZATIONS //you can define this for optimizations that may or may not be faster
//the optimizations in helpers.c are NOT safe!!!

#define ALLOW_MULTIPLE_TARGETS //This should be used if you want to allow multiple devices to
//use the same target in the messaging system
#define USE_DEVICE_CACHING //This should be defined in most cases as it dramaitcally increases the speed
//This is automatically ignored if LOW_MEMORY_USAGE is defined
//Also, ALLOW_MULTIPLE_TARGETS can't be defined with this, if it is, then this caching is disabled

#define USE_CPU_BIOS //This means to use the CPU BIOS. This considerably increases the speed of the BIOS

/***end***/


#endif
