unsigned char kstack[1024];


#include <k_JouleOS.h>

void putc_tmp(unsigned char chr){
    unsigned char *tmp;
    tmp=0xB8000;
    *tmp=chr;
    tmp++;
    *tmp=0x0F;
}

void testy(){
    putc_tmp('1');
}

void _main(){
    InstallGdt();
    GdtPtr=Gdt;

    InitMemMan();

    if(AllocCore(&physical_memory,1)==0){putc_tmp('h');for(;;){}}
    __asm("hlt");
    for(;;){}


}


/*****************
just a small notepad for me


basically what an idt stub should do is:
---
cli
push _segments
pusha
mov ds,kernel_segment
mov ax,ss
mov ebx,esp
mov ss,interrupt_segment
mov esp,interrupt_stack
push stack_saver_esp ;for our C function we call
;this has been setup of called before so..
pop ds
pop es
pop ss
pop gs
pop fs
popa
popf
call [our_handler_ptr]
pushf
pusha
push _segments ;instead of writing it out..
mov ss,stack_saver
mov esp,stack_saver
popa
pop _segments


method2:
dw segments 6 ;this stores the segments temporarily
dw tmp
dw tmp2
;transfer the int stuff

mov ax,0x10 ;this should the flat kernel segment(CS is also the flat kernel segment..)
mov dx,ds ;temporarily store ds
mov ds,ax
mov bx,segments
mov [bx],dx
mov dx,ss
mov ax,0x8 ;this is templated! 0x8 will be replaced with interrupt data segment
mov ss,ax
pusha

mov si,0x ;this is templated, 0x8 will be replaced with interrupt data segment
;now set the kernel data segments
mov ds,si
mov es,si
mov gs,si
mov fs,si
mov si,0x8 ;this is templated, 0x8 will be replaced with interrupt stack segment
mov ss,si
mov esp,0x100 ;this is templated, 0x100 will be replaced with interrupt stack offset




method 3:   <--This is the only method that should work..


template:
cli
push al
mov al,0 ;this is templated! this should be 0 for most interrupts, or 1 if a value is already pushed..
cmp al,1
jz no_pushing
jmp pushing:
no_pushing:
jmp continue_1
pushing:
push 0 ;this is for a dummy error code..
continue_1:
pop al
push 20 ;this is templated! 20 should be replaced with the interrupt number
pusha
push ds
push es
push fs
push gs
mov cx,ss
mov dx,esp ;store them for a bit..
mov ax,0x8 ;this is templated! 0x8 should be replaced with interrupt stack segment
mov ss,ax
mov esp,0x100 ;this is templated! 0x100 should be replaced with interrupt stack offset
push cx ;push our saved ss
push dx ;push our saved esp
mov ax,0x08 ;this is templated! 0x08 should be replaced with interrupt data segment
mov ds,ax ;setup data segments..
mov fs,ax
mov gs,ax

mov ax,cx
add dx,17*4 ;increment stack ptr to the first part of our interrupt..
mov es,ax ;set our es to the old ss
copy_loop: ;copy the data in the application stack to our interrupt stack
mov cx,17
mov esi,[es:dx]
push esi
sub dx,4
loop copy_loop

ending_:
mov ax,ds
mov es,ax
mov eax,esp
push eax ;push the stack pointer so we can use it as an argument in our called function
mov eax,0x20 ;This is templated! 0x20 should be replaced with interrupt code offset
call 0x10:eax ;This is templated! 0x10 should be replaced with interrupt code segment
pop eax ;trash from the argument
pop dx ;our earlier pushed esp
pop si ;our earlier pushed SS
mov ss,si
mov dx,esp
pop gs
pop fs
pop es
pop ds
mov al,0 ;this is templated! this should be 0(or anything) for normal interrupt, 1 for master irq, 2 for slave irq
cmp al,1
jz master_irq
cmp al,2
jz slave_irq
return_here:
popa
add esp,8 ;we do this cause we can't use a temp register basically it's just pop [temp] and pop [temp] again
iret

master_irq:
mov al,0x20
out 0x20, al
jmp return_here

slave_irq:
mov al,0x20
out 0xA0, al
out 0x20, al
jmp return_here

hlt ;<--End





***************/









