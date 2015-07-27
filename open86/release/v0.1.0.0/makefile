#Open86 makefile made by ME!!!
#...

_OBJS=.objs/dynamic_loading.o .objs/instructions/bitwise.o .objs/instructions/mov.o .objs/instructions/ints.o .objs/instructions/sub_add.o .objs/instructions/undoc.o .objs/instructions/groups.o .objs/instructions/ports.o .objs/instructions/jmp.o .objs/helpers.o .objs/main.o .objs/instructions/push_pop.o .objs/instructions/ect.o .objs/panic.o .objs/config_files.o
_CFLAGS=-g -w -c
default:
	fasm tester.asm tester.bin
	gcc $(_CFLAGS) dynamic_loading.c -o .objs/dynamic_loading.o
	gcc $(_CFLAGS) instructions/mov.c -o .objs/instructions/mov.o
	gcc $(_CFLAGS) instructions/bitwise.c -o .objs/instructions/bitwise.o
	gcc $(_CFLAGS) helpers.c -o .objs/helpers.o
	gcc $(_CFLAGS) instructions/jmp.c -o .objs/instructions/jmp.o
	gcc $(_CFLAGS) instructions/push_pop.c -o .objs/instructions/push_pop.o
	gcc $(_CFLAGS) instructions/ect.c -o .objs/instructions/ect.o
	gcc $(_CFLAGS) instructions/groups.c -o .objs/instructions/groups.o
	gcc $(_CFLAGS) instructions/ports.c -o .objs/instructions/ports.o
	gcc $(_CFLAGS) instructions/undoc.c -o .objs/instructions/undoc.o
	gcc $(_CFLAGS) instructions/sub_add.c -o .objs/instructions/sub_add.o
	gcc $(_CFLAGS) instructions/ints.c -o .objs/instructions/ints.o
	gcc $(_CFLAGS) panic.c -o .objs/panic.o
	gcc $(_CFLAGS) config_files.c -o .objs/config_files.o
	gcc $(_CFLAGS) main.c -o .objs/main.o

	g++ -o Open86 $(_OBJS)

build_asm:
	nasm -f bin tester.asm

clean:
	rm $(_OBJS)
