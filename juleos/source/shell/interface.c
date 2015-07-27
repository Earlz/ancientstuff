/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>Version 0.0.3 ALPHA<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>Shell\interface.c; Contains easy to use interfaces to rather hard to use C functions(user level, not development)<<<<*/

void Sload(void) /*Provides a simple interface to load a sector into memory; uses only "safe" sectors*/
{
	byte *inpt[10];
	byte sec[4];
	

printf("This loads safe sectors into memory");
nl();
printf("What sector number do you want to load: "); scanf(sec);
nl();
sec=filter1(sec);


}


byte filter1(byte num,byte len)
{
	byte counter;
	byte ret[len]; 
	while (counter<=len) {
		if (num=48)
	







/*48 0
49 1
50 2
51 3
52 4
53 5
54 6
55 7
56 8
57 9
*/