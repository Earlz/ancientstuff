/*
<Copyright Header>
Copyright (c) 2007 - 2009 Jordan "Earlz/hckr83" Earls
<http://www.Earlz.biz.tm>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.
   
THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED 
WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

PROJECT: libpcg
WWW: http://earlz.biz.tm
</Copyright Header>
*/


#include <stdio.h>
#include <stdlib.h>
#include <pcg.h>


#define NEED_MMAP_PROTECTION



#ifdef NEED_MMAP_PROTECTION
#include <sys/mman.h>
#endif
volatile int x;
void (*callme)(void);
char *debug;

int main(int argc,char **argv){
	char *mem;
	char *mem_old;
	
	x=0xF00DBA4;
	
#ifdef NEED_MMAP_PROTECTION
	mem=mmap(0,512,PROT_EXEC|PROT_WRITE|PROT_READ,MAP_ANON,-1,0);
#else
	mem=malloc(4096);
#endif
	/*
	mem_old=mem;
	
	debug=mem;
	printf("x=%x\n",x);
	printf("pcg_init\n");
	pcg_init();
	printf("pcg_set_arch\n");
	pcg_set_arch(ARCH_I386);
	printf("setup\n");
	int a;int sz=512;
	a=pcg_setup_function(mem,sz);
	if(a<0){printf("a<0 1\n");return -1;}
	mem+=a;
	sz-=a;
	printf("assign\n");
	a=pcg_assign_mem32_const32((uint32_t)&x,0xBADBABE,mem,sz);
	if(a<0){printf("a<0 2\n");return -1;}
	mem+=a;
	sz-=a;
	a=pcg_end_function(mem,sz);
	if(a<0){printf("a<0 3\n");return -1;}
	mem=mem_old;
	printf("mem=%x\n",mem);
	callme=(void*)mem;
	printf("callme\n");
	callme();
	printf("Done! x=%x\n",x);
	*/
	return 0;
}

