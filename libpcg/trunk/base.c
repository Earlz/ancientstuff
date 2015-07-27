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

#include "pcg.h"




pcg_arch_info pcg_arch_list[8];
int pcg_init(){
	pcg_arch_info *a;
	a=&pcg_arch_list[0];
	a->base_family=PCG_BASE_NONE;
	a->bits=0;
	a->ptr_size=0;
	a->int_size=0;
	a->long_size=0;

	a=&pcg_arch_list[PCG_ARCH_I386];
	a->base_family=PCG_BASE_X86;
	a->bits=32;
	a->ptr_size=32;
	a->int_size=32;
	a->long_size=64;

	return 0;
}

int pcg_new_state(pcg_state *s,int arch,void *mem,int sz,uint64_t base,int self_running){
	if(arch>PCG_NUM_OF_ARCHS){
		return -1;
	}
	s->arch=arch;
	s->mem=mem;
	s->sz=sz;
	switch(pcg_arch_list[arch].ptr_size){
		case 16:
			s->base16=(uint16_t)base;
			s->ip16=0;
		break;
		case 32:
			s->base32=(uint32_t)base;
			s->ip32=0;
		break;
		case 64:
			s->base64=base;
			s->ip64=0;
		break;
		default:
			return -1;
		break;
	}
	s->self_running=self_running;
	
	return 0;
}
/*
int pcg_set_arch(int arch){
	if(arch>8){
		return 1;
	}
	pcg_arch=pcg_arch_list[arch];
	pcg_arch_num=arch;
	return 0;
}
*/


//void assign_mem32_const(void *
int pcg_assign_mem32_const32(pcg_state *s,uint32_t ptr,uint32_t c,void *mem,int sz){
	char *m=s->mem; //makes things hella easier
	switch(s->arch){
		case PCG_ARCH_I386:
			if(s->sz<10){return -2;}
			m[s->ip32+0]=0xB8; //op for mov eax,imm32
			*(uint32_t*)&m[s->ip32+1]=c; //the imm32
			m[s->ip32+5]=0xA3; //op for mov offset16/32,eAX
			*(uint32_t*)&m[s->ip32+6]=ptr;
			s->ip32+=10;
			return 0;
		break;
		default:
			return -1;
		break;
	}
	return -1;
}

int pcg_setup_function(pcg_state *s,void *mem,int sz){
	char *m=s->mem;
	switch(s->arch){
		case PCG_ARCH_I386:
			return 0;
		break;
		default:
			return -1;
		break;
	}
	return -1;
}

int pcg_end_function(pcg_state *s,void *mem,int sz){
	char *m=s->mem;
	switch(s->arch){
		case PCG_ARCH_I386:
			if(s->sz<1){
				return -2;
			}
			m[s->ip32+0]=0xC3; //ret
			s->ip32++;
			return 0;
		break;
		default:
			return -1;
		break;
	}
	return -1;
}

/*Strategy:
for an if, in data, we put the address that we must "fix"
and fill the jump_label with 0s.
Then, when we get to pcg_endif we pass that if_data to it
and then at the location of the endif, we will now have the
location to jump to for 'if not'.
For some archs. this may be a pointless approach, but for
x86, it's required.
*/



int pcg_if_mem32_mem32(uint32_t lptr,uint32_t rptr,int condition,
								pcg_if_data* data,void* mem,int sz)
{
	

}





