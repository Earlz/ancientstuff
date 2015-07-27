@echo off
echo This will install JuleOS 0.1.0 ALPHA onto your floppy disk in A:
echo please make sure you do not need this floppy as it
echo is not guarenteed it will still work after running it
echo all this does is copy main.com as startup.bin
echo and writes boot12.bin as the bootsector
echo this should keep your fat12 filesystem intact
echo writes to A: and the fat12 format on it MUST be valid
pause
bootable bin\boot12.bin A:
copy bin\main.com A:\startup.bin