//See COPYRIGHT.txt for info on copyright

void int0h(struct regs *r) { /*i prefer avoiding asm stubs*/
    __asm("cli"); /*disable interrupts to avoid an irq*/

    k_printf("\nException 0x0; Divide by zero error,halted");
    for(;;) {}
    return;
}
void int1h(regs *r) {
    char buffer[16];
    __asm("cli");
    k_printf("\nException 0x1; Debug Breakpoint, halted\n");
    k_printf("Error occured at eip: 0x%x",r->eip);
    //_ultoa(r->eip,buffer,10);
    //k_printf(buffer);
    for(;;){}
    return;
}

void int2h(regs *r) {
    __asm("cli");
    k_printf("\nException 0x2; Non Maskable Interrupt, halted");
    for(;;) {}
    return;
}
void int3h(regs *r) {
    __asm("cli");
    k_printf("\nException 0x3; Breakpoint interrupt, halted");
    k_printf("Error occured at eip: 0x%x",r->eip);
    for(;;) {}
    return;
}
void int4h(regs *r) {
    __asm("cli");
    k_printf("\nException 0x4; Into detected overflow, halted");
    for(;;) {}
    return;
}
void int5h(struct regs *r) {
    __asm("cli");
    k_printf("\nException 0x5; Out of bounds error, halted");
    for(;;) {}
    return;
}
void int6h(regs *r) {
     unsigned int *t;
    __asm("cli");
    k_printf("\nException 0x6; Invalid opcode error, halted\n");
    t=r->eip;
    k_printf("occured at eip: %x\nByte is: %x ",r->eip,*t);

    for(;;) {}
    return;
}
void int7h(struct regs *r) {
    __asm("cli");
    k_printf("\nException 0x7; No coproccessor error, halted");
    for(;;) {}
    return;
}
void int8h(struct regs *r) {
    __asm("cli");
    SetESP(0x200); //set it to have a valid stack -- at this point print the message, no resume
    k_printf("\nException 0x8; Double fault error, halted");
    for(;;) {}
    return;
}
void int9h(struct regs *r) {
    __asm("cli");
    k_printf("\nException 0x9; Coprocessor segment overrun error, halted");
    for(;;) {}
    return;
}
void int10h(struct regs *r) {
    __asm("cli");
    k_printf("\nException 0xA; Bad TSS error, halted");
    for(;;) {}
    return;
}
void int11h(struct regs *r) {
    __asm("cli");
    k_printf("\nException 0xB; Segment not present errror, halted");

    for(;;) {}
    return;
}
void int12h(regs *r) {
    __asm("cli");
    k_printf("\nException 0xC; Stack Fault error, halted");
    k_printf("\nESP: 0x%x SS: 0x%x",r->esp,r->ss);
    for(;;) {}
    return;
}
void int13h(regs *r) {
     unsigned char *blah;
   __asm("cli");
   blah=r->eip;
    k_printf("\nException 0xD; General protection fault, halted\n");
    k_printf("Error code: 0x%x\n",r->err_code);
    k_printf("opcode: 0x%x --",*blah);
    switch(*blah){
         case 0x8E:
         k_printf("error moving to segment reg.\n");
         break;
         case 0x1F:
         k_printf("error popping to DS\n");
         break;
         case 0x07:
         k_printf("error popping to ES\n");
         break;
         case 0x17:
         k_printf("error popping to SS\n");
         break;
    }


    for(;;) {}
    return;
}
volatile unsigned int blah_tmp;
void int14h( regs *r) {
    __asm("cli");
    k_printf("\nException 0xE; Page fault error, halted\n");
  //  uint2str (t1.err_code,&buffer,8);
     __asm(".intel_syntax noprefix\n"
     "mov eax,CR2\n"
     "mov [_blah_tmp],eax\n"
     ".att_syntax\n");
    k_printf("CR2: 0x%x  \nEIP: 0x%x",blah_tmp,r->eip);

    for(;;) {}
    return;
}
void int15h(struct regs *r) {
    __asm("cli");
    k_printf("\nException 0xF; Unknown interrupt error, halted");
    for(;;) {}
    return;
}
void int16h(struct regs *r) {
    __asm("cli");
    k_printf("\nException 0x10; Coprocessor Fault, halted");
    for(;;) {}
    return;
}
void int17h(struct regs *r) {
    __asm("cli");
    k_printf("\nException 0x11; Alignment check error, halted");
    for(;;) {}
    return;
}

void int18h(struct regs *r) {
    __asm("cli");
    k_printf("\nException 0x12; Machine check error, halted");
    for(;;) {}
    return;
}
void int19_31h(struct regs *r) {
    __asm("cli");
    k_printf("\nException 19-31; Reserved interrupt(no info) , halted");
    for(;;) {}
    return;
}

void unknown_int(struct regs *r) {
    __asm("cli");
    k_printf("\nUnknown interrupt, halted");
    for(;;)
        ;
    return;
}