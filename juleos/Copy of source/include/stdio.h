/*>>>>JuleOS<<<<*/
/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>Include/stdio.h; Includes remakes of some of the most common C Library functions, Stable as of Tuesday, August 16, 2005 5:34 PM(GMT -6),printf,input only, scanf needs redone<<<<*/
void printf(char*);
void printn(int);
/*FORMATTED IS NOT YET ALLOWED*/
/*
void printf2(char* txt)
{
	if (txt[1]='e') {
		bprint('1');
	}
	if (*txt[1]='e' {
		bprint('0');
	}
	while (*txt!=0)
{
    bprint(*txt);

    ++txt;
}
}
*/

void background(void){
	/*do stuff in the background while waiting for user to input command*/
}

char* input(char* txt)  /*working and way better than scanf, plus has background(), very expanable compared to scanf also returns just a pointer to buffer*/
{ 
	char tmp;
	byte going=1;
	keys returned;
	char* orig;
	byte safe=0; /*this is a filter for keys, should be set if the key is an asci character or if the key is controled*/
	orig=txt;
	
		while (going){
			
			while (bioskey(1)==0)
			{
				background();
				if (bioskey(1)>0){break;}
			}
			returned=getkey();
			
			*txt=returned.asc;
			if (*txt==0){safe=0;}else{safe=1;}
		switch(returned.scan)
		{
			case 14:
			if (txt==orig) {
			putcur(getcurx()-1,getcury());
			txt--;
			*txt=0;
			safe=1;
			}else{
			putcur(getcurx()-1,getcury());
			*txt=0;
			bprint2(' ',curx,cury);
			txt--;
			safe=0;
			}
			break;
		
			case 28:
			*txt=0; /*change to null*/
			 going=0;
			 safe=1;
			break;


			case 156:
			*txt=0; /*change to null*/
			 going=0;
			 safe=1;
			break;
			
			case 75: /*weird, under windows it says left arrow key is scancode #203 so im using: the scancode chart in \ect\scancode.gif */
			
			putcur(curx-2,cury);
			txt--;
			txt--;
			/*almost working*/
			safe=1;
			break;
			
			}
			if (safe==0) { /*if key pushed is a special key that has no asc code and is only a scancode*/
			*txt=0;
			txt--;
			putcur(getcurx()-1,getcury());
			}
			bprint2(*txt,getcurx(),getcury()); /*use the fast and better way to do text*/
			putcur(getcurx()+1,getcury());
		 txt++; /*advance text pointer*/
		 safe=0;
	 }
	 nw
	 return orig;
}



void printf(char* txt)  /*writes a string onto the screen, >need to add formatting support>/ must send a pointer*/
{
	byte tmp=0;
	while (*txt!=0) {
		if (*txt==12){putcur(curx,cury+1);}
		bprint2(*txt,curx,cury);
		putcur(curx+1,cury);
		++txt;
	}
}

void printa(byte x,byte y,char* txt){
	byte old[1];
	old[0]=curx;
	old[1]=cury;
	putcur(x,y);
	printf(txt);
	putcur(old[0],old[1]);
}

void printn(int n){
	char t[20];
	 int2str(n,&t,10);
           	printf(t);
}

byte set(int n,byte d){
		char t[20];
		char *p;
		printn(n);
	 int2str(n,&t,2);nw
	 printf(&t[0]);
	 p=&t;
	 p=p+0;nw
	 if (*p==1){return 1;}else{return 0;}
 }
	
int inputn(void)
{
	byte buf[50];
	byte* ptr;int ret;
	input(&buf);
	ptr=&buf;
	ret=str2int(ptr);
return ret;
}
/*
char* scanf2() 
{ 
     char* txt[2048]; 
     char* tmp; 
     char* trash; 
     int a,b,c; 
     while (tmp!=10) { 
          a++ 
         *txt[a]=tmp; 
          getkey(&tmp,&trash); /*add delete function
          bprint(tmp);
          if(tmp == 13)
          {
               return txt;
          }
     }
}
*/
/*must put these functions into an interrupt somehow*/
void trapme(byte scan) /*key trapping mechanism for system calls*/
{
if (scan==59) { printf("hmm");}



}

unsigned char* scanf(void)  /*gets a string from the keyboard, breaks on enter, need to add arrow and backspace control* -backup*/
{ 
     unsigned char txt[scanf_BUFFER]; 
	keys returned;
     u8b* tmp; 
     unsigned char* trash; 
     int a,b=1,c; 
     a=0;
	while (b==1) {
		
		 returned=getkey(); /*get a char from the keyboard and store it in *tmp*/
		txt[a]=returned.asc; /*adds the just pressed key to txt*/
		
		 bprint(txt[a]); /*echos what you just pressed*/
		if (txt[a]==0) { 
			a=a-1;
			putcur(getcurx()-1,getcury());
		}
		switch(returned.scan)
		{
			case 14:
			if (a==0) {
			bprint(':');
			a=1;
			}
			bprint('');
			putcur(getcurx()-1,getcury());
			txt[a]=0;
			a=a-2;
			break;
		
			case 28:
			txt[a]=0; /*change to null*/
			 b=0;
			break;


			case 156:
			txt[a]=0; /*change to null*/
			 b=0;
			break;
			}

		 a++; /*advance counter*/
	 }
	return txt;
}

/*
unsigned char* scanf(void)  /*gets a string from the keyboard, breaks on enter, need to add arrow and backspace control* -backup
{ 
     unsigned char txt[scanf_BUFFER]; 
	keys returned;
     u8b* tmp; 
     unsigned char* trash; 
     int a,b=1,c; 
     a=0;
	while (b==1) {
		
		 returned=getkey(); /*get a char from the keyboard and store it in *tmp*
		txt[a]=returned.asc; /*adds the just pressed key to txt*
				if (returned.scan==1) {
		reboot();
		}
		 bprint(txt[a]); /*echos what you just pressed*
		if (txt[a]==0) { 
			a=a-1;
			putcur(getcurx()-1,getcury());
			trapme(returned.scan);
		}

		if (returned.scan==14) {
			if (a==0) {
			bprint(':');
				a=1;
		}
			bprint('');
			putcur(getcurx()-1,getcury());
			txt[a]=0;
			a=a-2;
		}
		 if (returned.scan==28) { /*if the user pressed enter then break*
		 txt[a]=0; /*change to null*
		 b=0;
		break;
		 }
		if (returned.scan==156) {
		txt[a]=0; /*change to null*
		 b=0;
		}

		 a++; /*advance counter*
		 }
	return txt;
}
		 
/*		
unsigned char* scanf_BACKUP(void)  /*gets a string from the keyboard, breaks on enter, need to add arrow and backspace control* -depricated
{ 
     unsigned char txt[scanf_BUFFER]; 
     u8b* tmp; 
     unsigned char* trash; 
     int a,b=1,c; 
     a=0;
	while (b==1) {
		
		 *tmp=getkey1(); /*get a char from the keyboard and store it in *tmp*
		txt[a]=*tmp; /*adds the just pressed key to txt*
		 bprint(txt[a]); /*echos what you just pressed*
		 if (txt[a]==13) { /*if the user pressed enter then break*
		 txt[a]=0; /*change to null*
		 b=0;
		break;
		 }
		 a++; /*advance counter*
		 }
	return txt;
}
/*	
void scanf3(u8b pnt) 
{ 
     char* txt[2048]; 
     char* tmp; 
     char* trash; 
     int a,b=1,c; 
     while (b=1) 
     { 
          getkey(txt,&trash);
          txt[a]=*tmp; 
          bprint(*txt[a]); 
          if(txt[a] == 13) 
          { 
               break; 
          } 
          a++; 
     } 
     
}
*/
			