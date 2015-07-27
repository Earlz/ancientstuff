[org 0x7C0B]
[cpu 8086]
[section .text]
entry:
mov ax,cs
mov ds,ax
mov es,ax
mov ss,ax
mov sp,0x7000

;call ReadBoot


mov si,disk_driver_name
mov dl,0
mov bx,0x2000
mov es,bx
mov bx,0
call _ReadFile

mov si,fs_driver_name
mov dl,0
mov bx,0x3000
mov es,bx
mov bx,0
call _ReadFile

mov si,kernel_name
mov dl,0
mov bx,0x9000
mov es,bx
mov bx,0
call _ReadFile




jmp 0x9000:0


cli
hlt



ReadBoot:
ret


;DS:SI and ES:BX being null-terminated strings
;returns with zero flag set if the same
_CompareStrings: ;Works fully(though not always safe..)
push si
push bx
push ax
.loop1:
mov al,[SI]
cmp al,[ES:BX]
jne .leave

cmp al,0
je .leave
inc si
inc bx
jmp .loop1
.leave:
pop ax
pop bx
pop si
ret



;DS:SI file string(no directory)
;ES:BX buffer place(must have at least 512 bytes)
;DL drive
_ReadFile:
push ax
push bx
push di
mov ax,word [CS:0x7C09] ;gets it out of bootsector(already loaded)
jmp .first_skip

.more_search:
add ax,word [ES:bx+1]
.first_skip:
mov cx,1
push ax
push dx
call _ReadSector
pop dx
pop ax
jc .error
cmp byte [ES:bx],0x0F ;if not a file
jne .more_search
cmp byte [es:bx+12],byte 0 ;If not in the / directory
jne .more_search
push bx
add bx,16
call _CompareStrings
pop bx
jne .more_search


.loadfile: ;AX being sector of file(and file header info being in [ES:BX+0]
mov cx,[es:bx+1]
push cx
mov si,bx
add si,16
push ax
mov ah,0
mov al,[es:bx+11] ;get past the name string
add si,ax

mov di,bx
mov cx,512
mov ax,si
sub ax,bx
sub cx,ax
pop ax
push ds
push es ;mov ds,es
pop ds
rep movsb
pop ds

pop cx
mov bx,di
inc ax
dec cx ;if only 1 sector was to be read, ReadSector will simply return...
call _ReadSector



.error:
pop di
pop bx
pop ax

ret

;AX absolute sector number, DL drive
;ES:BX buffer place
;CX for number of sectors
;CF for error
_ReadSector: ;Not full error checking, but all works..
cmp cx,0
clc
je .leave
.over_check:
cmp ax,18*80*2-1
stc
jg .leave
push bx
mov [tmp],dl
.loop1:
push cx
push ax
push dx
mov cl,18 ;SPT (sectors per track)
div cl
mov cl,ah
inc cl ;now got sector
mov ah,0
mov ch,80 ;CPH(cylinders oer head)
div ch

mov ch,ah
mov dh,al

;set things up for int 13h
mov al,1 ;one sector
mov dl,[tmp]

;do it!
mov ah,0x02

int 0x13
jc .leave2
pop cx
pop ax
pop dx
add bx,512
inc ax
loop .loop1
pop bx
ret

.leave2:
pop cx
pop ax
pop dx
pop bx
.leave:
ret




[section .data]

disk_driver_name: db "dd.d",0
fs_driver_name: db "sfs.d",0
kernel_name: db "kernel16.bin",0


[section .bss]
reserved_space:
tmp: resb 1


