#<Copyright Header>
#Copyright (c) 2007 - 2009 Jordan "Earlz/hckr83" Earls  <http://www.Earlz.biz.tm>
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions
#are met:
#
#1. Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#2. Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#3. The name of the author may not be used to endorse or promote products
#   derived from this software without specific prior written permission.
#
#THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
#INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
#AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
#THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#This make file was based directly off of the original BSD 4.3 'as' sourcecode for 'makefile'
#</Copyright Header>

HDRS = internal.h pcg.h

OBJS = base.o
TEST_OBJS = testpcg.o 

		

#AS = /bin/as
#LD = /bin/ld

DFLAGS= -DDEBUG
CFLAGS = -g -I ./ -Wall -O ${DFLAGS}

LDFLAGS = -L ./ -lpcg

default: libpcg.a libpcg.so.0.1 testpcg_static

libpcg.a:	${OBJS}
	${AR} crs libpcg.a $(OBJS)

libpcg.so.0.1: ${OBJS}
	${CC} -shared $(OBJS) -o libpcg.so.0.1


testpcg: ${TEST_OBJS}
	${CC} ${LDFLAGS} ${TEST_OBJS} -o testpcg

testpcg_static: ${TEST_OBJS} libpcg.a
	${CC} -static ${TEST_OBJS} ${LDFLAGS} -o testpcg_static 


.c.o: ${HDRS}
	${CC} ${CFLAGS} -c $*.c

test: libpcg.so.0.1
	export LD_LIBRARY_PATH=./ && ./testpcg

clean:
	rm -f ${OBJS} core a.out errs ${TEST_OBJS}

install:
	install -s libpcg.a ${PREFIX}/lib




