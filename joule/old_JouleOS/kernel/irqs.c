//See COPYRIGHT.txt for info on copyright
volatile BOOL fdd_done=0;


volatile unsigned long timer_ticks;
volatile unsigned long timer=0; //this decrements every tick
volatile unsigned long timer1=0; //usable
volatile unsigned long timer2=0; //usable
volatile unsigned long timer3=0; //usable
volatile unsigned long timer4=0; //for wait() only!
volatile unsigned long time_slice=0;
extern volatile BOOL multitasking;

void (*timer1_callback)(unsigned long timer_ticks);

//extern volatile CONSOLE *ProcessConsole;
extern volatile unsigned int TextUI;
volatile unsigned char no_draw=0;
regs r_saved_irq0;
void *target_ip_irq0;
unsigned short target_cs_irq0;
void CallbackMiddle();
void UseIntCallback(regs *r,void *target,unsigned short target_cs){
    //r_saved_irq0=*r; //save regs
    r->cs=0x10;
    r->eip=CallbackMiddle;

}

void CallbackMiddle(){


}


void irq0(regs *r) { /*overflow causes invalid opcode error, although that dont quite make sense*/
    /*timer*/
    int i;
    /* Increment our 'tick count' */
    timer_ticks++;
    if(timer1!=0){timer1--;UseIntCallback(r,timer1_callback,0x10);}
    if(timer2!=0){timer2--;}
    if(timer3!=0){timer3--;}
    if(timer4!=0){timer4--;}
    if(timer_ticks%30000){ //30 seconds
   // screensaver1();
    }
    if(timer_ticks%25==0){
    	RefreshConsole();
    }
    //breakpoint();
    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */


    //draw status --no worries about buffers, since this is draw last it topmost
    if(timer_ticks-timer==500){_putc('\\',CurrentConsole->width-1,CurrentConsole->height-1);}
    if(timer_ticks-timer==1000){_putc('|',CurrentConsole->width-1,CurrentConsole->height-1);}
    if(timer_ticks-timer==1500){_putc('/',CurrentConsole->width-1,CurrentConsole->height-1);}
    if(timer_ticks-timer==2000){_putc('-',CurrentConsole->width-1,CurrentConsole->height-1);timer=timer_ticks;}

    //update cursor
    i=((volatile)CurrentConsole->cury*(volatile)CurrentConsole->width)+(volatile)CurrentConsole->curx;
    outportb(0x3D4, 14);
    outportb(0x3D5, i >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, i);

    if((volatile)multitasking==1){
        //printf("c");
        TaskSchedule();
    }




    return;
}




void irq1(regs *r) {
    //w00t volatile works on mingw!!!!
    // k_printf("?");
     kbd_handler(r);
    return;
}
void irq2(regs *r) {

    return;
}
void irq3(regs *r) {

    return;
}
void irq4(regs *r) {
     ComPort1_Handler(r);
    return;
}
void irq5(regs *r) {

    return;
}



void irq6(regs *r) {
     FDD_Handler(r);
    return;
}
void irq7(regs *r) {

    return;
}


void irq8(regs *r) {

    return;
}

void irq9(regs *r) {

    return;
}

void irq10(regs *r) {

   return;
}

void irq11(regs *r) {

    return;
}



void irq12(regs *r) {

     mouse_handler();

     return;
}

void irq13(regs *r) {

    return;
}

void irq14(regs *r) {

    return;
}

void irq15(regs *r) {

    return;
}
