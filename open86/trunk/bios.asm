;SECTION .text
;[bits 16]
org 0
cli

mov di,2
mov cx,16
mov dx,0
mov ds,dx
mov bx,32
.loop_interrupt:
mov dx,cs
mov [bx],dx
mov dx,empty_int
add bx,di
mov [bx],dx
add bx,di
loop .loop_interrupt
sti

mov cx,0xb800
mov ds,cx
;mov bx,0
mov cx,255
mov bx,512
mov di,-1
mov dl,0xF0

.text_loop:
mov [bx],cl
add bx,di
mov [bx],dl
add bx,di
loop .text_loop
nop

jmp bah
bah:
jmp bah

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
mov bx,8*8
mov cx,cs
mov [bx],cx
mov bx,8*8+2
mov cx,_int_8h

mov [bx],cx

int 0x10

nop
hlt

my_string db 'h',0,'i'


_int_8h:
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







_add_testing: ;works!
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




_new_modrm: ;works!
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

jumping_test: ;fixed!
nop
hlt


mul_testing: ;works!
mov ax,1024
mov bx,128

mul bx
nop
mov ax,dx
sub ax,word 2
jz _mul_success
hlt
_mul_success:
nop
hlt


div_testing: ;works!!
mov ax,0x000
mov dx,0x1
;mov dx,0x1
mov bx,2
;nop
div bx
nop
;nop
;mov ax,dx
sub ax,word 0x8000
jz div_success
hlt
div_success:
nop
hlt



cmp_testing: ;Works!
mov ax,0xd123
mov bx,0xF123
cmp al,bl
;nop
;now test memory...
mov ax,0x2121 ;random stack segment
mov ss,ax
mov sp,0x500
push cs
pop ds ;set stack to the current segment
mov ax,0xF154
mov bx,cmp_temp_mem
cmp word [bx],ax


jz cmp_success
hlt
cmp_success:
nop
mov ax, 0x12
out 0xFF,ax ;endme port
hlt
cmp_temp_mem: dw 0xF154


contrib_testing:
mov ax,12
inc ax
nop
je contrib_success
hlt
contrib_success:
nop
hlt


bitwise_testing:
mov ax,1100100101b
mov bx,1000111011b
and ax,bx
mov cx,1000100001b
cmp ax,cx
je bitwise_success
hlt
bitwise_success:
nop
hlt











success:
nop
hlt

failure:
hlt
iret

init: ;this will initialize stack and segments and eventually other things..
mov ax,0x100
mov ss,ax
mov sp,0x80
mov ax,cs
mov ds,ax
mov es,ax
ret


more_testing:
mov cx,7
mov ax,cs
mov ds,ax
mov es,ax
mov si, string1+6
mov di, string2+6
cld

repe cmpsb

jne success
jmp failure

string1: db "shello"
string2: db "shella"


small_benchmark:
mov cx,0xFFFF
extra_loop:

mov ax,0x90
mov ds,ax
mov si,0
mov dx,0x0
mov ss,dx
mov sp,0x800
mov ax,0x2000
mov es,ax
mov di,0
push cx

mov cx,0xFFFF
mov bx,0

loop_set:
mov [bx],cx
;nop
mov bx,cx
loop loop_set

push ds
push es

pop ds
mov bx,0
mov cx,0xFFFF
loop_set2:
mov [bx],cx
mov bx,cx
loop loop_set2

pop ds


mov cx,0xFFFF
repe cmpsw

mov ax,0
cmp cx,ax

je success_bench
hlt
success_bench:

pop cx
loop extra_loop
mov ax, 0x12
out 0xFF,ax ;endme port
;nop

hlt


device_testing:
mov bx,0x20
mov ax,0x50
mov [bx],ax
mov ax,0xB800
mov ds,ax
mov es,ax

mov cx,255
_draw_loop:
mov bx,cx
mov [bx],byte cl
loop _draw_loop




nop
infinite_loop:
jmp infinite_loop
testi:
nop
ret

_call_test:
mov ax,10
mov ss,ax
mov sp,0x80

call testi

hlt


;;;;;random useful things;;;

PutPoint: ;ax=x bx=y cx=count dl=color
push ds
push di
push ax
push bx
push cx
push dx

mov di, 0xA000
mov ds,di

mov di,0

;nop
;this translates to C as bp=ax(x)+(bx(y)*320)
;mov bp,0
mov di,ax ;di=ax
mov ax,320
push dx
mul bx ;(bx*320)
pop dx ;ignore top word..
add di,ax ;+

;cx is already setup

_drawing_loop:
;nop
mov byte [di],dl
mov ax,1

add di,ax
loop _drawing_loop
pop dx
pop cx
pop bx
pop ax

pop di
pop ds

ret
;;END;;
failure1:
nop
nop
hlt

_interrupt:
cli
mov ax, 0x12
out 0xFF,ax ;endme port
hlt

empty_int:
mov ax, 0x12
out 0xFF,ax
nop

iret



again_display_test:
mov di,2
mov cx,16
mov dx,0
mov ds,dx
mov bx,32
_loop_interrupt:
mov dx,cs
mov [bx],dx
mov dx,empty_int
add bx,di
mov [bx],dx
add bx,di
loop _loop_interrupt
sti

mov cx,0xb800
mov ds,cx
;mov bx,0
mov cx,255
mov bx,512
mov di,-1
mov dl,0xF0

text_loop:
mov [bx],cl
add bx,di
mov [bx],dl
add bx,di
loop text_loop
nop
jmp $


display_a:
mov ax,0xB800
mov ds,ax
mov cl,'a'
mov dl,0x0F
mov [0],cl
mov [1],dl
nop
jmp infinite_loop




display_test:
mov [0],CS
mov bx,timer_test

mov [2],bx

mov bx,1
mov ax,0
mov si,0
mov dl,0
;mov di,2
mov cx,word 0xFF

;sti
;jmp infinite_loop
_colors_loop:
mov di,1
add ax,di
;push cx
;mov cx,3
;pop si

;push si
push cx
mov dx, cx
mov cx,2

;cmp ah,0xF1

;je infinite_loop

call PutPoint
;nop
pop cx
;nop
loop _colors_loop
mov bp,10
cmp bx,bp
je infinite_loop
mov cl,0xFF
mov ax,0
mov dx,1
add bx,dx
mov dl,0
jmp _colors_loop

nop

jmp infinite_loop
tmp: db 0

timer_test:
jmp infinite_loop
;iret


_clear_screen:
push cx
push dx
push ax
push bx
push di
mov di,0
mov cx,0xFFFF
mov bx,0
mov ax,cx
mov dx,0
clear_loop:
mov ax,cx
call PutPoint
loop clear_loop
pop di
pop bx
pop ax
pop dx
pop cx
ret

new_opcodes:
mov dx,0xB800
mov ds,dx
mov bp,0
mov si,0

mov ax,1
mov bx,1
mov dl,'h'
mov dh,0x0F
call PutChar
jmp infinite_loop

jnz video_failure
;mov [1],byte 0x0F

video_success:
mov [0],byte 'y'
mov [1],byte 0x0F
jmp infinite_loop

video_failure:
mov[0],byte 'n'
mov[1],byte 0x0F
jmp infinite_loop

PutChar: ;ax and bx is x and y(respectively) dl is character, dh is color
push es
push cx
push bx
mov cx,0xb800
mov es,cx
mov cx,ax
;pos=y*(80)+x
push ax
push dx

mov ax,80
mul bx
pop dx
mov cx,ax
pop ax
add cx,ax
push ax
push dx
mov ax,2
mul cx
pop dx
mov bx,ax
pop ax
mov [bx],dl
mov [bx+1],dh
pop bx
pop cx
pop es
ret

PutString: ;ES is string segment, SI is string offset, dl is color, ends when a char in the string is 0









