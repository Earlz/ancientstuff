/*>>>>JuleOS<<<<*/
/*>>>>Main Programmer: Jordan Earls<<<<*/
/*>>>>Version 0.0.3 ALPHA<<<<*/
/*>>>>16-Bit Edition<<<<*/
/*>>>>Just another toy OS>Inspired by OpenSourceOS(OPOS)*/
/*>>>>Built Using Turbo C with Turbo ASM<<<<*/
/*>>>>Main.c; Initializations, defines, includes, and calls the shell<<<<*/
asm .486p
/*the asm .386 means to use real mode .386 instructions plus enable protected mode although it is not switched to-need them for flatreal mode init*/
#include "prepu.c"

int main()
{

signed int ret;

switch2flat();
int_init(); /*intitialize interrupt vectors*/
/*32 bit segments on the way, fs and gs are the only ones expanded
why only fs and gs you ask?
well they require a 80386 compiler and most <386 compilers  are not capable of handling the advanced segments
*/

	ret=SMain(0); /*call the shell main*/
/*add error checking for ret here*/

}
/*note: there are 9 full segments to use and probably last  2 will be reserved for caching and big vars, first 1 will be for juleos kernel, and the rest applications can fight over  plus there is a .98th of a segment at the end but not sure why it isnt a full segment*/
/*note: the current fdfs design has a limit of 4064 names including special entries*/


	