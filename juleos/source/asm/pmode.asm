;this code switches to flat real mode or "unreal" mode
;only affects fs and gs for compatibiltiy purposes
;this code was modified for use with tasm
                jmp strt
         DESC386 STRUC
        limlo   dw      ?
        baselo  dw      ?
        basemid db      ?
        dpltype db      ?       ; p(1) dpl(2) s(1) type(4)
        limhi   db      ?       ; g(1) d/b(1) 0(1) avl(1) lim(4)
        basehi  db      ?
DESC386 ENDS
gdt     DESC386 <GDT_END - gdt - 1, gdt, 0, 0, 0, 0>  ; the GDT itself
        DESC386 <0ffffh, 0, 0, 091h, 0ffh, 0>   
        GDT_END:

        strt:



        ; first, calculate the linear address of GDT
        xor     edx,edx         ; clear edx
        xor     eax,eax         ; clear edx
        mov     dx,ds           ; get the data segment
        shl     edx,4           ; shift it over a bit
        add     dword ptr [gdt+2],edx   ; store as GDT linear base addr

        ; now load the GDT into the GDTR
        lgdt    fword ptr gdt   ; load GDT base (286-style 24-bit load)
        mov     bx,1 * size DESC386 ; point to first descriptor
        mov     eax,cr0         ; prepare to enter protected mode
        or      al,1            ; flip the PE bit
        cli                     ; turn off interrupts
        mov     cr0,eax         ; we're now in protected mode

        mov     fs,bx           ; load the FS segment register
        mov gs,bx
        and     al,0FEh         ; clear the PE bit again
        mov     cr0,eax         ; back to real mode
        sti                    ; resume handling interrupts
        ret                     ;
;----------------------------------------------------------------------
       ; 4G data segment
