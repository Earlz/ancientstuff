Best viewed in courier new(for the tables)


General info:
JouleOS is a 32bit operating system designed to run from a floppy
JouleOS in kernel mode uses a flat address space and does not yet support multitasking
currently there is no gui
Also JouleOS is written in C with a bit of ASM




Compiling:
currently the makefile does not work, so you must have code::blocks to compile it

to run your own compiled version you need to get ahold of one of my floppy disk images(in /images/), copy it to your own floppy(or use an image loader) and from there just build, run c2f.bat and voila

also if you want to use a floppy disk besides B: go into c2f.bat and replace B:\... with A:\.... and you can change the JouleOS.bxrc


Using it:

JouleOS requires an i486 compatable cpu and at least 4mb of memory

JouleCon is a text based console and supports arguments and multiple commands per line
for arguments you type in something like this COMMAND;ARGUMENT
the ; is the seperator you can change it in the source by changing sarg in console.c

for multiple commands you can type something like  COMMAND1;ARGUMENT_FOR_COMMAND1;COMMAND2;ARGUMENT FOR COMMAND2;COMMAND3.....
and remember that spaces do not matter in JouleCon

JouleCon also now supports limited multiple consoles!
push the F6 key to goto the next console;


Command List:

help -no arguments- prints some help text -this is out of date right now
ver -no arguments- prints version info
info -no arguments- prints some info such as what the seperator character is
set info -not supported yet- not supported yet
cls -no arguments- clears the screen
set video mode -which mode- changes video mode; see fig. 1
terminal -no arguments- this is actually a basic terminal for communication over com1 --might now be broken with all the updates
list -no arguments- This will list the consoles



+--Figure 1--------------------+
|     Video modes:             |
|______________________________|                  
|number|width|height|font width|
+------+-----+------+----------+
| 0    | 90  | 60   | 8        |
| 1    | 80  | 25   | 16       |
| 2    | 90  | 30   | 16       |
| 3    | 80  | 50   | 8        |
| 4    | 40  | 50   | 8        |
| 5    | 40  | 25   | 16       |
| g1   | 320 | 240  |graphics  |
+------------------------------+





Extras:
if you think that JouleOS might have frozen up you can try pushing F4 which should work most of the time for unfreezing it
there is also a command called test and its arguments are t1, t2, t3, t4.....
you can try them if you want but I won't guarrentee they won't crash your pc
and I guess thats about it