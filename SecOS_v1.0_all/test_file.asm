;because of how well FASM is with making "automated" binary files, I use it
;instead of a hex editor
first_file:
db 0x3C ;file verification byte
dw 0x0001 ;only 1 sector is used
.fname: db "testing1.txt" ;the filename string
times 16-($-.fname) db 0 ;fill in the rest of the 16bytes for the filename
dd 0x00000000 ;actual filesize(unused)
db 0x0 ;attribute byte
;actual file data

.our_message: db "hello there! this is sector one!",0

times 512- ($-first_file)  db 0 ;this will complete the sector for the file
;we are now in the second sector
org 0x1000
second_file:


db 0x3C
dw 0x0001
.fname: db "#2 testing.txt"
times 16-($-.fname) db 0
dd 0x00000000 ;actual filesize(unused)
db 0x0 ;attribute byte

jmp .after
.our_message: db "This is sector two!",0
.after:
mov ax,0
mov es,ax
mov si,.t_string
mov ah,1
int 0x80
mov ax,0xC001
mov gs,ax
retf ;return to the shell

.t_string: db 0x0a,"Hello from sector two!",0x0a,"This is actually a whole different program!",0


times 512- ($-second_file)  db 0

third_file: ;this is actually a block of empty sectors
db 0x2C
dw 2877 ;rest of the floppy is empty
.fname: db "#3 testing.txt"
times 16-($-.fname) db 0
dd 0x00000000 ;actual filesize(unused)
db 0x0 ;attribute byte
.jump:
jmp .after
.our_message: db "Finally! sector three!",0

.after:
cli
hlt

times 512-($-third_file) db 0
