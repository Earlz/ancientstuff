/*>>>>JuleOS<<<<*/
/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>Version 0.0.3 ALPHA<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>LowLevel.c; Low level C functions consiting mostly of inline asm, This is the true core<<<<*/
void putcur(byte,byte);
void printf(char*);
void testi(void);
byte iseven(word);
word memsize(void) /*gets full memory size(gets extended memory size and adds 1048575(1 meg) to it*/
{
	word size;
	asm mov ah,0x88
	asm int 0x15
	asm mov size,ax
	size=size+1024; /*1 meg, reads in kb not bytes*/
	return size;
}
	

void putdot(word x, word y, byte color)
{
	word offset;
		offset = 320 * y + x;
		pokeb(0xa000,offset,color);
}

byte getpix(word x,word y)
{
	word offset;
	byte color;
		offset = 320 * y + x;
		peekb(0xa000,offset,&color);
		return color;
}

void setvideomode(u8b mode) /*sets the video mode using int 10h*/
{
	asm mov ah,0x00
	asm mov al,mode
	asm int 0x10
}

byte getvideomode(void)
{
	byte ret;
	byte tmp;
	asm mov ah,0x0f
		asm int 0x10
		asm mov tmp,al
		ret=tmp;
		return ret;
}


void setvideopage(u8b page) /*sets the video page and updates the video_page variable*/
{
	asm mov al,page
	asm int 0x10
	video_page=page;
}
void reboot(void) /*warm reboots the pc*/
{
	asm include asm\reboot1.asm
	
/*
	asm mov bx,0xFFFF
		asm mov es,bx
		asm jmp WORD [es:0x0000]
*/
		}
/*
u16b memorysize(void)
{
	u16b a;
	u16b *ret;
	asm int 0x12
	asm mov a,ax 
	*ret=a;
	return *ret;
}
not working
*/
void bprint(u8b* chr) /*prints a single character to the screen*/
{
	asm mov ah,0x0e
	asm mov al,chr
	asm int 0x10
}

void bprint2(byte chr,byte col,byte row)
{
	word offset;
	offset=160*row+col*2;
	if (iseven(offset)==1){
	pokeb(0xb800,offset,chr);
	pokeb(0xb800,offset+1,0x0f);
	}else{
	pokeb(0xb800,offset+1,chr);
	pokeb(0xb800,offset+2,0x0f);
}
}


void read_disk2(u8b numsec,u8b track,u8b sector,u8b head,u8b drive, u8b high,u8b low) /*reads a single sector from the chosen disk; high and low is address to buffer*/
{
	byte rety;
	asm pusha
	asm push es
	asm mov ah,0x02
	asm mov al,numsec
	asm mov ch,track
	asm mov cl,sector
	asm mov dh,head
	asm mov dl,drive
	asm mov es,high
	asm mov bx,low
	asm int 0x13
	asm mov rety,ah
	asm pop es
	asm popa
	return rety;
}

void write_disk2(u8b numsec,u8b track,u8b sector,u8b head,u8b drive, word seg,word offset) /*rwrites a single sector from the chosen disk; high and low is address to buffer*/
{

	asm mov ah,0x03
	asm mov al,numsec
	asm mov ch,track
	asm mov cl,sector
	asm mov dh,head
	asm mov dl,drive
	asm mov es,seg
	asm mov bx,offset
	asm int 0x13
}
/*
void putdot(u8b color,u8b col,u8b row)  /*puts a single dot on the screen; col is x, row is y, only works in graphics mode* -depricated
{
	asm mov ah,0x0c
	asm mov al,color
	asm mov bh,video_page
	asm mov cx,col
	asm mov dx,row
	asm int 0x10
}
*/

void putcur(byte x,byte y) /*repositions the cursor*/
{
	asm mov ah,0x02
	asm mov dh,y
	asm mov dl,x
	asm mov bh,video_page
	asm int 0x10
}
keys getkey(void) /*gets a key from the buffer and WAITS if none*/
{
	keys rety;
	u8b tmpa,tmpb;
	asm mov ah,0x00
	asm int 0x16
	asm mov rety.scan,ah
	asm mov rety.asc,al
return rety;
}


/* -depricated
u8b getkey1(void) /*current to-be-replaced getkey function   -UPGRADE TO STRUCTS*
{
	u8b rety;
	u8b tmpa,tmpb;
	asm mov ah,0x00
	asm int 0x16
	/*asm mov scanc,ah*
	asm mov rety,al
	/*
	*key=tmpb;
	*scanc=tmpa;
*
return rety;
}
*/

byte getcurx() /*gets the current cursor X position*/
{
	u8b ret;
	u8b tmp;
	asm mov ah,0x03
	asm mov bh,video_page
	asm int 0x10
	asm mov tmp,dl
	ret=tmp;
	return ret;
}
byte getcury() /*gets the current cursor Y position*/
{
	u8b ret;
	u8b tmp;
	asm mov ah,0x03
	asm mov bh,video_page
	asm int 0x10
	asm mov tmp,dh
	ret=tmp;
	return ret;
}

 void _scrollup(int nol,int x1,int y1,int x2,int y2) /*scrolls the screen up*/
 {	asm	mov ah , 0x06
	asm	mov al , nol
	asm	mov cl , x1
	asm	mov ch , y1
	asm	mov dl , x2
	asm	mov dh , y2
	asm	mov bh , 0
	asm	int 0x10
 }
void poke(word segment,word offset,byte* dat){
	byte i;
	i=*dat;
	asm pusha
	asm push ds
	asm push i
	asm mov bx,segment
	asm mov ds,bx
	asm mov bx,offset
	asm pop ax
	asm mov [bx],al
	asm pop ds
	asm popa
}


void pokeb32(dword linear,byte dat){
	asm push si
	asm push ax
	asm mov esi,linear
	asm mov al,dat
	asm mov [gs:si],al
	asm pop ax
	asm pop si
}



/*

void int_init(void)  /*strange it jumps to ey whats up when a cls is used to prevent characters from going off the screen?
{
	
	bytes u;
	word cseg;
	byte num;
	asm mov bx,cs
	asm mov cseg,bx
	u=word2bytes(cseg);
	num=201;
	pokeb(0x0000,num,u.high);
	pokeb(0x0000,num+1,u.low);
/*all fine until here; guess i need a seperate file for interrupts?   asm mov bx,itesti: *
	asm lea bx,itesti
	asm mov cseg,bx
	u=word2bytes(0x0100);
	pokeb(0x0000,num+2,u.high);
	pokeb(0x0000,num+3,u.low);
/*
	asm cli;
asm xor ax,ax	
asm mov es,ax	
	asm mov WORD [0:83*4], bx
	asm mov bx,CS
asm mov WORD [es:83*4+2], bx
	asm sti
*
	}
void testi(void)
{

asm label itesti near
	printf("hey whats up");
	asm mov dx,50
	asm iret
}
*/

void int_init(void){
	asm include asm\ints.asm
}
void int22(void){
	printf("hello");
}

void farjmp(word seg,word offset)
{
	asm push seg
	asm push offset
	asm include asm\farjmpa.asm
	/*not too sure if this works and currently no way to test*/
}  

void load2mem(word segment,word offset,byte *dat, word size)
{
	byte *datptr;
	word a=0;
	datptr=dat;
	while (a<=size){
		
		pokeb(segment,offset+a,*datptr);
		datptr++;
		a++;
	}
	
	
}

byte getmem(word segment,word offset,word size)
{
	byte dat[512];
	word a;
	while (a<=size){
		
		dat[a]=peekb(segment,offset);
		a++;
	}
	return *dat;
}

void switch2flat(void){ /*switches to flat real mode- only affects gs and fs*/
	asm include asm\pmode.asm
}


/*
char* test_get()  /* asm code borrowed from an example from enu8086
{
	char* ret[80]
	
asm mov 



	PUSH    AX
PUSH    CX
PUSH    DI
PUSH    DX

MOV     CX, 0                   ; char counter.

CMP     DX, 1                   ; buffer too small?
JBE     empty_buffer            ;

DEC     DX                      ; reserve space for last zero.


;============================
; Eternal loop to get
; and processes key presses:

wait_for_key:

MOV     AH, 0                   ; get pressed key.
INT     16h

CMP     AL, 13                  ; 'RETURN' pressed?
JZ      exit


CMP     AL, 8                   ; 'BACKSPACE' pressed?
JNE     add_to_buffer
JCXZ    wait_for_key            ; nothing to remove!
DEC     CX
DEC     DI
PUTC    8                       ; backspace.
PUTC    ' '                     ; clear position.
PUTC    8                       ; backspace again.
JMP     wait_for_key

add_to_buffer:

        CMP     CX, DX          ; buffer is full?
        JAE     wait_for_key    ; if so wait for 'BACKSPACE' or 'RETURN'...

        MOV     [DI], AL
        INC     DI
        INC     CX
        
        ; print the key:
        MOV     AH, 0Eh
        INT     10h

JMP     wait_for_key
;============================

exit:

; terminate by null:
MOV     [DI], 0

empty_buffer:

POP     DX
POP     DI
POP     CX
POP     AX

}

*/


