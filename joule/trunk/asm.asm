SECTION .text
use32
GLOBAL _asm_start
EXTERN _kstack
extern _testy
GLOBAL _kernel_entry
extern __main
_kernel_entry:
cli
mov eax,0x100008
jmp eax
jmp _asm_start

_asm_start:

;call __main
;jmp 0x100008
;call _testy
mov esp,_kstack+4096
;set the stack right
mov eax,CR0
and eax,0x7FFFFFFF
mov CR0,eax
mov eax,0
mov CR3,eax

;hlt
push __main
ret
hlt
;These variables are filled by LD!
_code:dd 0
_bss:dd 0
_end:dd 0
_start:dd 0


extern _gp
global _flush_segments
_flush_segments
lgdt [_gp]
mov ax,8
mov ds,ax
mov es,ax
mov gs,ax
mov fs,ax
mov ss,ax

jmp 0x10:flush_it
nop
flush_it:
nop
ret

;far ret template, this will not work as is!
;this is so that you can make normal C functions.
far_template:
mov ebp,esp
add ebp,12 ;skip the pushed ip and cs and then it's where we pushed esp
mov eax,[ss:ebp]
push eax ;push the stack pointer again..
mov eax,0x20000000 ;the value is at offset 0xE
call eax ;this is templated! 0x200 should be what the interrupt handler is ;this is templated! 0x200 should be what the interrupt handler is
pop eax
retf ;<--end total size of far_template is 22 bytes


int_template:
cli
push eax
mov al,0 ;this is templated! this should be 0 for most interrupts, or 1 if a value is already pushed.. -at 0x3
cmp al,1
jz no_pushing
jmp pushing
no_pushing:
jmp continue_1
pushing:
push 0 ;this is for a dummy error code..
continue_1:
pop eax
push 20 ;this is templated! 20 should be replaced with the interrupt number -at 0x19
pusha
push ds
push es
push fs
push gs
mov ecx,0
mov cx,ss
mov edx,esp ;store them for a bit..
mov ax,0x8 ;this is templated! 0x8 should be replaced with interrupt stack segment -0x30
mov ss,ax
mov esp,0x100 ;this is templated! 0x100 should be replaced with interrupt stack offset -at 0x35
push ecx ;push our saved ss
push edx ;push our saved esp
mov ax,0x08 ;this is templated! 0x08 should be replaced with interrupt data segment -at 0x3D
mov ds,ax ;setup data segments..
mov fs,ax
mov gs,ax

mov ax,cx
add edx,21*4 ;increment stack ptr to the first part of our interrupt..
mov es,ax ;set our es to the old ss
mov ecx,21
copy_loop: ;copy the data in the application stack to our interrupt stack
mov ebx,edx
mov esi,[es:ebx]
push esi
sub edx,4
loop copy_loop

ending_:
mov ax,ds
mov es,ax
mov eax,esp
push eax ;push the stack pointer so we can use it as an argument in our called function
;mov ebx,0x20 ;This is templated! 0x20 should be replaced with interrupt code offset
;crap...we have to have a far ret..hmmm
;only good solution I can think of is you have to tell a spot of memory in the segment(address space) that we can load a far ret capable thing..
;and just have to write something like push esp+8(I think)\ncall int_handler\n retf\n ..or something to deal with that call..
call 0x10:0x14000 ;This is templated! 0x10 should be replaced with interrupt code segment and 0x200 should be replaced with the farcall stub
;offset is at 0x6C, segment at 0x70
pop eax ;trash from the argument
pop edx ;our earlier pushed esp
pop esi ;our earlier pushed SS
mov ss,si
mov edx,esp
pop gs
pop fs
pop es
pop ds
mov al,0 ;this is templated! this should be 0(or anything) for normal interrupt, 1 for master irq, 2 for slave irq -at 0x80
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

hlt ;<--End template size is 160 bytes






