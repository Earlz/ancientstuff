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

#ifndef PCG_H
#define PCG_H

#include <stdint.h>

#define PCG_NUM_OF_ARCHS 8

#define PCG_ARCH_NONE 0
#define PCG_ARCH_I386 1
#define PCG_ARCH_AMD64 2
#define PCG_ARCH_I386_REALMODE 3
#define PCG_ARCH_ARM7 4
#define PCG_ARCH_ARM9 5
#define PCG_ARCH_Z80 6
#define PCG_ARCH_PPC 7

#define PCG_BASE_NONE 0
#define PCG_BASE_X86 1
#define PCG_BASE_ARM 2
#define PCG_BASE_Z80 3
#define PCG_BASE_PPC 4

typedef struct {
	int base_family;
	int bits;
	int ptr_size;
	int int_size;
	int long_size;
}pcg_arch_info;

typedef struct{
	void *mem; //The memory to write instructions to
	union{
		uint16_t base16;
		uint32_t base32;
		uint64_t base64;
	};
	/*don't have to worry about size differences because
	we can't store 64 bits of data using a 32bit host*/
	int self_running;
	union{
		uint16_t ip16;
		uint32_t ip32;
		uint64_t ip64;
	};
	int arch;
	int sz;
}pcg_state;

typedef struct{
	uint64_t address;
}pcg_if_data;

int pcg_init();

int pcg_assign_mem32_const32(pcg_state *s,uint32_t ptr,uint32_t c,void *mem,int sz);
int pcg_setup_function(pcg_state *s,void *mem,int sz);
int pcg_end_function(pcg_state *s,void *mem,int sz);


#endif

