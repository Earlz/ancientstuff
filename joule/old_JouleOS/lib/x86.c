/******
PUBLIC DOMAIN
This file has been made by me and can be used with no limitations or liscense
This is meant to make C able to do things like load the gdt fully(including setting segments)
This is only 32bit instructions and is just the simple base commands

//almost all of these will be inline
*/

inline unsigned char inportb(unsigned int port) {
    unsigned char ret;

asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
    return ret;
}
inline void outportb(unsigned int port,unsigned char value) {
asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}

/* Stop Interrupts */
inline void stopints() {
    asm ("cli");
}
inline void startints() {
	asm("sti");
}
unsigned int x86_tmpi; //these are because symbols are not recorded for the stack
unsigned short x86_tmps;
unsigned char x86_tmpc;
void *x86_tmpp;

//segments
inline void SetDS(unsigned short seg){
     x86_tmps=seg;
     __asm(".intel_syntax noprefix\n"
     "mov ds,[_x86_tmps]\n"
     ".att_syntax\n");
}

inline void SetES(unsigned short seg){
     x86_tmps=seg;
     __asm(".intel_syntax noprefix\n"
     "mov es,[_x86_tmps]\n"
     ".att_syntax\n");
}
inline void SetSS(unsigned short seg){
     x86_tmps=seg;
     __asm(".intel_syntax noprefix\n"
     "mov ss,[_x86_tmps]\n"
     ".att_syntax\n");
}
inline void SetGS(unsigned short seg){
     x86_tmps=seg;
     __asm(".intel_syntax noprefix\n"
     "mov gs,[_x86_tmps]\n"
     ".att_syntax\n");
}
inline void SetFS(unsigned short seg){
     x86_tmps=seg;
     __asm(".intel_syntax noprefix\n"
     "mov fs,[_x86_tmps]\n"
     ".att_syntax\n");
}

inline void SetCS(unsigned short seg,void *where){
     x86_tmps=seg;
     x86_tmpi=(unsigned int)where;
     __asm(".intel_syntax noprefix\n"
     "push ss\n"
     "push esp\n"
     "pushf\n"
     "push [_x86_tmps]\n"
     "push [_x86_tmpi]\n"
     "iret\n"
     ".att_syntax\n");
}

//set cs not sure how to do and it be useful

inline unsigned short GetDS(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmps],ds\n"
	".att_syntax\n");
	return x86_tmps;
}
inline unsigned short GetES(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmps],es\n"
	".att_syntax\n");
	return x86_tmps;
}
inline unsigned short GetFS(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmps],fs\n"
	".att_syntax\n");
	return x86_tmps;
}
inline unsigned short GetGS(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmps],gs\n"
	".att_syntax\n");
	return x86_tmps;
}
inline unsigned short GetSS(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmps],ss\n"
	".att_syntax\n");
	return x86_tmps;
}
inline unsigned short GetCS(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmps],cs\n"
	".att_syntax\n");
	return x86_tmps;
}

//stack

inline void Push(unsigned int val){
	x86_tmpi=val;
     __asm(".intel_syntax noprefix\n"
	"push [_x86_tmpi]\n"
	".att_syntax\n");
}

inline unsigned int Pop(){
     __asm(".intel_syntax noprefix\n"
	"pop [_x86_tmpi]\n"
	".att_syntax\n");
	return x86_tmpi;
}



//control regs
inline unsigned int GetCR0(){
     __asm(".intel_syntax noprefix\n"
     "push eax\n"
	"mov eax,cr0\n"
	"mov [_x86_tmpi],eax\n"
	"pop eax\n"
	".att_syntax\n");
	return x86_tmpi;
}
inline unsigned int GetCR1(){
     __asm(".intel_syntax noprefix\n"
     "push eax\n"
	"mov eax,cr1\n"
	"mov [_x86_tmpi],eax\n"
	"pop eax\n"
	".att_syntax\n");
	return x86_tmpi;
}
inline unsigned int GetCR2(){
     __asm(".intel_syntax noprefix\n"
     "push eax\n"
	"mov eax,cr2\n"
	"mov [_x86_tmpi],eax\n"
	"pop eax\n"
	".att_syntax\n");
	return x86_tmpi;
}
inline unsigned int GetCR3(){
     __asm(".intel_syntax noprefix\n"
     "push eax\n"
	"mov eax,cr3\n"
	"mov [_x86_tmpi],eax\n"
	"pop eax\n"
	".att_syntax\n");
	return x86_tmpi;
}
inline unsigned int GetCR4(){
     __asm(".intel_syntax noprefix\n"
     "push eax\n"
	"mov eax,cr4\n"
	"mov [_x86_tmpi],eax\n"
	"pop eax\n"
	".att_syntax\n");
	return x86_tmpi;
}

inline void SetCR0(unsigned int val){
	x86_tmpi=val;
     __asm(".intel_syntax noprefix\n"
     "push eax\n"
     "mov eax,[_x86_tmpi]\n"
	"mov cr0,eax\n"
	"pop eax\n"
	".att_syntax\n");
}

inline void SetCR1(unsigned int val){
	x86_tmpi=val;
     __asm(".intel_syntax noprefix\n"
     "push eax\n"
     "mov eax,[_x86_tmpi]\n"
	"mov cr1,eax\n"
	"pop eax\n"
	".att_syntax\n");
}
inline void SetCR2(unsigned int val){
	x86_tmpi=val;
     __asm(".intel_syntax noprefix\n"
     "push eax\n"
     "mov eax,[_x86_tmpi]\n"
	"mov cr2,eax\n"
	"pop eax\n"
	".att_syntax\n");
}
inline void SetCR3(unsigned int val){
	x86_tmpi=val;
     __asm(".intel_syntax noprefix\n"
     "push eax\n"
     "mov eax,[_x86_tmpi]\n"
	"mov cr3,eax\n"
	"pop eax\n"
	".att_syntax\n");
}
inline void SetCR4(unsigned int val){
	x86_tmpi=val;
     __asm(".intel_syntax noprefix\n"
     "push eax\n"
     "mov eax,[_x86_tmpi]\n"
	"mov cr4,eax\n"
	"pop eax\n"
	".att_syntax\n");
}


//Lxxx commands such as lgdt

inline void LGDT(void *ptr){
	x86_tmpp=ptr;
     __asm(".intel_syntax noprefix\n"
     "lgdt [_x86_tmpp]\n"
	".att_syntax\n");
}

inline void LLDT(void *ptr){
	x86_tmpp=ptr;
     __asm(".intel_syntax noprefix\n"
     "lldt [_x86_tmpp]\n"
	".att_syntax\n");
}
inline void LTR(unsigned short selector){
	x86_tmps=selector;
     __asm(".intel_syntax noprefix\n"
     "mov dx,_x86_tmps\n"
     "ltr [_x86_tmps]\n"
	".att_syntax\n");
}
inline void LIDT(void *ptr){
	x86_tmpp=ptr;
     __asm(".intel_syntax noprefix\n"
     "lidt [[_x86_tmpp]]\n"
	".att_syntax\n");
}



//general register control

inline void SetEAX(unsigned int val){
	x86_tmpi=val;
	__asm(".intel_syntax noprefix\n"
	"mov eax,[_x86_tmpi]\n"
	".att_syntax\n");
}

inline void SetEBX(unsigned int val){
	x86_tmpi=val;
	__asm(".intel_syntax noprefix\n"
	"mov ebx,[_x86_tmpi]\n"
	".att_syntax\n");
}

inline void SetECX(unsigned int val){
	x86_tmpi=val;
	__asm(".intel_syntax noprefix\n"
	"mov ecx,[_x86_tmpi]\n"
	".att_syntax\n");
}

inline void SetEDX(unsigned int val){
	x86_tmpi=val;
	__asm(".intel_syntax noprefix\n"
	"mov edx,[_x86_tmpi]\n"
	".att_syntax\n");
}

inline void SetESP(unsigned int val){
	x86_tmpi=val;
	__asm(".intel_syntax noprefix\n"
	"mov esp,[_x86_tmpi]\n"
	".att_syntax\n");
}

inline void SetEBP(unsigned int val){
	x86_tmpi=val;
	__asm(".intel_syntax noprefix\n"
	"mov ebp,[_x86_tmpi]\n"
	".att_syntax\n");
}

inline void SetESI(unsigned int val){
	x86_tmpi=val;
	__asm(".intel_syntax noprefix\n"
	"mov esi,[_x86_tmpi]\n"
	".att_syntax\n");
}

inline void SetEDI(unsigned int val){
	x86_tmpi=val;
	__asm(".intel_syntax noprefix\n"
	"mov edi,[_x86_tmpi]\n"
	".att_syntax\n");
}




inline unsigned int GetEAX(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmpi],eax\n"
	".att_syntax\n");
	return x86_tmpi;
}

inline unsigned int GetEDI(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmpi],edi\n"
	".att_syntax\n");
	return x86_tmpi;
}

inline unsigned int GetEBX(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmpi],ebx\n"
	".att_syntax\n");
	return x86_tmpi;
}
inline unsigned int GetECX(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmpi],ecx\n"
	".att_syntax\n");
	return x86_tmpi;
}
inline unsigned int GetEDX(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmpi],edx\n"
	".att_syntax\n");
	return x86_tmpi;
}
inline unsigned int GetESI(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmpi],esi\n"
	".att_syntax\n");
	return x86_tmpi;
}
inline unsigned int GetEBP(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmpi],ebp\n"
	".att_syntax\n");
	return x86_tmpi;
}
inline unsigned int GetESP(){
	__asm(".intel_syntax noprefix\n"
	"mov [_x86_tmpi],esp\n"
	".att_syntax\n");
	return x86_tmpi;
}




inline unsigned int GetEFLAGS(){
	__asm(".intel_syntax noprefix\n"
	"pushfd\n"
	"pop [_x86_tmpi]\n"
	".att_syntax\n");
	return x86_tmpi;
}




//ect commands
inline void HLT(){
	asm volatile("hlt");
}



//these are for editing other stacks
unsigned int *other_stack; //esp
unsigned short other_ss;
unsigned int other_extra;
unsigned int *GetOtherStack(){
     return other_stack;
}

void SetOtherStack(unsigned int *stack){
     other_stack=stack;
}
unsigned short GetOtherSS(){
     return other_ss;
}
void SetOtherSS(unsigned short _ss){
     other_ss=_ss;
}


unsigned int *PushOther(unsigned int dat){

}

unsigned int PopOther(){
     unsigned int tmp;
     tmp=*other_stack;
     other_stack++;
     return tmp;
}




















