#JouleOS portable makefile
#$lib_linker -r -s $static_output $link_objects
#nasm -f coff asm.asm
_OBJS=.objs/kernel/main.o .objs/kernel/gdt.o .objs/lib/extras.o .objs/kernel/mem_manager.o .objs/lib/bitmap_manip.o .objs/kernel/idt.o .objs/lib/strings.o .objs/lib/asm_in_c.o
_CFLAGS=-nostartfiles -nostdlib -ffreestanding -nostdinc -s -I./include -march=i486 -DDEBUG -DNDEBUG
_LFLAGS=-T link.ld
_FLOPPY_DRIVE=B:

default:
	nasm -f coff asm.asm
	gcc $(_CFLAGS) -c kernel/main.c -o .objs/kernel/main.o
	gcc $(_CFLAGS) -c kernel/gdt.c -o .objs/kernel/gdt.o
	gcc $(_CFLAGS) -c kernel/idt.c -o .objs/kernel/idt.o
	gcc $(_CFLAGS) -c lib/strings.c -o .objs/lib/strings.o
	gcc $(_CFLAGS) -c lib/extras.c -o .objs/lib/extras.o
	gcc $(_CFLAGS) -c kernel/mem_manager.c -o .objs/kernel/mem_manager.o
	gcc $(_CFLAGS) -c lib/bitmap_manip.c -o .objs/lib/bitmap_manip.o
	gcc $(_CFLAGS) -c lib/asm_in_c.c -o .objs/lib/asm_in_c.o
	ld $(_LFLAGS) -o jouleos.bin asm.o $(_OBJS)
	objcopy -O binary jouleos.bin

copy_to_floppy:
	cp jouleos.bin $(_FLOPPY_DRIVE)/kernel.bin
