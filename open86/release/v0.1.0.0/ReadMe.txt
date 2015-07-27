To build Open86 you just use makefile, if your using windows then you can
use makefile.win32, the only difference between them is the win32 one will build
and link a resource file

currently only windows and unix are supported

To load a file for emulation(currently no floppy emulation) you just have to open up
test_config.o86 and then change the "tester.bin" to what you need
and then open test_config.o86 with open86(or just drag it onto open86)

Open86 is still in alpha stage and is very unstable and limited!