
push es
mov bx,0
mov es,bx
lea bx,int22
mov es:[280],bx
mov es:[282],cs
pop es
ret
int22:
push ds
MOV AX, 0B800h    ; set AX to hexadecimal value of B800h.
MOV DS, AX        ; copy value of AX to DS.
MOV CL, 'A'       ; set CL to ASCII code of 'A', it is 41h.
MOV CH, 01011111b ; set CH to binary value.
MOV BX, 15Eh      ; set BX to 15Eh.
MOV [BX], CX      ; copy contents of CX to memory at B800:015E
pop ds
iret

;working in emu8086 but nothing else