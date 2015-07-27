; VIDEO.ASM
;
; Miscellaneous assembly routines for video I/O in Piclab.  This code is
; copyrighted (C) 1988,89,90 by Lee Daniel Crocker and John Bridges, but
; is freely distributable.  Most of the Super VGA code is John's and was
; generously donated to the developers of the CompuServe PICS forum.  I
; have made some changes to his original code, so any screw-ups are
; probably mine.
;
; John can be reached at CompuServe [73307,606] or through GRAFX group:
;
; GRAFX Group
; 30448 Rancho Viejo, Suite 100
; San Juan Capistrano, CA  92675
;
;	Modified by Bert Tyler and Charles Marslett on 8/94 to add
;	1024x768x256, 1280x1024x256, and Sierra/VESA true-color support
;	If someone wants to add non-VESA true-color support, do so by:
;	Adding the proper chip-specific BIOS values to the ax21/bx21
;	through ax34/bx34 arrays
;	The existing 'truecolorline' routine should be able to handle
;	the actual scanline display function.

extrn	_palette:word
extrn	_vesadetect:word
extrn	_gcmap:byte

V_TEXT	segment word public 'CODE'
V_TEXT	ends
_DATA	segment word public 'DATA'
_DATA	ends
segment word public 'CONST'
ends
_BSS	segment word public 'BSS'
_BSS	ends

DGROUP	group	CONST, _BSS, _DATA
	assume	cs:V_TEXT, ds:_DATA, ss:DGROUP

	public	_chipset
	public	_memsize

_DATA	segment

curbk	dw	0
bankadr	dw	offset V_TEXT:$nobank
_chipset dw	0
_memsize dw	0
vgatype	dw	0

result	dw	0
rsave	dw	0
gsave	dw	0
bsave	dw	0
dist	dd	0
ldist	dd	0

;		Cirrus        Tseng     C&T       ATI       AheadA    VESA
;		       Video7      Paradise  Trident   Everex    AheadB

;		640x400x256 mode-setting values, by chipset
ax1	dw	6f05h, 6f05h, 2fh,   5eh, 78h, 5ch, 61h, 70h, 60h, 60h, 4f02h
bx1	dw	66h,   66h,   0,     0,   0,   0,   0,   14h, 0,   0,   100h

;		640x480x256 mode-setting values, by chipset
ax2	dw	6f05h, 6f05h, 2eh,   5fh, 79h, 5dh, 62h, 70h, 61h, 61h, 4f02h
bx2	dw	67h,   67h,   0,     0,   0,   0,   0,   30h, 0,   0,   101h

;		800x600x256 mode-setting values, by chipset
ax3	dw	6f05h, 6f05h, 30h,   5ch, 7bh, 5eh, 63h, 70h, 62h, 62h, 4f02h
bx3	dw	69h,   69h,   0,     0,   0,   0,   0,   31h, 0,   0,   103h
 
;		1024x768x256 mode-setting values, by chipset
ax4	dw	6f05h, 6f05h, 38h,   0,   0,   62h, 0,   70h, 63h, 63h, 4f02h
bx4	dw	6ah,   6ah,   0,     0,   0,   0,   0,   32h, 0,   0,   105h

;		1280x1024x256 mode-setting values, by chipset
ax5	dw	0,     0,     3eh,   0,   0,   0,   0,   0,   0,   0,   4f02h
bx5	dw	0,     0,     0,     0,   0,   0,   0,   0,   0,   0,   107h

;		640x480xTrueColor (16-bit) mode-setting values, by chipset
ax21	dw	0,     0,     10f0h, 0,   0,   0,   0,   0,   0,   0,   4f02h
bx21	dw	0,     0,     002eh, 0,   0,   0,   0,   0,   0,   0,   111h

;		800x600xTrueColor (16-bit) mode-setting values, by chipset
ax22	dw	0,     0,     10f0h, 0,   0,   0,   0,   0,   0,   0,   4f02h
bx22	dw	0,     0,     0030h, 0,   0,   0,   0,   0,   0,   0,   114h

;		1024x768xTrueColor (16-bit) mode-setting values, by chipset
ax23	dw	0,     0,     10f0h, 0,   0,   0,   0,   0,   0,   0,   4f02h
bx23	dw	0,     0,     0038h, 0,   0,   0,   0,   0,   0,   0,   117h

;		1280x1024xTrueColor (16-bit) mode-setting values, by chipset
ax24	dw	0,     0,     10f0h, 0,   0,   0,   0,   0,   0,   0,   4f02h
bx24	dw	0,     0,     003eh, 0,   0,   0,   0,   0,   0,   0,   11ah

;		640x480xTrueColor (24-bit) mode-setting values, by chipset
ax31	dw	0,     0,     10f0h, 0,   0,   0,   0,   0,   0,   0,   4f02h
bx31	dw	0,     0,     2effh, 0,   0,   0,   0,   0,   0,   0,   112h

;		800x600xTrueColor (24-bit) mode-setting values, by chipset
ax32	dw	0,     0,     10f0h, 0,   0,   0,   0,   0,   0,   0,   4f02h
bx32	dw	0,     0,     30ffh, 0,   0,   0,   0,   0,   0,   0,   115h

;		1024x768xTrueColor (24-bit) mode-setting values, by chipset
ax33	dw	0,     0,     10f0h, 0,   0,   0,   0,   0,   0,   0,   4f02h
bx33	dw	0,     0,     38ffh, 0,   0,   0,   0,   0,   0,   0,   118h

;		1280x1024xTrueColor (24-bit) mode-setting values, by chipset
ax34	dw	0,     0,     10f0h, 0,   0,   0,   0,   0,   0,   0,   4f02h
bx34	dw	0,     0,     3effh, 0,   0,   0,   0,   0,   0,   0,   11bh

;		Cirrus        Tseng     C&T       ATI       AheadA    VESA
;		       Video7      Paradise  Trident   Everex    AheadB


vptbl	dw	06b00h	; horz total
	dw	05901h	; horz displayed
	dw	05a02h	; start horz blanking
	dw	08e03h	; end horz blanking
	dw	05e04h	; start h sync
	dw	08a05h	; end h sync
	dw	00d06h	; vertical total
	dw	03e07h	; overflow
	dw	04009h	; cell height
	dw	0ea10h	; v sync start
	dw	0ac11h	; v sync end and protect cr0-cr7
	dw	0df12h	; vertical displayed
	dw	02d13h	; offset
	dw	00014h	; turn off dword mode
	dw	0e715h	; v blank start
	dw	00616h	; v blank end
	dw	0e317h	; turn on byte mode
vpend	label	word

bayer2	db	2, 130, 34, 162, 10, 138, 42, 170
	db	194, 66, 226, 98, 202, 74, 234, 106
	db	50, 178, 18, 146, 58, 186, 26, 154
	db	242, 114, 210, 82, 250, 122, 218, 90
	db	14, 142, 46, 174, 6, 134, 38, 166
	db	206, 78, 238, 110, 198, 70, 230, 102
	db	62, 190, 30, 158, 54, 182, 22, 150
	db	254, 126, 222, 94, 246, 118, 214, 86

_DATA	ends

V_TEXT	segment

$cirrus	proc	near
	sub	di, di
	mov	dx, 3d4h	; assume 3dx addressing
	mov	al, 0ch		; screen a start address hi
	out	dx, al		; select index
	inc	dx		; point to data
	mov	ah, al		; save index in ah
	in	al, dx		; get screen a start address hi
	xchg	ah, al		; swap index and data
	push	ax		; save old value
	push	dx		; save crtc address
	xor	al, al		; clear crc
	out	dx, al		; and out to the crtc

	mov	al, 1fh		; Eagle ID register
	dec	dx		; back to index
	out	dx, al		; select index
	inc	dx		; point to data
	in	al, dx		; read the id register
	mov	bh, al		; and save it in bh

	mov	cl, 4		; nibble swap rotate count
	mov	dx, 3c4h	; sequencer/extensions
	mov	bl, 6		; extensions enable register

	ror	bh, cl		; compute extensions disable value
	mov	ax, bx		; extensions disable
	out	dx, ax		; disable extensions
	inc	dx		; point to data
	in	al, dx		; read enable flag
	or	al, al		; disabled ?
	jnz	exit		; nope, not an cirrus

	ror	bh, cl		; compute extensions enable value
	dec	dx		; point to index
	mov	ax, bx		; extensions enable
	out	dx, ax		; enable extensions
	inc	dx		; point to data
	in	al, dx		; read enable flag
	cmp	al, 1		; enabled ?
	jne	exit		; nope, not a cirrus
	mov	di, 1
exit:
	pop	dx		; restore crtc address
	dec	dx		; point to index
	pop	ax		; recover crtc index and data
	out	dx,ax		; restore crtc value
	ret
$cirrus	endp

$gochk	proc	near
	push	si
	mov	si, bx

	mov	al, cl
	call	dx
	xchg	bl, es:[di]
	mov	al, ch
	call	dx
	xchg	bh, es:[di]

	xchg	si, bx

	mov	al, cl
	call	dx
	xor	bl, es:[di]
	mov	al, ch
	call	dx
	xor	bh, es:[di]

	xchg	si, bx

	mov	al, ch
	call	dx
	mov	es:[di], bh
	mov	al, cl
	call	dx
	mov	es:[di], bl

	mov	al, 0
	call	dx
	or	si, si
	pop	si
	ret
$gochk	endp

$chkbk	proc	near		;paradise bank switch check
	mov	di, 0b800h
	mov	es, di
	xor	di, di
	mov	bx, 1234h
	call	$gochk
	jnz	nopara
	mov	bx, 4321h
	call	$gochk
	jnz	nopara
	clc
	ret
nopara:
	stc
	ret
$chkbk	endp

$pdrsub	proc	near		;Paradise
	push	dx
	mov	ah, al
	mov	dx, 3ceh
	mov	al, 9
	out	dx, ax
	pop	dx
	ret
$pdrsub	endp

$tseng	proc	near		;Tseng
	push	ax
	push	dx
	and	al, 7
	mov	ah, al
	shl	ah, 1
	shl	ah, 1
	shl	ah, 1
	or	al, ah
	or	al, 01000000b
	mov	dx, 3cdh
	out	dx, al
	pop	dx
	pop	ax
	ret
$tseng	endp

$tseng4	proc	near		;Tseng
	push	ax
	push	dx
	and	al, 15
	mov	ah, al
	shl	ah, 1
	shl	ah, 1
	shl	ah, 1
	shl	ah, 1
	or	al, ah
	mov	dx, 3cdh
	out	dx, al
	pop	dx
	pop	ax
	ret
$tseng4	endp

$trident proc	near		;Trident
	push	ax
	push	dx
	mov	dx, 3ceh	;set page size to 64k
	mov	al, 6
	out	dx, al
	inc	dl
	in	al, dx
	dec	dl
	or	al, 4
	mov	ah, al
	mov	al, 6
	out	dx, ax
		
	mov	dl, 0c4h	;switch to BPS mode
	mov	al, 0bh
	out	dx, al
	inc	dl
	in	al, dx
	dec	dl

	mov	ah, byte ptr [curbk]
	xor	ah, 2
	mov	dx, 3c4h
	mov	al, 0eh
	out	dx, ax
	pop	dx
	pop	ax
	ret
$trident endp

$video7 proc	near		;Video 7
	push	ax
	push	dx
	push	cx
	and	ax, 15
	mov	ch, al
	mov	dx, 3c4h
	mov	ax, 0ea06h
	out	dx, ax
	mov	ah, ch
	and	ah, 1
	mov	al, 0f9h
	out	dx, ax
	mov	al, ch
	and	al, 1100b
	mov	ah, al
	shr	ah, 1
	shr	ah, 1
	or	ah, al
	mov	al, 0f6h
	out	dx, al
	inc	dx
	in	al, dx
	dec	dx
	and	al, not 1111b
	or	ah, al
	mov	al, 0f6h
	out	dx, ax
	mov	ah, ch
	mov	cl, 4
	shl	ah, cl
	and	ah, 100000b
	mov	dl, 0cch
	in	al, dx
	mov	dl, 0c2h
	and	al, not 100000b
	or	al, ah
	out	dx, al
	pop	cx
	pop	dx
	pop	ax
	ret
$video7 endp
	
$paradise proc	near		;Paradise
	push	ax
	push	dx
	mov	dx, 3ceh
	mov	ax, 50fh	;turn off write protect on VGA registers
	out	dx, ax
	mov	ah, byte ptr [curbk]
	shl	ah, 1
	shl	ah, 1
	shl	ah, 1
	shl	ah, 1
	mov	al, 9
	out	dx, ax
	pop	dx
	pop	ax
	ret
$paradise endp

$chipstech proc	near		;Chips & Tech
	push	ax
	push	dx
	mov     dx, 46e8h	;place chip in setup mode
	mov     ax, 1eh
	out     dx, ax
	mov     dx, 103h	;enable extended registers
	mov     ax, 0080h
	out     dx, ax
	mov     dx, 46e8h	;bring chip out of setup mode
	mov     ax, 0eh
	out     dx, ax
	mov	ah, byte ptr [curbk]
	shl	ah, 1		;change 64k bank number into 16k bank number
	shl	ah, 1
	mov	al, 10h
	mov	dx, 3d6h
	out	dx, ax
	pop	dx
	pop	ax
	ret
$chipstech endp

$ativga proc	near		;ATI VGA Wonder
	push	ax
	push	dx
	mov	ah, al
	mov	dx, 1ceh
	mov	al, 0b2h
	out	dx, al
	inc	dl
	in	al, dx
	shl	ah, 1
	and	al, 0e1h
	or	ah, al
	mov	al, 0b2h
	dec	dl
	out	dx, ax
	pop	dx
	pop	ax
	ret
$ativga endp

$everex proc	near		;Everex
	push	ax
	push	dx
	push	cx
	mov	cl, al
	mov	dx, 3c4h
	mov	al, 8
	out	dx, al
	inc	dl
	in	al, dx
	dec	dl
	shl	al, 1
	shr	cl, 1
	rcr	al, 1
	mov	ah, al
	mov	al, 8
	out	dx, ax
	mov	dl, 0cch
	in	al, dx
	mov	dl, 0c2h
	and	al, 0dfh
	shr	cl, 1
	jc	nob2
	or	al, 20h
nob2:	out	dx, al
	pop	cx
	pop	dx
	pop	ax
	ret
$everex endp

$aheada	proc	near
	push	ax
	push	dx
	push	cx
	mov	ch, al
        mov     dx, 3ceh	;Enable extended registers
        mov     ax, 200fh
        out     dx, ax
	mov	dl, 0cch	;bit 0
	in	al, dx
	mov	dl, 0c2h
	and	al, 11011111b
	shr	ch, 1
	jnc	@f
	or	al, 00100000b
@@:	out	dx, al
	mov	dl, 0cfh	;bits 1,2,3
	mov	al, 0
	out	dx, al
	inc	dx
	in	al, dx
	dec	dx
	and	al, 11111000b
	or	al, ch
	mov	ah, al
	mov	al, 0
	out	dx, ax
	pop	cx
	pop	dx
	pop	ax
	ret
$aheada	endp

$aheadb	proc	near
	push	ax
	push	dx
	push	cx
	mov	ch, al
        mov     dx, 3ceh	;Enable extended registers
        mov     ax, 200fh
        out     dx, ax
	mov	ah, ch
	mov	cl, 4
	shl	ah, cl
	or	ah, ch
	mov	al, 0dh
	out	dx, ax
	pop	cx
	pop	dx
	pop	ax
	ret
$aheadb	endp

;	The following variables are set by the 'setmode()' routine
;	(and perhaps adjusted by the 'setvesa()' routine that it may
;	call), and are subsequently used by the 'svgaline()' and
;	'truecolorline()' routines.   (BDT)

linelen	dw	0			; length of a scanline
bytesperline dw	0			; adapter bytes-per=scanline
videoseg dw	0			; adapter access segment
wingran	dw	0			; (we'll multiply the bank # by this)
vesa_usemode dw	0			; VESA mode we're (attempting to) use
true_cnt dw	0			; counter of bytes processed
true_type db	0			; true-color type flag:
					;   0 = not a truecolor mode
					;   1 = hicolor (5,5,5) style
					;   2 = XGA (5,6,5) style
					;   3 = 16M 24-bit (8,8,8) style
					;   4 = 16M 24-bit (8,8,8,rsvd 8) style

;	NOTE: Don't change the order of the variables below!
;	The VESA-detection and VESA-mode-query logic copies its
;	256-character VESA info blocks into the structures below,
;	and relies on the fact that any leftover data gets copied over
;	the top of variables whose value isn't important at the time!
;	                       (BDT)

; 	the first 20 bytes of the 256-byte VESA BIOS-info block

vesa_signature	dw	0,0		; should be "VESA"
vesa_version	dw	0		; VESA version number
vesa_oemptr	dd	0		; pointer to OEM string
vesa_capabilities db	0,0,0,0		; video capabilities bit-flags
vesa_modeslist	dd	0		; pointer to list of supported modes
vesa_totalmem	dw	0		; total adapter memory in 64K chunks

;	the first 40 bytes of the 256-byte VESA mode-info block

vesa_mode_info	dw	0		; mode info: attributes
vesa_winaattrib	db	0		; Win AA attribs 
vesa_winbattrib	db	0		; Win BB attribs 
vesa_wingran	dw	0		; window granularity
vesa_winsize	dw	0		; window size
vesa_winaseg	dw	0		; window AA segment
vesa_winbseg	dw	0		; window BB segment
vesa_funcptr	dd	0		; bank_switcher
vesa_bytespscan	dw	0		; bytes per scan line
vesa_xres	dw	0		; X-resolution
vesa_yres	dw	0		; Y-resolution
vesa_xcharsize	db	0		; X charsize
vesa_ycharsize	db	0		; Y charsize
vesa_numplanes	db	0		; number of planes
vesa_bitsppixel	db	0		; bits / pixel
vesa_numbanks	db	0		; number of banks
vesa_memmodel	db	0		; memory-model type
vesa_banksize	db	0		; bank size in KB
vesa_numpages	db	0		; number of complete images
vesa_rsvd	db	0		; reserved for page function
vesa_redsize	db	0		; red mask size
vesa_redpos	db	0		; red mask position
vesa_greensize	db	0		; green mask size
vesa_greenpos	db	0		; green mask position
vesa_bluesize	db	0		; blue mask size
vesa_bluepos	db	0		; blue mask position
vesa_rsvdsize	db	0		; reserved mask size
vesa_rsvdpos	db	0		; reserved mask position
vesa_directinfo	db	0		; direct-color-mode attributes

;	The following data gets filled in and used only by 'truecolorline()'

true_templine	db	32*4 dup(0)	; room for 32 pixels of 32-bit mode
		db	128  dup(0)	; fluff to fill the VESA 256-byte block

$vesa	proc	near
	push	ax
	push	bx
	push	cx
	push	dx
	mov	dx, ax
	mov	ax, 4f05h
	sub	bx, bx
	call	cs:[vesa_funcptr]	; bank-switch the fast way
	sti
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	ret
$vesa	endp

$nobank	proc	near
	ret
$nobank	endp

;
;	int vtype(void)
;
; Returns one of the following codes according to the video adapter found:
;
;  -1	MDA, Herc, or other unsupported adapter
;   0	CGA or 64k EGA
;   1	EGA
;   2	Standard VGA
;   3	256k SuperVGA
;   4	512k SuperVGA
;
; If 3 or 4 is returned, the variable "chipset" is set to one of these:
;
;   1	Cirrus
;   2	Headland (Video 7, New Dell)
;   3	Tseng Labs (Orchid, Genoa, Willow)
;   4	Paradise (Old Compaq, Dell)
;   5	Chips & Technologies
;   6	Trident
;   7	ATI VGA Wonder
;   8	Everex EVGA or Viewpoint
;   9	Ahead Systems "A"
;  10	Ahead Systems "B"
;  11	VESA
;

public	_vtype
_vtype	proc	far

	push	bp
	mov	bp, sp
	push	si
	push	ds
	push	di

	mov	ax, _DATA
	mov	ds, ax

	mov	[_memsize], 256

	sub	bx, bx
	mov	ax, 1A00h
	int	10h
	cmp	al, 1Ah
	je	couldbe
	jmp	notvga
couldbe:
	cmp	bl, 7
	jae	isvga
	jmp	notvga
isvga:
;	moved the VESA test to first place instead of last, 
;	and fixed up the VESA code - Bert Tyler
	xor	ax, ax
	cmp	ax, _vesadetect		;override vesa detection
	je	novesa	
vesa_test:
	push	cs			; Test for VESA
	pop	es
	mov	di, offset vesa_signature
	mov	ax, 4F00h
	int	10h
	cmp	ax, 004Fh		; VESA "OK" return
	jne	novesa
;					 look for the identifying "VESA"
	mov	ax, cs:vesa_signature
	cmp	ax, 4556h		; 'VE'
	jne	novesa
	mov	ax, cs:vesa_signature+2
	cmp	ax, 4153h		; 'SA'
	jne	novesa

	mov	[_chipset], 11
	mov	[bankadr], offset V_TEXT:$vesa
	mov	ax, cs:vesa_totalmem	; Available memory, in 64K units
	mov	cl, 6
	shl	ax, cl
	mov	[_memsize], ax
	mov	si, 3
	cmp	ax, 256			; Is it greater than 256K?
	jle	to_fini
	inc	si
to_fini:
	jmp	fini
novesa:
	mov	si, 3
	mov	ax, 0C000h
	mov	es, ax
	cmp	word ptr es:[40h], '13'
	jnz	noati
	mov	[_chipset], 7
	mov	[bankadr], offset V_TEXT:$ativga

	cli
	mov	dx, 1ceh
	mov	al, 0bbh
	out	dx, al
	inc	dl
	in	al, dx
	sti
	and	al, 20h
	jz	no512
	mov	[_memsize], 512
	inc	si
no512:
	jmp	fini
noati:
	mov	ax, 7000h		;Test for Everex
	xor	bx, bx
	cld
	int	10h
	cmp	al, 70h
	jnz	noev
	mov	[_chipset], 8
	mov	[bankadr], offset V_TEXT:$everex

	and	ch, 11000000b
	jz	@f
	inc	si
@@:	and	dx, 0fff0h
	cmp	dx, 6780h
	jz	yeste
	cmp	dx, 2360h
	jnz	note
yeste:
	mov	[_chipset], 6
	mov	[bankadr],offset V_TEXT:$trident
note:
	jmp	fini
noev:
	mov	dx, 3c4h		;Test for Trident
	mov	al, 0bh
	out	dx, al
	inc	dl
	in	al, dx
	cmp	al, 0fh
	ja	notri
	cmp	al, 2
	jb	notri

	mov	[_chipset], 6
	mov	[bankadr], offset V_TEXT:$trident
	inc	si
	jmp	fini
notri:
	mov	ax, 6f00h		;Test for Video 7
	xor	bx, bx
	cld
	int	10h
	cmp	bx, 'V7'
	jnz	nov7
	mov	[_chipset], 2
	mov	[bankadr], offset V_TEXT:$video7

	mov	ax, 6f07h
	cld
	int	10h
	and	ah, 7fh
	cmp	ah, 1
	jbe	gotchip
	inc	si
gotchip:
	jmp	fini
nov7:
	call	$cirrus			;Test for Cirrus
	cmp	[_chipset], 0
	jne	gotchip
	mov	dx, 3ceh		;Test for Paradise
	mov	al, 9			;check Bank switch register
	out	dx, al
	inc	dx
	in	al, dx
	dec	dx
	or	al, al
	jnz	nopara2

	mov	ax, 50fh		;turn off write protect on VGA registers
	out	dx, ax
	mov	dx, offset V_TEXT:$pdrsub
	mov	cx, 1
	call	$chkbk
	jc	nopara2			;if bank 0 and 1 same not paradise
	mov	[_chipset], 4
	mov	[bankadr], offset V_TEXT:$paradise

	inc	si
	jmp	fini
nopara2:
	mov	ax, 5f00h		;Test for Chips & Tech
	xor	bx, bx
	cld
	int	10h
	cmp	al, 5fh
	jnz	noct
	mov	[_chipset], 5
	mov	[bankadr], offset V_TEXT:$chipstech

	cmp	bh, 1
	jb	gotchip
	inc	si
	jmp	fini
noct:
	mov	dx, 03CCh
	in	al, dx
	shr	al, 1
	sbb	dl, dl
	and	dl, 020h
	add	dl, 0B4h	; Get CRTC address

	mov	al, 23h
	out	dx, al
	in	ax, dx
	mov	bl, ah
	or	ah, 083h
	out	dx, ax
	in	ax, dx
	xchg	ah, bl
	out	dx, ax
	test	ah, 003h
	jnz	maybe4
	and	bl, 083h
	cmp	bl, 083h
	jne	maybe4

	mov	[_chipset], 3	; We found an ET3000
	mov	[bankadr], offset V_TEXT:$tseng
	inc	si
	jmp	fini

maybe4:
	mov	al, 33h
	out	dx, al
	in	ax, dx
	mov	bl, ah
	not	ah
	out	dx, ax
	in	ax, dx
	not	ah
	out	dx, ax
	xor	bl, ah
	jz	is_4kw32
	cmp	bl, 0F0h
	jb	nots
	cmp	ah, 0F0h
	jb	nots
is_4kw32:
	mov	[_chipset], 3
	mov	[bankadr], offset V_TEXT:$tseng4
	inc	si
	jmp	fini
nots:
        mov     dx, 3ceh		;Test for Ahead A or B chipsets
        mov     ax, 200fh
        out     dx, ax
        inc	dx
        in      al, dx
	cmp	al, 21h
	jz	verb
	cmp	al, 20h
	jnz	noab
	mov	[_chipset], 9
	mov	[bankadr], offset V_TEXT:$aheada
	inc	si
	jmp	short fini
verb:
	mov	[_chipset], 10
	mov	[bankadr], offset V_TEXT:$aheadb
	inc	si
	jmp	short fini
noab:
nosvga:
	mov	ax, 2
	jmp	short vtout
fini:
	mov	ax, si
	jmp	short vtout
notvga:
	mov	ah, 12h
	mov	bl, 10h
	int	10h
	cmp	bl, 10h
	je	notega
	cmp	bl, 0
	jne	not64k
	sub	ax, ax
	jmp	short vtout
not64k:
	mov	ax, 1
	jmp	short vtout
notega:
	int	11h
	and	al, 30h
	cmp	al, 30h
	jne	cgaok
	mov	ax, -1
	jmp	short vtout
cgaok:
	sub	ax, ax
vtout:
	pop	di
	pop	ds
	pop	si
	pop	bp

	ret

_vtype	endp

;
;	newbank()
;

newbank	proc	near		;bank number is in AX
	push	ds

	push	ax
	mov	ax, _DATA
	mov	ds, ax
	pop	ax

	cli
	cmp	ax, [curbk]
	je	samebk
	mov	[curbk], ax
	push	dx			; adjust for non-64K granularity
	push	ax
	mul	cs:wingran
	pop	dx
	call	word ptr [bankadr]
	pop	ax
samebk:
	sti
	pop	ds
	ret
newbank	endp

nextbank proc	near
	push	ds

	push	ax
	mov	ax, _DATA
	mov	ds, ax
	pop	ax

	cli
	mov	ax, [curbk]
	inc	ax
	mov	[curbk], ax
	push	dx			; adjust for non-64K granularity
	push	ax
	mul	cs:wingran
	pop	dx
	call	word ptr [bankadr]
	pop	ax

	sti
	pop	ds
	ret
nextbank endp

waitforkey	proc	near
	push	ax
	push	dx
	mov	ah,2
	mov	dl,7
	int	21h
	mov	ah,07
	int	21h
	pop	dx
	pop	ax
	ret
waitforkey	endp

setvesa	proc	near

vesa_trynextmode:
	mov	cx, bx
	mov	cs:vesa_usemode,bx
	push	cs
	pop	es
	mov	di, offset vesa_mode_info
	mov	ax, 4F01h
	int	10h
	cmp	ah, 0
	jne	to_svnextmode

	mov	al,cs:vesa_bitsppixel
	cmp	al,32
	jne	vesa_not32
	mov	cs:true_type,4
	jmp	short vesa_ok1

to_svnextmode:
	jmp	svnextmode

vesa_not32:
	cmp	al,24
	jne	vesa_not24
	mov	cs:true_type,3
	jmp	short vesa_ok1
vesa_not24:
	cmp	al,16
	jne	vesa_ok1
	cmp	cs:vesa_redpos,11
	jne	vesa_notxga
	mov	cs:true_type,2
	jmp	short vesa_ok1
vesa_notxga:
	cmp	cs:vesa_redpos,10
	jne	vesa_ok1
	mov	cs:true_type,1
vesa_ok1:				;If none are satisfied, assume VBE 1.1

	mov	ax,cs:vesa_winaseg	; save the video segment
	mov	cs:videoseg,ax

	mov	ax,cs:vesa_bytespscan	; save the bytes / scanline
	mov	cs:bytesperline,ax

	mov	cx,cs:vesa_wingran	; calculate the bank # adjustment
	cmp	cx,0			; don't divide by 0!
	jne	skip_granfix
	mov	cx,1
skip_granfix:
	mov	ax,64
	div	cl
	mov	cs:wingran,ax		; we'll multiply the bank # by this

;	sanity checks: ensure that we can actually use this sucker
	mov	ax,cs:vesa_mode_info	; This mode must be supported,
	and	ax,01h
	cmp	ax,01h
	jne	svnextmode

	mov	al,cs:vesa_winaattrib	; Window AA must be supported
	and	al,05h			; and writable
	cmp	al,05h
	jne	svnextmode

	cmp	cs:vesa_winsize,64	; it must be a 64K window
	jne	svnextmode

	cmp	word ptr cs:vesa_funcptr,0	; it must have a direct bank-switch addr
	je	svnextmode

	cmp	cs:vesa_usemode, 110h	; are we trying for a "true-color" mode?
	jb	svgointomode
	cmp	cs:vesa_memmodel, 7	; yup, - it had better not be YUV
	je	svnextmode

svgointomode:
	mov	ax,4f02h
	int	10h
	cmp	ah,0
	je	svfok

svnextmode:					; try another VESA mode?
	cmp	cs:vesa_usemode, 110h
	jbe	svfail				; not if at/below 640x480x32K
	cmp	cs:vesa_usemode, 113h
	je	svfail				; not if at 800x600x32K
	cmp	cs:vesa_usemode, 116h
	je	svfail				; not if at 1024x768x32K
	cmp	cs:vesa_usemode, 119h
	je	svfail				; not if at 1280x1024x32K
	dec	cs:vesa_usemode			; try the next lower mode
	mov	bx, cs:vesa_usemode
	jmp	vesa_trynextmode

svfok:
	sub	ax, ax
	jmp	short svret
svfail:
	mov	ax, -1
svret:
	ret
setvesa	endp

;
;	int set_mode(int ax, int bx, int linelen);
;
public	_set_mode
_set_mode proc	far 

pax	equ	word ptr [bp+6]
pbx	equ	word ptr [bp+8]
pline	equ	word ptr [bp+10]

	push	bp
	mov	bp, sp
	push	ds
	push	si
	push	di

	mov	ax, _DATA
	mov	ds, ax
	mov	ax, -1
	mov	[curbk], ax

	mov	ax,pline		; save the line length
	mov	cs:linelen,ax
	mov	cs:true_type,0		; assume not a truecolor mode
	mov	cs:bytesperline,ax	; assume the adapter's bytes/line
	mov	ax,0a000h
	mov	cs:videoseg,ax		; assume the adapter's segment
	mov	ax,1
	mov	cs:wingran,ax		; assume a 64K granularity

	mov	ax, pax
	mov	bx, pbx

	cmp	ax, 0
	je	tweak
setbios:
	push	bp
	int	10h
	pop	bp
	jmp	svreturn

tweaktype dw	0

tweak:
	cmp	bx, 8
	jne	nottw8
sv360x480:
	mov	ax, 13h
	int	10h

	mov	dx, 3c4h
	mov	ax, 604h
	out	dx, ax

	mov	ax, 0f02h
	out	dx, ax

	mov	ax, 0a000h
	mov	es, ax
	sub	di, di
	mov	ax, di
	mov	cx, 21600
	rep	stosw

	mov	ax, 100h
	out	dx, ax
	mov	dx, 3c2h
	mov	al, 0e7h
	out	dx, al
	mov	dx, 3c4h
	mov	ax, 300h
	out	dx, ax

	mov	dx, 3d4h
	mov	al, 11h
	out	dx, al
	inc	dx
	in	al, dx
	and	al, 7fh
	out	dx, al
	dec	dx

	mov	ax, _DATA
	mov	ds, ax
	mov	si, offset _DATA:vptbl
	mov	cx, ((offset vpend)-(offset vptbl)) / 2
svl1:
	lodsw
	out	dx, ax
	loop	svl1
	jmp	svreturn
nottw8:
	cmp	bx, 10
	jne	nott10

	jmp	badmode		; Tweak 10 not currently supported
nott10:
	mov	si, [_chipset]
	dec	si
	shl	si, 1

	cmp	bx, 1
	jne	nott1
	mov	ax, ax1[si]
	mov	bx, bx1[si]
	jmp	svgaset
nott1:
	cmp	bx, 2
	jne	nott2
	mov	ax, ax2[si]
	mov	bx, bx2[si]
	jmp	svgaset
nott2:
	cmp	bx, 3
	jne	nott3
	mov	ax, ax3[si]
	mov	bx, bx3[si]
	jmp	svgaset
nott3:
	cmp	bx, 4
	jne	nott4
	mov	ax, ax4[si]
	mov	bx, bx4[si]
	jmp	svgaset
nott4:
	cmp	bx, 5
	jne	nott5
	mov	ax, ax5[si]
	mov	bx, bx5[si]
	jmp	svgaset
nott5:
	cmp	bx, 21
	jne	nott21
	mov	ax, ax21[si]
	mov	bx, bx21[si]
	mov	cs:true_type,2		; true-color mode
	jmp	svgaset
nott21:
	cmp	bx, 22
	jne	nott22
	mov	ax, ax22[si]
	mov	bx, bx22[si]
	mov	cs:true_type,2		; true-color mode
	jmp	svgaset
nott22:
	cmp	bx, 23
	jne	nott23
	mov	ax, ax23[si]
	mov	bx, bx23[si]
	mov	cs:true_type,2		; true-color mode
	jmp	svgaset
nott23:
	cmp	bx, 24
	jne	nott24
	mov	ax, ax24[si]
	mov	bx, bx24[si]
	mov	cs:true_type,2		; true-color mode
	jmp	svgaset
nott24:
	cmp	bx, 31
	jne	nott31
	mov	ax, ax31[si]
	mov	bx, bx31[si]
	mov	cs:true_type,3		; true-color mode
	jmp	svgaset
nott31:
	cmp	bx, 32
	jne	nott32
	mov	ax, ax32[si]
	mov	bx, bx32[si]
	mov	cs:true_type,3		; true-color mode
	jmp	svgaset
nott32:
	cmp	bx, 33
	jne	nott33
	mov	ax, ax33[si]
	mov	bx, bx33[si]
	mov	cs:true_type,3		; true-color mode
	jmp	svgaset
nott33:
	cmp	bx, 34
	jne	nott34
	mov	ax, ax34[si]
	mov	bx, bx34[si]
	mov	cs:true_type,3		; true-color mode
	jmp	svgaset
nott34:
	jmp	badmode
nottrue:
	jmp	setbios
svgaset:
	cmp	ax, 0
	jne	setok
	cmp	bx, 0
	je	badmode
setok:
	cmp	ax, 4F02h
	je	vesa_mode

	cmp	cs:true_type,0		; true-color mode?
	je	nottrue			;  nope.
	mov	cx,cs:bytesperline	;  yes - double bytes/line assumption
	add	cs:bytesperline,cx	;    (for the Sierra DAC)
	mov	cs:true_type,1		;    (assumes the Sierra DAC)
sierra_test:
;
;	Sierra DAC detection logic (if any is desired) goes here.
;	(Other non-VESA "true-color" tests would go here as well).
;	JMP to 'badmode' if not found, 'setbios' if found
;	Note that the values of AX and BX must be saved around
;	this logic, as we're already set up for the BIOS call!
;	(Change the 'videoseg', 'wingran', 'bytesperline' and
;	'true_type' variables here, too, if needs be.)

	jmp	setbios
vesa_mode:
	call	setvesa
	cmp	ax,0
	je	svreturn
badmode:
	mov	ax, -1
	jmp	short smout
svreturn:
	sub	ax, ax
smout:
	pop	di
	pop	si
	pop	ds
	pop	bp

	ret

_set_mode endp

;
;	int keypressed(void);
;

public	_keypressed
_keypressed proc far

	mov	ah, 1
	int	16h
	jnz	gotit
	sub	ax, ax
	jmp	short kpout
gotit:
	mov	ax, 1
kpout:
	ret

_keypressed endp

;
;	int getkey(void);
;

public	_getkey
_getkey proc	far

	mov	ah, 0
	int	16h
	cmp	al, 0
	je	special
	sub	ah, ah
	jmp	short keyout
special:
	mov	al, ah
	sub	ah, ah
	neg	ax
keyout:
	ret

_getkey endp

;
;	void vga1line(int line, U8 *indices);
;

public	_vga1line
_vga1line proc	far

line	equ	word ptr [bp+6]
indices equ	dword ptr [bp+8]

	push	bp
	mov	bp, sp
	push	ds
	push	si
	push	es
	push	di

	mov	ax, 0a000h
	mov	es, ax
	mov	ax, line
	mov	cx, 320
	mul	cx
	mov	di, ax
	lds	si, indices
	rep	movsb

	pop	di
	pop	es
	pop	si
	pop	ds
	pop	bp
	ret

_vga1line endp

;
;	void vga2line(int line, U8 *indices);
;

public	_vga2line
_vga2line proc	far

v_line	equ	word ptr [bp+6]
v_indices equ	dword ptr [bp+8]

	push	bp
	mov	bp, sp
	push	ds
	push	si
	push	es
	push	di

	lds	si, v_indices
	les	di, v_indices
	mov	cx, 40
	mov	bx, si
v2l1:
	lodsw
	stosb
	lodsw
	stosb
	lodsw
	stosb
	lodsw
	stosb
	movsb
	lodsw
	stosb
	lodsw
	stosb
	lodsw
	stosb
	movsb

	loop	v2l1
	mov	si, bx

	mov	cx, 90
v2l2:
	lodsb
	mov	byte ptr es:[di], al
	lodsb
	mov	byte ptr es:[di+90], al
	lodsb
	mov	byte ptr es:[di+180], al
	lodsb
	mov	byte ptr es:[di+270], al
	inc	di
	loop	v2l2

	mov	si, bx
	add	si, 360
	mov	ax, 0a000h
	mov	es, ax
	mov	ax, 90
	mul	v_line
	mov	bx, ax
	mov	dx, 3c4h

	mov	di, bx
	mov	cx, 45
	mov	ax, 0102h
	out	dx, ax
	rep	movsw

	mov	di, bx
	mov	cx, 45
	mov	ax, 0202h
	out	dx, ax
	rep	movsw

	mov	di, bx
	mov	cx, 45
	mov	ax, 0402h
	out	dx, ax
	rep	movsw

	mov	di, bx
	mov	cx, 45
	mov	ax, 0802h
	out	dx, ax
	rep	movsw

	pop	di
	pop	es
	pop	si
	pop	ds
	pop	bp
	ret

_vga2line endp

;
;	void svgaline(int line, U8 *indices);
;

public	_svgaline
_svgaline proc	far

s_line	equ	word ptr [bp+6]
s_indices equ	dword ptr [bp+8]

	push	bp
	mov	bp, sp
	push	ds
	push	si
	push	es
	push	di

	mov	es, cs:videoseg
	lds	si, s_indices

	mov	ax, s_line
	mul	cs:bytesperline

	mov	di, ax
	mov	ax, dx
	call	newbank

	mov	cx, cs:linelen
	add	di,cx
	sub	di,cx
	jnb	onebank1		; If borrow, line will wrap

	mov	cx, di
	push	cx
	neg	cx
	shr	cx, 1
	rep movsw			; Move the rest of this 64K page
	rol	cx, 1
	rep movsb
	pop	cx
	add	cx, cs:linelen
	call	nextbank

onebank1:
	shr	cx, 1
	rep movsw			; Move the remainder of the scanline
	rol	cx, 1
	rep movsb

	pop	di
	pop	es
	pop	si
	pop	ds
	pop	bp
	ret

_svgaline endp

;
;	void truecolorline( int line, U8 *indices);
;

public	_truecolorline
_truecolorline proc	far

t_line	equ	word ptr [bp+6]
t_indices equ	dword ptr [bp+8]

	push	bp
	mov	bp, sp
	push	ds
	push	si
	push	es
	push	di

	lds	si, t_indices			; pointer to the raw data
	xor	di, di				; pointer to the results line

	mov	cs:true_cnt, 0			; initialize out counter
	mov	es, cs:videoseg			; and video segment

	cmp	cs:true_type,1			; sierra-DAC (5,5,5) style?
	jne	notsierra
sierraloop:					; build the 5x5x5 output line
	mov	cx, 32				; in 32-pixel segments
	xor	di, di
sierraloop2:
	push	cx
	mov	bx, si
	mov	ah,ds:0[bx]			; red component
	and	ax,0f800h
	shr	ax,1
	add	bx,cs:linelen
	mov	dl,ds:0[bx]			; green component
	and	dx,0f8h
	mov	cl,2
	shl	dx,cl
	add	ax,dx
	add	bx,cs:linelen
	mov	dl,ds:0[bx]			; blue component
	and	dx,0f8h
	mov	cl,3
	shr	dx,cl
	add	ax,dx
	mov	word ptr cs:true_templine[di],ax
	add	di,2
	inc	si
	pop	cx
	loop	sierraloop2

	push	si
	mov	si, offset true_templine
	mov	ax, t_line
	mul	cs:bytesperline
	add	ax, cs:true_cnt
	adc	dx, 0

	mov	di, ax
	mov	ax, dx
	call	newbank

	mov	cx, 32*2
	cmp	di, -32*2
	jbe	@f
	mov	cx, di
	push	cx
	neg	cx
	shr	cx, 1
	rep movs word ptr [di], word ptr cs:[si]
	rol	cx, 1
	rep movs byte ptr [di], byte ptr cs:[si]
	pop	cx
	add	cx, 32*2
	call	nextbank
@@:
	shr	cx, 1
	rep movs word ptr [di], word ptr cs:[si]
	rol	cx, 1
	rep movs byte ptr [di], byte ptr cs:[si]

	pop	si
	mov	dx, cs:true_cnt
	add	dx, 32*2
	mov	cs:true_cnt, dx
	cmp	dx, cs:bytesperline
	jb	sierraloop

	jmp	truecolorexit

to_notxga:
	jmp	notxga
	
notsierra:
	cmp	cs:true_type,2			; XGA (5,6,5) style?
	jne	to_notxga
xgaloop:					; build the 5x6x5 output line
	mov	cx, 32				; in 32-pixel segments
	xor	di, di
xgaloop2:
	push	cx
	mov	bx, si
	mov	ah,ds:0[bx]			; red component
	and	ax,0f800h
	add	bx,cs:linelen
	mov	dl,ds:0[bx]			; green component
	and	dx,0fch
	mov	cl,3
	shl	dx,cl
	add	ax,dx
	add	bx,cs:linelen
	mov	dl,ds:0[bx]			; blue component
	and	dx,0f8h
	mov	cl,3
	shr	dx,cl
	add	ax,dx
	mov	word ptr cs:true_templine[di],ax
	add	di,2
	inc	si
	pop	cx
	loop	xgaloop2

	push	si
	mov	si, offset true_templine
	mov	ax, t_line
	mul	cs:bytesperline
	add	ax, cs:true_cnt
	adc	dx, 0

	mov	di, ax
	mov	ax, dx
	call	newbank

	mov	cx, 32*2
	cmp	di, -32*2
	jbe	@f
	mov	cx, di
	push	cx
	neg	cx
	shr	cx, 1
	rep movs word ptr [di], word ptr cs:[si]
	rol	cx, 1
	rep movs byte ptr [di], byte ptr cs:[si]
	pop	cx
	add	cx, 32*2
	call	nextbank
@@:
	shr	cx, 1
	rep movs word ptr [di], word ptr cs:[si]
	rol	cx, 1
	rep movs byte ptr [di], byte ptr cs:[si]

	pop	si
	mov	dx, cs:true_cnt
	add	dx, 32*2
	mov	cs:true_cnt, dx
	cmp	dx, cs:bytesperline
	jb	xgaloop

	jmp	truecolorexit

to_not24bit: jmp not24bit

notxga:
	cmp	cs:true_type,3			; 16M 24-bit (8,8,8) style?
	jne	to_not24bit
attloop:					; build the 8x8x8 output line
	mov	cx, 32				; in 32-pixel segments
	mov	dx, cs:linelen
	xor	di, di
att32l:
	mov	bx,si
	mov	al,ds:[bx]			; red component
	mov	cs:true_templine+2[di],al
	add	bx,dx
	mov	al,ds:[bx]			; green component
	mov	cs:true_templine+1[di],al
	add	bx,dx
	mov	al,ds:[bx]			; blue component
	mov	cs:true_templine[di],al
	cmp	cs:vesa_bluepos,16		; say, this isn't B/G/R?
	jne	@f
	mov	ah,cs:true_templine[di]		; (oops - it's B/G/R!)
	mov	al,cs:true_templine+2[di]	; ((reverse them!))
	mov	cs:true_templine[di],al
	mov	cs:true_templine+2[di],ah
@@:	add	di,3
	inc	si
	loop	att32l

	push	si
	mov	si, offset true_templine
	mov	ax, t_line
	mul	cs:bytesperline			; was in CX, needed there?
	add	ax, cs:true_cnt
	adc	dx, 0

	mov	di, ax
	mov	ax, dx
	call	newbank

	mov	cx, 32*3
	cmp	di, -32*3
	jbe	onebank7
	mov	cx, di
	push	cx
	neg	cx
	shr	cx, 1
	rep movs word ptr [di], word ptr cs:[si]
	rol	cx, 1
	rep movs byte ptr [di], byte ptr cs:[si]
	pop	cx
	add	cx, 32*3
	call	nextbank
onebank7:
	shr	cx, 1
	rep movs word ptr [di], word ptr cs:[si]
	rol	cx, 1
	rep movs byte ptr [di], byte ptr cs:[si]

	pop	si
	mov	dx, cs:true_cnt
	add	dx, 32*3
	mov	cs:true_cnt, dx
	cmp	dx, cs:bytesperline
	jae	truecolorexit
	jmp	attloop

not24bit:
	cmp	cs:true_type,4			; 16M 32-bit (8,8,8,8) style?
	jne	truecolorexit
trueloop:					; build the 8x8x8 output line
	mov	cx, 32				; in 32-pixel segments
	mov	dx, cs:linelen
	xor	di, di
true32l:
	mov	bx,si
	mov	al,ds:[bx]			; red component
	mov	cs:true_templine+2[di],al
	add	bx,dx
	mov	al,ds:[bx]			; green component
	mov	cs:true_templine+1[di],al
	add	bx,dx
	mov	al,ds:[bx]			; blue component
	mov	cs:true_templine[di],al
	cmp	cs:vesa_bluepos,24		; say, this isn't B/G/R?
	jne	@f
	mov	ah,cs:true_templine[di]		; (oops - it's B/G/R!)
	mov	al,cs:true_templine+2[di]	; ((reverse them!))
	mov	cs:true_templine[di],al
	mov	cs:true_templine+2[di],ah
@@:	add	di,4
	inc	si
	loop	true32l

	push	si

	mov	si, offset true_templine
	mov	ax, t_line
	mul	cs:bytesperline			; was in CX, needed there?
	add	ax, cs:true_cnt
	adc	dx, 0

	mov	di, ax
	mov	ax, dx
	call	newbank
	mov	cx, 32*2
	rep movs word ptr [di], word ptr cs:[si]

	pop	si
	mov	ax, cs:true_cnt
	add	ax, 32*4
	mov	cs:true_cnt, ax
	cmp	ax, cs:bytesperline
	jb	trueloop

truecolorexit:
	pop	di
	pop	es
	pop	si
	pop	ds
	pop	bp
	ret

_truecolorline endp

;
;	void cgaline(int line, U8 *gray);
;

public	_cgaline
_cgaline proc	 far

c_line	equ	word ptr [bp+6]
c_gray	equ	dword ptr [bp+8]

	push	bp
	mov	bp, sp
	sub	sp, 80	; bitmap = [bp-80]

	push	ds
	push	si
	push	es
	push	di

	lea	di, [bp-80]
	push	ss
	pop	es
	mov	cx, 40
	sub	ax, ax
	rep	stosw
	lea	bx, [bp-80]

	mov	dl, 80h
	lds	si, c_gray
	mov	ax, _DATA
	mov	es, ax
	mov	di, offset _DATA:bayer2
	mov	ax, c_line
	and	ax, 7
	shl	ax, 1
	shl	ax, 1
	shl	ax, 1
	add	di, ax
	mov	cx, 80
cltop:
	mov	al, byte ptr ds:[si]
	cmp	al, byte ptr es:[di]
	jbe	nodot1
	or	byte ptr ss:[bx], dl
nodot1:
	inc	di
	shr	dl, 1

	lodsb
	cmp	al, byte ptr es:[di]
	jbe	nodot2
	or	byte ptr ss:[bx], dl
nodot2:
	inc	di
	shr	dl, 1
	jnc	cltop

	sub	di, 8
	mov	dl, 80h
	inc	bx
	loop	cltop

	mov	dx, 0B800h
	mov	ax, c_line
	shr	ax, 1
	jnc	notodd
	add	dx, 200h
notodd:
	mov	es, dx
	mov	cx, 80
	mul	cx
	mov	di, ax

	lea	si, [bp-80]
	push	ss
	pop	ds
	mov	cx, 40
	rep	movsw

	pop	di
	pop	es
	pop	si
	pop	ds

	mov	sp, bp
	pop	bp
	ret

_cgaline endp

;
;	void egaline(int line, U8 *indices);
;

public	_egaline
_egaline proc	 far

e_line	equ	word ptr [bp+6]
e_indices equ	dword ptr [bp+8]

	push	bp
	mov	bp, sp
	sub	sp, 240

	push	ds
	push	si
	push	es
	push	di

	lea	di, [bp-240]
	push	ss
	pop	es
	mov	cx, 120
	sub	ax, ax
	rep	stosw
	lea	bx, [bp-240]

	mov	dl, 80h
	lds	si, e_indices
	mov	ax, _DATA
	mov	es, ax
	mov	di, offset _DATA:bayer2
	mov	ax, e_line
	and	ax, 7
	shl	ax, 1
	shl	ax, 1
	shl	ax, 1
	add	di, ax
	mov	cx, 80
eltop:
	mov	dh, byte ptr es:[di]
	lodsb
	cmp	al, dh
	jbe	nored
	or	byte ptr ss:[bx], dl
nored:
	mov	al, byte ptr ds:[si+639]
	cmp	al, dh
	jbe	nogreen
	or	byte ptr ss:[bx+80], dl
nogreen:
	mov	al, byte ptr ds:[si+1279]
	cmp	al, dh
	jbe	noblue
	or	byte ptr ss:[bx+160], dl
noblue:
	inc	di
	shr	dl, 1
	jnc	eltop

	sub	di, 8
	mov	dl, 80h
	inc	bx
	loop	eltop

	mov	dx, 0A000h
	mov	es, dx
	mov	ax, e_line
	mov	cx, 80
	mul	cx
	mov	bx, ax
	mov	di, bx

	push	ss
	pop	ds
	lea	si, [bp-240]

	mov	dx, 3C4h
	mov	ax, 0102h
	out	dx, ax

	mov	cx, 40
	rep	movsw

	mov	ax, 0202h
	out	dx, ax
	mov	di, bx
	mov	cx, 40
	rep	movsw

	mov	ax, 0402h
	out	dx, ax
	mov	di, bx
	mov	cx, 40
	rep	movsw

	mov	ax, 0802h
	out	dx, ax
	mov	ax, -1
	mov	di, bx
	mov	cx, 40
	rep	stosw

	pop	di
	pop	es
	pop	si
	pop	ds

	mov	sp, bp
	pop	bp
	ret

_egaline endp

;
;	void readdac(U8 *dac);
;

public	_readdac
_readdac proc	far

rd_array equ	dword ptr [bp+6]

	push	bp
	mov	bp, sp
	push	es

	les	dx, rd_array
	mov	ax, 1017h
	mov	bx, 0
	mov	cx, 256
	int	10h

	pop	es
	pop	bp
	ret

_readdac endp

;
;	void writedac(U8 *dac);
;

public	_writedac
_writedac proc	far

wd_array equ	dword ptr [bp+6]

	push	bp
	mov	bp, sp
	push	es

	les	dx, wd_array
	mov	ax, 1012h
	mov	bx, 0
	mov	cx, 256
	int	10h

	pop	es
	pop	bp
	ret

_writedac endp

;
;	void xlat(U8 *, U8 *, U8 *, int);
;

public	_xlat
_xlat	proc	far

source	equ	dword ptr [bp+6]
dest	equ	dword ptr [bp+10]
table	equ	dword ptr [bp+14]
count	equ	word ptr [bp+18]

	push	bp
	mov	bp, sp
	push	es
	push	di
	push	ds
	push	si

	lds	bx, table
	mov	dx, ds
	lds	si, source
	les	di, dest
	mov	cx, count
	mov	bp, ds
xtop:
	lodsb
	mov	ds, dx
	xlatb
	stosb
	mov	ds, bp
	loop	xtop

	pop	si
	pop	ds
	pop	di
	pop	es
	pop	bp
	ret

_xlat	endp

;
;	void xlatp(U8 *, U8 *, U8 *, int);
;

public	_xlatp
_xlatp	proc	far

xl_src	equ	dword ptr [bp+6]
xl_dest	equ	dword ptr [bp+10]
xl_tbl	equ	dword ptr [bp+14]
xl_cnt	equ	word ptr [bp+18]

	push	bp
	mov	bp, sp
	push	es
	push	di
	push	ds
	push	si

	lds	bx, xl_tbl
	mov	dx, ds
	lds	si, xl_src
	les	di, xl_dest
	mov	cx, xl_cnt
	mov	bp, ds
xptop:
	lodsb
	mov	ds, dx
	xlatb
	add	byte ptr es:[di], al
	inc	di
	mov	ds, bp
	loop	xptop

	pop	si
	pop	ds
	pop	di
	pop	es
	pop	bp
	ret

_xlatp	endp

;	Next needed for version 1.71 map function
;
;	U8 __nearest(U16 *);
;
	assume	ds:DGROUP, es:_DATA

public	__nearest
__nearest proc far

input	equ	[bp+6]

	push	bp
	mov	bp, sp
	push	ds
	push	si
	push	es
	push	di

	lds	si, input
	mov	ax, _DATA
	mov	es, ax
	lodsw
	and	al, 0F8h
	add	al, 4
	mov	byte ptr es:rsave, al
	lodsw
	and	al, 0F8h
	add	al, 4
	mov	byte ptr es:gsave, al
	lodsw
	and	al, 0F8h
	add	al, 4
	mov	byte ptr es:bsave, al

	mov	ax, 0ffffh
	mov	word ptr es:ldist, ax
	mov	word ptr es:ldist+2, ax

	mov	ax, DGROUP
	mov	ds, ax
	mov	cx, _palette
	mov	si, offset DGROUP:_gcmap
nl1:
	sub	ah, ah
	lodsb
	sub	ax, es:rsave
	imul	ax

	mov	bx, ax
	mov	di, dx
	add	bx, ax
	adc	di, dx
	add	bx, ax
	adc	di, dx

	sub	ah, ah
	mov	al, byte ptr [si+255]
	sub	ax, es:gsave
	imul	ax
	shl	ax, 1
	rcl	dx, 1
	shl	ax, 1
	rcl	dx, 1
	add	bx, ax
	adc	di, dx

	sub	ah, ah
	mov	al, byte ptr [si+511]
	sub	ax, es:bsave
	imul	ax
	shl	ax, 1
	rcl	dx, 1
	add	bx, ax
	adc	di, dx

	mov	ax, word ptr es:ldist+2
	cmp	ax, di
	jb	skip
	ja	copy

	mov	ax, word ptr es:ldist
	cmp	ax, bx
	jbe	skip
copy:
	mov	word ptr es:ldist, bx
	mov	word ptr es:ldist+2, di
	mov	word ptr es:result, cx
skip:
	loop	nl1

	mov	ax, _palette
	sub	ax, word ptr es:result
bmdone:
	pop	di
	pop	es
	pop	si
	pop	ds
	pop	bp
	ret
__nearest endp

;	Next needed for version 1.92 map function
;
;	U8 __nearest2(U8 *);
;
	assume	ds:DGROUP, es:_DATA
public	__nearest2
__nearest2 proc far

input	equ	[bp+6]

	push	bp
	mov	bp, sp
	push	ds
	push	si
	push	es
	push	di

	lds	si, input
	mov	ax, _DATA
	mov	es, ax
	lodsb
	mov	byte ptr es:rsave, al
	lodsb
	mov	byte ptr es:gsave, al
	lodsb
	mov	byte ptr es:bsave, al

	mov	ax, 0ffffh
	mov	word ptr es:ldist, ax
	mov	word ptr es:ldist+2, ax

	mov	ax, DGROUP
	mov	ds, ax
	mov	cx, _palette
	mov	si, offset DGROUP:_gcmap
nl2:
	sub	ah, ah
	lodsb
	sub	ax, es:rsave
	imul	ax

	mov	bx, ax
	mov	di, dx

	mov	ax, word ptr es:ldist+2
	cmp	ax, di
	jb	skip2
	ja	ok1

	mov	ax, word ptr es:ldist
	cmp	ax, bx
	jbe	skip2
ok1:
	sub	ah, ah
	mov	al, byte ptr [si+255]
	sub	ax, es:gsave
	imul	ax
	add	bx, ax
	adc	di, dx

	mov	ax, word ptr es:ldist+2
	cmp	ax, di
	jb	skip2
	ja	ok2

	mov	ax, word ptr es:ldist
	cmp	ax, bx
	jbe	skip2
ok2:

	sub	ah, ah
	mov	al, byte ptr [si+511]
	sub	ax, es:bsave
	imul	ax
	add	bx, ax
	adc	di, dx

;	cmp	di,0
;	jne	notzero
;	cmp	bx,0
;	je	bmdone3	
;notzero:
	mov	ax, word ptr es:ldist+2
	cmp	ax, di
	jb	skip2
	ja	copy2

	mov	ax, word ptr es:ldist
	cmp	ax, bx
	jbe	skip2
copy2:
	mov	word ptr es:result, cx
	mov	word ptr es:ldist, bx
	mov	word ptr es:ldist+2, di
skip2:
	loop	nl2

bmdone3:
	mov	ax, _palette
	sub	ax, word ptr es:result
bmdone2:
	pop	di
	pop	es
	pop	si
	pop	ds
	pop	bp
	ret
__nearest2 endp

V_TEXT	ends
	end
