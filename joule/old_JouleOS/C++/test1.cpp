//See COPYRIGHT.txt for info on copyright
#include <stdio.h>
#include <k_JouleOS.h>
char test495r[494]="Hello!";

extern "C"{
extern volatile unsigned long timer_ticks;
void test_cpp(){
	char* tmp=(char*)"hi!";
	unsigned long tmp83=timer_ticks;
	printf("test_:");
	printf(tmp);
	unsigned long tmp2=timer_ticks;
	if(tmp2==5){printf("\n5!!!");

}
}
}

