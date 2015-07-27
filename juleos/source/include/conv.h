/*>>>>JuleOS<<<<*/
/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>Version 0.0.3 ALPHA<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>conversion.h; Contains a few bitlevel conversion functions<<<<*/

typedef struct{
	byte low;
	byte high;
}bytes;
typedef struct{ /*not currently used*/
	word low;
	word high;
}words;

word byte2word(bytes t) /*converts 2 bytes to 1 word*/
{
word retu;
byte tmp,tmp2;
tmp=t.low;
tmp2=t.high;
asm mov al,tmp
asm mov ah,tmp2
asm mov retu,ax
return retu;
}

bytes word2bytes(word w) /*converts 1 word to a high and low byte*/
{
	bytes t;
	byte tmp,tmp2;
	asm mov ax,w
	asm mov tmp,ah
	asm mov tmp2,al
	t.low=tmp2;
	t.high=tmp;
	return t;
}


	

	