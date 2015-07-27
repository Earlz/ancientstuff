//See COPYRIGHT.txt for info on copyright

/* You will need to code these up yourself!  */
char *memcpy(char *dest, char *src, int count)
{	int tmp=0;
	while(tmp<=count){
		*dest=*src;
		tmp++;dest++;src++;
	}
	return dest;
    /* Add code here to copy 'count' bytes of data from 'src' to
    *  'dest', finally return 'dest' */
}

char *memset(char *dest, unsigned char val, int count)
{	int tmp=0;
	while(tmp<=count){
		*dest=val;
		dest++;tmp++;
	}
	return dest;
    /* Add code here to set 'count' bytes in 'dest' to 'val'.
    *  Again, return 'dest' */
}

short *_memsetw(short *dest, unsigned short val, int count)
{	int tmp=0;
	while(tmp<=count){
		*dest=val;
		dest++;tmp++;
	}
	return dest;
    /* Same as above, but this time, we're working with a 16-bit
    *  'val' and dest pointer. Your code can be an exact copy of
    *  the above, provided that your local variables if any, are
    *  unsigned short */
}

int strlen(char *str)
{	int retr=0;
	while(*str!=0){
		retr++;
	}
	return retr;
    /* This loops through character array 'str', returning how
    *  many characters it needs to check before it finds a 0.
    *  In simple words, it returns the length in bytes of a string */
}

unsigned char _strcmp(char *str1,char* str2){
	for(;;){

		if (*str1!=*str2){return 0;}
		if (*str1==0 && *str2==0){return 1;}
		if(*str1==0 || *str2==0){return 0;}
		str1++;str2++;
	}

}

// int_to_ascii only converts digits lower 10

char int_to_ascii(int number)
{
	char ascii;
	if (number > 9) return 0;
	ascii = number + 48;
	return ascii;
}




unsigned int str2num(char *str){
	unsigned int num=0,i=0;
	unsigned int len;
	len=strlen(str);
	len--;
	while (*str!=0){
     if (*str==0){return num;}
     //if(*str=='0'){num=num+upwr(10,len);}else{
     num=num+((*str-48)*upwr(10,len));
     //}
     i++;str++;len--;
	}
	putc(num);
	return num;
}




unsigned int _replacec(char *str,char search,char replace){
     unsigned int counter=0;

	while (*str!=0){
		if(*str==search){*str=replace;counter++;}
		str++;
	}
	return counter;
}


unsigned char *num2bstr(unsigned int num,unsigned char *buffer){ //requires 33 bytes and converts to binary
     signed int i,j=31;
     for(i=0;i<=31;i++){
     	if((num&(1<<i))==0){buffer[j]='0';
     	}else{
     		buffer[j]='1';
     	}
     	j--;
     }
     buffer[32]=0; //null terminate
     return buffer;
}





/*
int atomax( const char * s )
{
    int rc = 0;
    char sign = '+';
    const char * x;
    while ( isspace( *s ) ) ++s;
    if ( *s == '+' ) ++s;
    else if ( *s == '-' ) sign = *(s++);
    while ( ( x = memchr( _PDCLIB_digits, *(s++), 10 ) ) != NULL )
    {
        rc = rc * 10 + ( x - _PDCLIB_digits );
    }
    return ( sign == '+' ) ? rc : -rc;
}


*/
/*
void str_cat (char *str1,char *str2)
{
	char *str_save;
	int i=0;
	int s=0;
	do{
		str_save[i] = str1[i];
		i++;
	}while (str1[i] != '\0');

	do{
		str_save[i] = str2[s];
		s++;
		i++;
	}while (str2[s] != '\0');

	str_save[i] = '\0';
	i=0;
	do{
		str1[i] = str_save[i];
		i++;
	}while (str_save[i] = '\0');
	str1[i] = '\0';
}
	*/




void *memmove(void *dst, void *src,unsigned int length){
	unsigned char *tmp,*tmp2;unsigned int i;

	tmp=dst;
	tmp2=src;
	for (i=0;i<=length;i++){
		*tmp=*tmp2;
		tmp2++;tmp++;
	}
	return dst;
}


