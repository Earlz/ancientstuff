;FDFS Boot Sector
;jumps to juleos bootsector(2 sectors)
;For FDFS 1.0 16bit
;BootSector 0.4 pre-alpha fdfs version; will be changed when designs are more final
;compiled using emu8086
ORG 7C00h ;boot signature

MOV     AH, 00h
MOV     AL, 03h
INT     10h
;set up v8086 mode here

MOV     AH, 02h ; read function.
MOV     AL, 17  ; sectors to read. 17/2=8.5kb
MOV     CH, 0   ; cylinder.
MOV     CL, 2   ; sector.
MOV     DH, 0   ; head.
; DL not changed! - drive number.
MOV     BX, 0500h   ;this is segment
MOV     ES, BX
MOV     BX, 0 ;this is intitial offset

; read!
INT     13h

MOV     AH, 02h ; read function.
MOV     AL, 18  ; sectors to read. 17/2=8.5kb*2=17kb+.5kb=17.5kb so should have enough room for expansion
MOV     CH, 1   ; cylinder. ;reached sector limit on 0 cylinder so advance to 1 and start fdfs on cylinder 2
MOV     CL, 1   ; sector.
MOV     DH, 0   ; head.
; DL not changed! - drive number.
MOV     BX, 0500h   ;this is segment
MOV     ES, BX
MOV     BX, 512*17 ;this is offset it reads to, must add what was already read 
int 13h
JMP     0500h:0000h ;this is where it jumps starts at the first segment
;now just to figure out how the crap im going to load the kernel using less than 3 sectors of space

;working!!

