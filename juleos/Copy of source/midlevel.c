/*>>>>JuleOS<<<<*/
/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>Version 0.0.3 ALPHA<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>MidLevel.c; Includes higher level functions using none or very little inline asm, This has a high dependency on LowLevel.c<<<<*/
void printf(char*);
 u16b strlen(u8b *s1) /*gets the length of a string; copied from opos*/
 {	u16b counter =0;
	while(*s1)
	{	counter++;
		s1++;
	}
	return counter;
 }

 u8b streq(char *s1,char *s2) /*returns 1 if the strings are alike, copied from opos*/
 {	if(strlen(s1)==strlen(s2))
	{	int i=0;
		for(;i<strlen(s1);++i)
		{	if(s1[i]!=s2[i])
				return 0;
		}
		return 1;

	}
	return 0;
}
/*
void line2(u8b x1,u8b y1,u8b x2,u8b y2,u8b color) /*draws a line to the screen, currently wip*
{
	u8b tmp;
	u8b tmpx,tmpy;
	
tmpx=x1;
tmpy=y1;
tmp=1;
	if (x1<x2) {
		if (y1<y2) {
			while (tmp) {
				putdot(color,tmpx,tmpy);
				if (tmpx<x2){
				tmpx++;
			}
			if (tmpy<y2)  {
				tmpy++;
			}
			if (tmpy=y2,tmpx=x2) {
			tmp=0;
		}
		}
	}
}
}	
*/			





void box(word l,word w,word x,word y,byte color) /*draws a box to the screen, currently wip*/
{
	word tmpx,tmpy,i,t;
	
	tmpx=x;
	tmpy=y;
	t=1;
	while (t=1) {
		for (i=1;i==w;i+1) {
			putdot(tmpx+i,tmpy,color);
		}
		if (tmpy==y+l){
			t=0;
		}
		++tmpy;
	}
}

void DrawBox(word left, word top,word right,word bottom,byte color) /*draws a box but not sure where it came from...*/
{
  word tmpx,tmpy;
  for(tmpx= left; tmpx > (right - left); tmpx++)
  {
    for(tmpy = top;tmpy > (bottom - top); tmpy++)
    {
      putdot(tmpx, tmpy,color);
    }
  }
}
	
void nl() /*positions cursor with a new line*/
{
	putcur(0,getcury()+1);
}	

	/*
	very unstable code, works and cls's but for screws up ds and can make random jmps to functions and labels*
	asm        MOV     AX, 40h
	asm push ds
       asm MOV     DS, AX  
        asm MOV     AH, 06h 
       asm MOV     AL, 0  
       asm MOV     BH, 07  
       asm MOV     CH, 0  
       asm MOV     CL, 0   
       asm MOV     DI, 84h 
       asm MOV     DH, [DI] 
       asm MOV     DI, 4Ah
       asm MOV     DL, [DI]
       asm DEC     DL      
       asm INT     10h 
asm pop ds
*/



 void cls (void)
 {	/* use the scroll function to clear the screen */
	asm include asm\clsa.asm
 }





void int2str2(int value)
{
          int radix = 10;
          char buffer[20];
            /*  *strptr = itoa(value,buffer,radix);*/
}

word nextseg(void)
{
	word cseg;
	word ret;
	asm mov bx,cs
	asm mov cseg,bx
	ret=cseg+0xfff;
	return ret;
}





byte iseven(word value){
	byte ret;
	word tmp;
	tmp=value%2;
	if (tmp>0){ret=0;}else{ret=1;}
	return ret;
}
