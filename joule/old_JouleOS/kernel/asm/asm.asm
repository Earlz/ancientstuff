;;;;See COPYRIGHT.txt for info on copyright
SECTION .text
GLOBAL _asm_start
_asm_start:
EXTERN _kstack
EXTERN __main
;call __main
;jmp 0x100008
mov esp,_kstack+4096
;set the stack right
mov eax,CR0
and eax,0x7FFFFFFF
mov CR0,eax
mov eax,0
mov CR3,eax
;hlt
push __main
ret
hlt

global _FarCall
_FarCall:
pop eax
pop ecx
mov DWORD [far_data],eax
;assumes you do all that segement and pusha stuff
;call FAR [far_data],ecx
pop ecx
pop eax
ret





global _gdt_flush     ; Allows the C code to link to this
extern _gp            ; Says that '_gp' is in another file
_gdt_flush:
    lgdt [_gp]        ; Load the GDT with our '_gp' which is a special pointer
    mov ax, 0x8      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss,ax
    jmp 0x10:flush2   ; 0x08 is the offset to our code segment: Far jump!
flush2:
    ret


global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31





;  0: Divide By Zero Exception
_isr0:
    cli
    push byte 0    ; A normal ISR stub that pops a dummy error code to keep a
                   ; uniform stack frame
    push byte 0
    jmp isr_common_stub

;  1: Debug Exception
_isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub
_isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub
_isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub
_isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub
_isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub
_isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub
_isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

            ; Fill in from 2 to 7 here!

;  8: Double Fault Exception (With Error Code!)
_isr8:
    cli
    push byte 8        ; Note that we DON'T push a value on the stack in this one!
                   ; It pushes one already! Use this type of stub for exceptions
                   ; that pop error codes!
    jmp isr_common_stub

_isr10:
    cli
    push byte 10        ; Note that we DON'T push a value on the stack in this one!
                   ; It pushes one already! Use this type of stub for exceptions
                   ; that pop error codes!
    jmp isr_common_stub
_isr11:
    cli
    push byte 11        ; Note that we DON'T push a value on the stack in this one!
                   ; It pushes one already! Use this type of stub for exceptions
                   ; that pop error codes!
    jmp isr_common_stub
_isr12:
    cli
    push byte 12       ; Note that we DON'T push a value on the stack in this one!
                   ; It pushes one already! Use this type of stub for exceptions
                   ; that pop error codes!
    jmp isr_common_stub
_isr13:
    cli
    push byte 13        ; Note that we DON'T push a value on the stack in this one!
                   ; It pushes one already! Use this type of stub for exceptions
                   ; that pop error codes!
    jmp isr_common_stub
_isr14:
    cli
    push byte 14        ; Note that we DON'T push a value on the stack in this one!
                   ; It pushes one already! Use this type of stub for exceptions
                   ; that pop error codes!
    jmp isr_common_stub

_isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub
_isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub
_isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub
_isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub
_isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub
_isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub
_isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub
_isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub
_isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub
_isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub
_isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub
_isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub
_isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub
_isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub
_isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub
_isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

_isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub

_isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

global _irqm0
global _irqm1
global _irqm2
global _irqm3
global _irqm4
global _irqm5
global _irqm6
global _irqm7


_irqm0:
     push byte 0
     push byte 32
     jmp irqm_common_stub

_irqm1:
     push byte 0
     push byte 33
     jmp irqm_common_stub
_irqm2:
     push byte 0
     push byte 34
     jmp irqm_common_stub
_irqm3:
     push byte 0
     push byte 35
     jmp irqm_common_stub
_irqm4:
     push byte 0
     push byte 36
     jmp irqm_common_stub
_irqm5:
     push byte 0
     push byte 37
     jmp irqm_common_stub
_irqm6:
     push byte 0
     push byte 38
     jmp irqm_common_stub
_irqm7:
     push byte 0
     push byte 39
     jmp irqm_common_stub

global _irqs8
global _irqs9
global _irqs10
global _irqs11
global _irqs12
global _irqs13
global _irqs14
global _irqs15


_irqs8:
     push byte 0
     push byte 40
     jmp irqs_common_stub

_irqs9:
     push byte 0
     push byte 41
     jmp irqs_common_stub

_irqs10:
     push byte 0
     push byte 42
     jmp irqs_common_stub

_irqs11:
     push byte 0
     push byte 43
     jmp irqs_common_stub

_irqs12:
     push byte 0
     push byte 44
     jmp irqs_common_stub

_irqs13:
     push byte 0
     push byte 45
     jmp irqs_common_stub

_irqs14:
     push byte 0
     push byte 46
     jmp irqs_common_stub

_irqs15:
     push byte 0
     push byte 47
     jmp irqs_common_stub


global __TaskInt

__TaskInt:
     push byte 0
     push byte 48
     jmp isr_common_stub


global __StartTasking
__StartTasking
     push byte 0
     push byte 100
     jmp isr_common_stub


global __AppInt51
__AppInt51:
     push byte 0
     push byte 0x51
     jmp isr_common_stub








; We call a C function in here. We need to let the assembler know
; that '_fault_handler' exists in another file
extern _fault_handler

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:

    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x8   ; Load the Kernel Data Segment descriptor!
    cli
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	mov ss,ax
    mov eax, esp   ; Push us the stack
    mov [_int_stack],esp
 	push eax ;for whatever reason
    mov eax, _fault_handler
    call eax   ; A special call, preserves the 'eip' register
	pop eax
	mov esp,[_int_stack]
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!


extern _int_stack


global _int_template
extern _int_catcher
_int_template: ;compiled once but edited and used as a template

     push byte 0 ;error code
     push byte 0 ;interrupt number
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x0   ; Load the Kernel Data Segment descriptor! -this is changed...
    cli
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	mov ss,ax
    mov eax, esp   ; Push us the stack
    mov [_int_stack],esp
 	push eax ;for whatever reason
    mov eax, _int_catcher ;this stays constant
    call eax   ; A special call, preserves the 'eip' register
	pop eax ;function dependent
	mov esp,[_int_stack]
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!


global _irqm_template
_irqm_template: ;compiled once but edited and used as a template
     push byte 0 ;error code
     push byte 0 ;interrupt number
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x0   ; Load the Kernel Data Segment descriptor! -this is changed at template time
    cli
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	mov ss,ax
    mov eax, esp   ; Push us the stack
     mov dword [_int_stack],esp
 	push eax ;for whatever reason

    mov eax, _int_catcher ;this stays constant
    call eax   ; A special call, preserves the 'eip' register

	pop eax
	mov esp,[_int_stack]
    pop gs
    pop fs
    pop es
    pop ds
    mov al,0x20 ;for master irq
     out 0x20, al
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

global _irqs_template
_irqs_template: ;compiled once but edited and used as a template

     push byte 0 ;error code
     push byte 0 ;interrupt number
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x0   ; Load the Kernel Data Segment descriptor! -this is changed at template time
    cli
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	mov ss,ax
    mov eax, esp   ; Push us the stack
    mov [_int_stack],esp
 	push eax ;for whatever reason
    mov eax, _int_catcher ;this stays constant
    call eax   ; A special call, preserves the 'eip' register
	pop eax
	mov esp,[_int_stack]
    pop gs
    pop fs
    pop es
    pop ds
    mov al,0x20
     out 0xA0, al
     out 0x20, al
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!






extern _SwitchIfNeeded
irqm_common_stub:

    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x8   ; Load the Kernel Data Segment descriptor!
    cli
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	mov ss,ax
    mov eax, esp   ; Push us the stack
    mov [_int_stack],esp
 	push eax ;for whatever reason
    mov eax, _fault_handler
    call eax   ; A special call, preserves the 'eip' register
	pop eax
	mov esp,[_int_stack]
    pop gs
    pop fs
    pop es
    pop ds
    mov al,0x20
     out 0x20, al
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    ; call _SwitchIfNeeded ;
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!


irqs_common_stub:

    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x8   ; Load the Kernel Data Segment descriptor!
    cli
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	mov ss,ax
    mov eax, esp   ; Push us the stack
    mov [_int_stack],esp
 	push eax ;for whatever reason
    mov eax, _fault_handler
    call eax   ; A special call, preserves the 'eip' register
	pop eax
	mov esp,[_int_stack]
    pop gs
    pop fs
    pop es
    pop ds
     mov al,0x20
     out 0xA0, al
     out 0x20, al
     popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

global _idt_load
extern _idtp
_idt_load:
    lidt [_idtp]
ret

global _StartTSS
_StartTSS:
     jmp 0x18:0
ret

global _testfunc2
_testfunc2:
cli
hlt ;halt here
ret


far_data:
dd 0





