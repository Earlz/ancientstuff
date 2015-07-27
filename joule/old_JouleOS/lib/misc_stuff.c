//See COPYRIGHT.txt for info on copyright
#include <k_JouleOS.h>

unsigned short getmemsize() {
    unsigned char highmem, lowmem;
    unsigned short mem;
    /* need to stop ints before accessing the CMOS chip */
    stopints();

    /* write to port 0x70 with the CMOS register we want to read */
    /* 0x30 is the CMOS reg that hold the low byte of the mem count */
    outportb(0x70,0x30);

    /* read CMOS values from port 0x71 */
    lowmem = inportb(0x71);

    /* write to port 0x70 with the CMOS register we want to read */
    /* 0x31 is the CMOS reg that hold the high byte of the mem count */
    outportb(0x70,0x31);

    /* read CMOS values from port 0x71 */
    highmem = inportb(0x71);

    /* fix the low and high bytes into one value */
    mem = highmem;
    mem = mem<<8;
    mem += lowmem;
    mem+=1024;
    __asm("sti");
    return mem;
}




/*
unsigned char digits[] ={
	'0','1','2','3','4','5','6',
	'7','8','9'};


char isspace(char *txt){
	if (*txt==' '){return 1;}else{return 0;}
}


int atomax( const char * s )
{
    int rc = 0;
    char sign = '+';
    const char * x;
    while ( isspace( *s ) ) ++s;
    if ( *s == '+' ) ++s;
    else if ( *s == '-' ) sign = *(s++);
    while ( ( x = memchr( digits, *(s++), 10 ) ) != 0 )
    {
        rc = rc * 10 + ( x - *digits );
    }
    return ( sign == '+' ) ? rc : -rc;
}
*/

//limited to 32 bits starting at bit 0
/* these dont work
void set_bit(int nr, unsigned long * address)
{

	__asm__ ("btsl %1,%0"
		: "=m" (address)
		: "Ir" (nr));
}


void clear_bit(int nr,unsigned long * address)
{
	__asm__("btrl %1,%0"
		: "=m" (address)
		: "Ir" (nr));
}
*/
//-1 means no bit is free lsb is 0
int find_0_bit(unsigned int dw){
	int i=0;
	//if(dw&1==0){return 0;}
	//i++;
	startover:
	for (i=i;i<=31;i++){
		if((dw&(1<<i))>>i==0){return i;}
	}
	return -1;
}
int find_1_bit(unsigned int dw){
     int i;
	for (i=0;i<=31;i++){
		if(dw&(1<<i)==1){return i;}
	}
	return -1;
}
//gets whether a bit is set or not
char get_bit(int num,unsigned int dw){
	return dw&(1<<num);
}
unsigned int set_bit(unsigned int *data,unsigned int num){
	*data=*data|(1<<num);
}

extern volatile unsigned long timer_ticks;
void wait(unsigned long ms){
	timer4=ms;
	while(timer4!=0){}
}

unsigned int upwr(unsigned int num,unsigned int power){
	unsigned int i;
	if (power==1){return num;}
	if (power==0){return 1;}
	for (i=1;i<power;i++){
		num=num*num;
	}
	return num;
}


unsigned int pwr(unsigned int what,unsigned int power){
     unsigned int i;unsigned int rety;
     rety=what;
     if(power==0){return 1;}
     if(power==1){return what;}
     for(i=2;i==power;i++){
     	rety=rety*what;
     }
     return rety;
}



