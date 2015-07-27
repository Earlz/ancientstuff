;SECTION .text
;[bits 16]
org 0

jmp jumping_test
mov ax,0xBAD
nop
hlt
mov al,1
mov ah,5
mov bh,2
mov dl,7
nop
mov ax,0
mov cx,0x1234
mov dx,0x4321
mov di,0x1221
mov bx,0x1234
nop
blah:
nop
;jmp 0x101:0xFeed
;jmp blah ;infinite loop
;hlt
mov ah,cl
nop
mov bx,1
mov di,2
mov dl,0x26
nop
;mov [bx+di+0xF12],dl
nop

mov bx,3
mov si,2
mov dl,0x26
;mov [bx+si],dl
mov [0xF12],dl
nop
mov bx,0xF10
mov si,1
nop
mov [si+bx+1],dl
nop


_bah:
nop
;jmp _bah
nop
mov sp,0x60
mov ax,0xFFF1
mov bx,0xFFF2
nop
push ax
push bx
nop
mov ax,0xFFF3
mov bx,0xFFF4
nop
pop bx
pop ax
nop


;push 0xF123
pop ax
nop
;push 0xF321
pop bp
nop


mov bx,0xF00

mov ax,0x4321
call _t1
loopy:
jmp loopy

_t2:
hlt


call _t1
hlt
mov ax,0x4FF4
nop
nop
_t1:
hlt
mov ax,0xF44F
nop
ret

hlt

mov ax,_blah
call ax


_blah:


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;second block of testing;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;push pop and call



_next:
jmp _third_block

call _t4
jmp l1
_t3:
mov bx,0x300
mov cx,0x1234
mov byte [bx],cl
;mov bx,0x301
mov byte [bx],ch
push word [bx]
pop ax
nop


hlt
_t4:
push ds
push cs
pop bx
pop ax
nop
hlt









l1:
nop
jmp l1
hlt




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;Third Block of Testing;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;conditionals and math

_third_block:
mov al,13
sub al,14
out 0xFF,al
hlt
jc _3success
jmp _3failure
hlt

_3success:
mov dx,0x1337 ;best thing I could come up with for success
nop
hlt

_3failure:
mov dx,0xBAD
nop
hlt




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;Fourth Block of Testing;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


_fourth_block:
mov cx,0xFFFF
mov ax,0
_return_here: ;for this test the comparison between real and virtual is
              ;Open86:.094s compared to Real hardware under windows: .063s
              ;and open86 also initializes a lot of stuff so it is great!!
              ;though if I don't do the 16bit overrides real hardware can get down to .015s

sub ax,1 ;4 cycles
loop _return_here ;5/17
;nop
mov al,0
out 0xFF,al



hlt







_full_test:
;call change_segs ;oops not supported yet
nop
call do_some_math
nop
nop
call cool_opcode
nop
nop


hlt



change_segs:
;push 1
;push 2
;push 3
pop ds
pop es
pop ss
ret


do_some_math:
mov ax,0x12
sub ax,1
jg _sucess1
jmp fail1
_sucess1:
mov dx,0x1337
ret
fail1:
mov dx,0xBAD
ret


cool_opcode:
mov ax,12
clc
salc
ret





;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;text testing;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_text_block:
mov al,0x0A
out 0xFA,al
mov al,'H'
out 0xFA,al
mov al,'e'
out 0xFA,al
mov al,'l'
out 0xFA,al
mov al,'l'
out 0xFA,al
mov al,'o'
out 0xFA,al
mov al,' '
out 0xFA,al
mov al,'f'
out 0xFA,al
mov al,'r'
out 0xFA,al
mov al,'o'
out 0xFA,al
mov al,'m'
out 0xFA,al
mov al,' '
out 0xFA,al
mov al,'O'
out 0xFA,al
mov al,'p'
out 0xFA,al
mov al,'e'
out 0xFA,al
mov al,'n'
out 0xFA,al
mov al,'8'
out 0xFA,al
mov al,'6'
out 0xFA,al
mov al,'!'
out 0xFA,al
mov al,0x0A
out 0xFA,al
mov al,0x0A
out 0xFA,al
nop
hlt








_mov_testing:
;mov ax,0x10
;mov ds,ax
mov ax,0x512
mov bx,0x150
mov si,2
mov [bx+si+0x102],ax
mov si,4
mov ax,0x410
mov [bx+si+0x102],ax
mov ax,1
mov ax,[bx+si+0x100] ;error here!
sub ax,0x512
jz sucess_mov1
hlt

sucess_mov1:
mov al,'!'
out 0xFA,al
mov ax,[bx+si+0x102]
sub ax,0x410
jz sucess_mov2
hlt
sucess_mov2:
mov al,'!'
out 0xFA,al
nop
mov ax,0xFFFF
mov ds,ax
mov bx,10
mov [bx],ax
hlt




nop
hlt



_int_testing:
;mov al,0
;out 0xFB,al
;mov sp,0x100
;mov dx,cs
;mov bx,0
;mov ds,bx
;mov cx,cs
;mov ax, _int_0
;mov [bx],cx
;mov bx,2
;mov [bx],ax
;nop
;mov al,0
;out 0xFB,al
;int 0
;mov ax,0xFAAA


mov al,0
mov sp,0x100
mov ax,100
mov ss,ax
mov ax,0
mov ds,ax
mov bx,0x40
mov cx,cs
mov [bx],cx
mov bx,0x42
mov cx,_int_10h
mov [bx],cx

;print a string!
mov ax,ds
mov es,ax
mov si,my_string
call PrintString

nop
hlt

my_string db 'h',0,'i'


_int_10h:
out 0x2F,al

iret


;not done yet..

_int_0:
mov al,'!'
out 0xFA,al
out 0xFA,al
;mov al,'?'
;out 0xFA,al
;hlt
iret


PrintString:
    push ds
    push es
    pop ds
    print_loopy:
        lodsb
        sub al, 0
        je Done
        mov ah, 0x0A
        int 0x10
        jmp print_loopy

Done:
pop ds
ret




__PrintString: ;PrintString --es:si
mov ax,es
push ds
mov ds,ax
mov ah,0x0A
mov bx,0
mov bp,0
print_loop:
mov al,[si+bp]
sub al,0 ;eventually replace with cmp
je print_done
mov cx,1
int 0x10
;temp only!
push ax
mov ax,bp
add ax,1
mov bp,ax
pop ax
jmp print_loop

print_done:
pop ds
ret







_add_testing:
mov ax,1
add ax,-1
jc carry
rety1:
jz zero
rety2:
nop
hlt
carry:
mov cx,1
jmp rety1
zero:
mov dx,1
jmp rety2




_new_modrm:
mov cx,0x500
mov ss,cx
mov sp,0x500
push cs
pop ds
mov bx,data_test
push word [bx]
pop ax
;mov ax,[bx]
mov dx,ax
mov cx,ax
sub ax,0x90F4
jz success__1

jmp data_test
success__1:
nop
hlt

data_test: hlt
nop

forever:
jmp forever



junk_data:
dd 20145463
dd 39248334
dd 838427
dd 3895494
dd 284722
dd 3854398
dd 384382
dd 3854382

jumping_test:
nop
hlt









