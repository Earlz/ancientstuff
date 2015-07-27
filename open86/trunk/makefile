#Open86 makefile made by ME!!!
#...

_OBJS=.objs/layer.o .objs/cpu_bios.o .objs/dynamic_loading.o .objs/instructions/inc_dec.o .objs/instructions/mul_div.o .objs/instructions/bitwise.o .objs/instructions/mov.o .objs/instructions/ints.o .objs/instructions/sub_add.o .objs/instructions/undoc.o .objs/instructions/groups.o .objs/instructions/ports.o .objs/instructions/jmp.o .objs/helpers.o .objs/main.o .objs/instructions/push_pop.o .objs/instructions/ect.o .objs/panic.o .objs/config_files.o .objs/instructions/load.o .objs/instructions/bcd.o
_CFLAGS=-g -w -c
default:
	fasm tester.asm tester.bin
	fasm bios.asm bios.bin
	gcc $(_CFLAGS) dynamic_loading.c -o .objs/dynamic_loading.o
	gcc $(_CFLAGS) cpu_bios.c -o .objs/cpu_bios.o
	gcc $(_CFLAGS) instructions/mov.c -o .objs/instructions/mov.o
	gcc $(_CFLAGS) instructions/inc_dec.c -o .objs/instructions/inc_dec.o
	gcc $(_CFLAGS) instructions/mul_div.c -o .objs/instructions/mul_div.o
	gcc $(_CFLAGS) instructions/bitwise.c -o .objs/instructions/bitwise.o
	gcc $(_CFLAGS) helpers.c -o .objs/helpers.o
	g++ -g -c layer.cpp -o .objs/layer.o
	gcc $(_CFLAGS) instructions/jmp.c -o .objs/instructions/jmp.o
	gcc $(_CFLAGS) instructions/push_pop.c -o .objs/instructions/push_pop.o
	gcc $(_CFLAGS) instructions/ect.c -o .objs/instructions/ect.o
	gcc $(_CFLAGS) instructions/groups.c -o .objs/instructions/groups.o
	gcc $(_CFLAGS) instructions/ports.c -o .objs/instructions/ports.o
	gcc $(_CFLAGS) instructions/undoc.c -o .objs/instructions/undoc.o
	gcc $(_CFLAGS) instructions/sub_add.c -o .objs/instructions/sub_add.o
	gcc $(_CFLAGS) instructions/ints.c -o .objs/instructions/ints.o
	gcc $(_CFLAGS) instructions/load.c -o .objs/instructions/load.o
	gcc $(_CFLAGS) instructions/bcd.c -o .objs/instructions/bcd.o
	gcc $(_CFLAGS) panic.c -o .objs/panic.o
	gcc $(_CFLAGS) config_files.c -o .objs/config_files.o
	gcc $(_CFLAGS) main.c -o .objs/main.o

	g++ -o Open86 $(_OBJS)

build_asm:
	nasm -f bin tester.asm

clean:
	rm $(_OBJS)
