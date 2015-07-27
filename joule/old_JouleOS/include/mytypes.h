//PUBLIC DOMAIN
#ifndef _MYTYPES_H
#define _MYTYPES_H


/* hardware datatypes */
typedef unsigned char BYTE;           /* 8-bit byte */
typedef unsigned short WORD;          /* 16-bit word */
typedef unsigned long DWORD;          /* 32-bit dword */
typedef unsigned long ADDR;           /* address that should not be deref'd */
//lowercase:
typedef signed char sbyte;
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;



/* integer types */
typedef unsigned char UINT8;          /* 8-bit unsigned integer */
typedef signed char INT8;             /* 8-bit signed integer */
typedef unsigned short UINT16;        /* 16-bit unsigned integer */
typedef signed short INT16;           /* 16-bit signed integer */
typedef unsigned long UINT32;         /* 32-bit unsigned integer */
typedef signed long INT32;            /* 32-bit signed integer */

/* logical datatypes */
typedef unsigned char CHAR;           /* ISO 8859-1 character */
typedef unsigned char *STRPTR;        /* C-style NUL-terminated string */
typedef enum { FALSE=0,TRUE=1 } BOOL; /* boolean value */

/* constants */
#ifndef NULL
#define NULL ((void *)0)
#endif /* NULL */

/* useful macros */
#define BITFIELD(name,width) unsigned int name : width
#define HIBYTE(x) ((BYTE)((x) >> 8))
#define LOBYTE(x) ((BYTE)((x) & 0xff))
#define ABS(x) ((x) < 0 ? -(x) : (x))    /* NB: multiple evaluations! */


#endif

