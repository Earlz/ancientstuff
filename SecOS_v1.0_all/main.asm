;512OS;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;By Jordan Earlz(hckr83);;;;;;;;;;;;;;;;;;
;Started March 22, 2007;;;;;;;;;;;;;;;;;;;
;This is a small OS that is ;;;;;;;;;;;;;;
;that is completely contained;;;;;;;;;;;;;
;inside of a 512 byte boot sector;;;;;;;;;
;Note! this does not save the FAT ;;;;;;;;
;File system info!!;;;;;;;;;;;;;;;;;;;;;;;

;a bit of info;;;;;;;;;;;;;;;
;The stack segment is at 0 and by default sp is set to 0x800 at start of program
;CS is at 0; ip is at 0x7C00 (inherit with bootsector!)
;DS is at 0, same way as CS
;ES can be whatever

;I will use register calling conventions for everything, I think this is best because with stack
;you have to add extra instructions, and with a reserved memory area, you have to include a 16bit address in all of it


;NOTES: some things have been disabled to save space, this includes
;1. Skipping free sectors in NextFile, it will manually scan all sectors
;   as this works well, though a bit slower



org 0x7C00 ;boot sector ip offset

entry_point:
;;INITIALIZATION;;
mov ax,0
mov ss,ax
mov ds,ax
mov es,ax
mov sp,0x700
;AH is already 0..
mov al,3
int 0x10 ;set video mode to 3, 80x25x16(colors) (8x8 text)
;current color already 0x0F

;;initialize interrupt
;>>DO NOT ERASE BELOW!
mov bx,0x80*4
mov [bx],word FunctionInterrupt ;don't forget! the position of FunctionInterrupt is relative to 0, not CS!
add bx,2
mov [bx],word 0

jmp ShellBegin





;;EXTRA FUNCTIONS;;;;


;;_PrintString function, ES:SI points to the string to be printed, nothing else to do..
;; -->NO!!-->when done, SI will point to the end  of the string(where it is null terminated)
;;destroys nothing but flags
_PrintString:
pusha
mov ah,0x0E ;function is to print in TTY mode(for bios)

.writing_loop:
mov al,[ES:SI]
mov bx,0
cmp al,0 ;if there is null
je .end_loop


push ax
.normal_draw:
int 0x10 ;print the char

inc si
pop ax
cmp al,0x0A
je .add_return ;;if al was 0x0A
jne .writing_loop

.add_return: ;;this adds a return, this is so that we only have to use a one byte new line
mov al,0x0D
int 0x10
jmp .writing_loop

.end_loop:

popa
ret


FunctionInterrupt: ;;this is int 0x80
;;FUNCTION LIST;;
;ah=0 --Return to main kernel(nothing else)
;ah=1 --print string; ES:SI points to the null terminated string
;ah=2 --restart to kernel entry point(not usually needed)
;ah=3 --Where am I loaded; This will give you your abs sector number of where you
;       are loaded in ax and bx contains the length of this block
;ah=4 --Get List Of Files; This will load a list of files at 0x0000:0x500 and will return in al how many strings were loaded
;       note! This has a limit of 256 bytes!
;       also, at 0:0x600, for every string, it will have the starting sector and length of the file(a word for both of them)
;ah=5 --End this and Load that; This will end the current program and then load the filename specified in ES:SI
;ah=6 --Format This Floppy; This will format the current floppy, making all sectors to be "blank" for files, and
;       also will copy this bootsector onto it
push ds
push 0
pop ds


cmp ah,1
je .call_print_string

jmp .end

.call_print_string:
call _PrintString

.end:
pop ds
iret
;;

;AX should be the absolute sector number
;it returns CHS in the way that int 13h takes it
;cl is sector;ch is track; dh is head
;_abs2chs: ;completely working!
;ES:BX is buffer address
;dl is drive number
_WriteAbsSector:
mov [read_or_write],3
jmp continue_abs_sector
_ReadAbsSector:
mov [read_or_write],2
continue_abs_sector:
;cmp ax,18*80*2-1
;jg .failme
mov cl,18 ;SPT (sectors per track)
;inc ax
div cl
;mov bp,ax
mov cl,ah
inc cl ;now got sector

;mov dl,al ;save it for a bit
mov ah,0
mov ch,80 ;CPH(cylinders oer head)
;hlt
div ch

mov ch,ah
mov dh,al

;set things up for int 13h

mov dl,0
mov ah,[read_or_write] ;we wrote this earlier
mov al,1 ;one sector
;hlt
;do it!
int 0x13
;leave all those values intact..
ret
.failme:
;mov ax,-1
;ret

;;Lightly tested(unchecked free space or iterations more than 2)
NextFile: ;;This will "browse" files from StoopidFS floppies;
		  ;;AX should be set to where to start, this is usually 1, unless continueing a search
		  ;;This will only add ONE file
		  ;;AX will be returned as 0xFFFF(-1) if we reach the end, else AX is what sector number+length
		  ;;CF set on return if there was a disc error, in which case AX will return the error code
		  ;;ES:DI should contain an array of 13 chars in which to store the filename
		  ;;DX contains the first sector number of the found file
push bx


.checking_loop:
cmp ax,18*80*2-1

jg .failed_loop

push es
push 0
pop es
mov bx,floppy_buffer
mov dl,0 ;neccessary?

push ax
push cx
mov cx,5

.can_read:
call _ReadAbsSector




jnc .read_it
loop .can_read

;meh..don't do anything for error checking
.read_it:
pop cx
pop ax ;ax is destroyed in reading abs sector
pop es


cmp byte [floppy_buffer], 0x3C
je .found_file ;if we found the file
jne .skip_sectors
;cmp byte [floppy_buffer], 0x2C
;je .skip_sectors
;else continue looping

;cmp byte [floppy_buffer],0x2C
;je .skip_sectors
inc ax
.return_looping:

jmp .checking_loop
.failed_loop:
mov ax,-1
mov [es:si],byte 0

pop bx ;finish up
ret


.skip_sectors:
add ax,word [floppy_buffer+1]
jmp .return_looping


.found_file:
;ax is already set
;es is already 0
push si
mov si,floppy_buffer+3 ;haev it point to the filename string in floppy buffer

push ax
push di
push 0
pop ds
push cx
mov cx,16
.loop_string:

mov al,[ds:si]
mov [es:di],al
dec al ;cmp al,0
je .finish_loop_string
inc di
inc si

loop .loop_string
.finish_loop_string:
pop cx
pop di
pop ax
pop si
mov dx,ax
add ax,word [es:floppy_buffer+1]
;pop cx

;just continue..



.finish_up:

pop bx



ret

;halt_me:
;cli
;hlt

ReadFileIntoMemory:
;This will read a file into memory, [0:$filename] should be used as the name space for now
;ES:DI is destination space
;returns AX as the file size loaded

mov ax,0
push es
push di
mov es,ax
inc ax
mov si,filename
mov di,filename_space
.finding_loop:
call NextFile

cmp ax,-1
je .failed
call IsStrEqual
je .found_file
jmp .finding_loop

.found_file:
pop di
pop es
mov bx,di
mov ax,dx
call _ReadAbsSector

push cx
mov cx,[es:di+1]
cmp cx,word 1

je .finish
sub cx,2 ;decrement it by 2 so that if it is 2 it would be 0, so we would only go through it one extra time
inc ax
;if it's not a one sector file, then load the rest of it
.read_the_rest:
push dx
add bx,512
call _ReadAbsSector
pop dx
inc dx
loop .read_the_rest

.finish:

pop cx
ret

.failed:
pop di
pop es
ret



GetString:
;ES:DI should point to where the string space is
;CX is how long the space is(to avoid overflow
pusha ;save me some bytes!
.get_chars:
mov ah,0
int 0x16 ;get char from keyboard

mov ah,0x0E
mov bx,0
;al already set
cmp al,0x0D
je .got_enter
int 0x10 ;display char
mov [es:di],al
cmp al,0x08
je .backspace
inc di
loop .get_chars
.backspace:
mov al,' '
int 0x10
mov al,0x08
int 0x10
dec di
loop .get_chars
.got_enter:
;mov al,0x0A ;\n
;int 0x10 ;display the new line
mov byte [es:di],0 ;null terminate
popa
ret



ShellBegin:

.infinite_loop:

mov si,pre_cur
call _PrintString
mov di,StringSpace
mov cx,255
call GetString


mov cx,255
cld
mov di,StringSpace
;es and ds already set
;di is what the user entered
;si is our comparison string
mov si,test_command
call IsStrEqual
je .do_test_command
mov si,list_files_command
call IsStrEqual
je .list_files
mov si,exec_command
call IsStrEqual
je .do_exec_command



jmp .infinite_loop

.do_test_command: ;;<<THIS is working! it will display the bit of text in the files
				  ;;sadly at the time of this writing, I only have TWO extra bytes though
;ZF is ONE! this is because we used je to get here, so we use setcc to optimize this out a bit
;AL is used temporarily in this! it isn't used in any of this so..
sete al
jmp .start_it
.do_exec_command: ;;<<THIS is working, it will execute the file given
setne al ;got this down to two bytes!
.start_it:
mov di,filename
mov cx,256
mov si,file_comment
call _PrintString
call GetString
cmp al,1
je .t_command
;push 0x100
;pop es
mov di,0x1000
call ReadFileIntoMemory
;cmp ax,-1
;je .infinite_loop
;cli
;hlt
call  0x1000+24 ;had to make sure to add that +24 offset

jmp .infinite_loop

.t_command: ;;<<This willl print the string of a sector
mov di,filespace_1

call ReadFileIntoMemory
mov si,new_line
call _PrintString
mov si,filespace_1+24
call _PrintString


jmp .infinite_loop



.list_files:
mov [tmp2],0
mov si,new_line
call _PrintString
mov ax,1
mov di,filename_space

.list_files_loop:
call NextFile
cmp ax,-1
je .finish_list_files
mov si,di
call _PrintString


mov si,new_line
call _PrintString

jmp .list_files_loop


.finish_list_files:
jmp .infinite_loop


;ds:si is source string
;es:di is other stirng
;returns with ZF set if equal
IsStrEqual:
pusha
mov cx,256
.check_loop:
cmpsb
jne .not_equal
cmp byte [si],0
je .equal ;this means we reached the null termination
loop .check_loop
.too_long:
stc
.equal:
;this iwll be sure to set ZF
.not_equal:
 ;this will be sure to clear ZF
popa ;a bit expensive, but I need the bytes!
ret


;;;Initialized Variables(strings);;;



pre_cur: db 0x0A,">",0
new_line: db 0x0A,0
test_command: db "t",0
exec_command: db 'x',0
list_files_command: db "d",0
file_comment: db 0x0a,":",0
current_color: db 0x0F


;fspacing: db 0x0A,0
;time_stamp: dd %t
;db 0

;0x0A is \n



;*************************************
; Make program 510 byte's + 0xaa55
;*************************************

times 510- ($-entry_point)  db 0
boot_signature: dw 0xaa55 ;boot signature


;;;UNINITIALIZED MEMORY;;;
;This has memory that can't be specified for anything..
;current_color rb 1
tmp rb 1
read_or_write rb 1
floppy_buffer rb 512
StringSpace rb 256
filename_space rb 17
tmp2 rb 1
tmp3 rb 1
filename rb 17
filespace_1 rb 1024



bits = 16
display 'End Address is at 0x'
repeat bits/4
d = '0' + $ shr (bits-%*4) and 0Fh
if d > '9'
d = d + 'A'-'9'-1
end if
display d
end repeat
display 13,10
;	#define SPT 81
;	#define Heads 2
;sector = (abs%SPT)+1; -cl
;track = (abs/SPT)/Heads; -ch
;head = (abs/SPT)%Heads; -dh





;a bit more detailed info
;basically, it doesn't tell any info about the current device, Though it is usually assumed that it is on a 1.44mb floppy
;There are no "system" sectors like there are in most OS's, this is designed so that it could be done anything with..

;There is no index for finding files, you must scan through the whole floppy, when there are a lot of files this can be VERY slow, but for a simple thing with a few files it should work relatively well

;The only way to identify if a sector is a start of a file, empty, or data of a file, is to check the very first byte of the sector(this starts at abs sector 2)
;0x3C means start of file
;0x2C means empty space
;0x1C means reserved

;any other value is assumed to be a corrupted sector

;For the start of a file, the file info is stored like this,
;- 0x3C
;- word value of how many immediately following sectors
;- string of 16 chars for the filename(null terminated)
;- dword value of how many actual bytes in the file(this need not be used for a simple boot sector and you should use the above)
;- attribute byte, in this format(going from bit 7 to bit 0 using | as a seperator)
;read only|hidden|system|do not move me(for defrag apps)| everything else is unused
;at :25(decimal)
;-now just the data of the file

;note that no value is placed in the first byte of sectors in the "data" of a file, this is only placed at the first file sector

;free space info is stored like this:
;- 0x2C
;- word value of how many immediately following sectors are free

;reserved space info is stored like this:
;-0x1C
;- word value of how many immediately following sectors are free


;It is really quite simple now isn't it?
;you can add your own index file if wanted or something, whatever

;the only bad thing about this design is that it doesn't support fragmentation, so a "space alignment" program may need to be made to correctly align spots of free space so as to make more free space readily available

;also note that nothing in the boot sector is needed to be specified like in fat and most other FSs..(though this forces you to assume a lot)




;;MEMORY MAP;;
;0-500	 			reserved for BIOS and Interrupts
;500-700			unused
;700-800			Used for a stack of 128 bytes(64 values) for kernel
;800-7C00			free
;7C00-9000			Reserved for kernel code and variables
;9000-10000			a huge free area
;10000-9FE00		Applications are loaded here
;9FE00-A0000		Stack for applications
;everything above this is reserved for hardware or is ROM








