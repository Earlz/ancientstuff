


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




