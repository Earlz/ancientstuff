//See COPYRIGHT.txt for info on copyright
#ifndef _STRING_H
//#define _STRING_H
#ifdef __cplusplus
extern("C"){
#endif


char *memcpy(char *dest, char *src, int count);
short *memsetw(short *dest, unsigned short val, int count);
int strlen(char *str);
char *memset(char *dest, unsigned char val, int count);
void *memmove(void *dst,void *src,unsigned int length);







#ifdef __cplusplus
}
#endif

#endif