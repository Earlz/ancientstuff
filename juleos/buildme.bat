
cd source\
@echo on
tcc -mt -lt -n..\bin main.c
cd ..
bin\bootable bin\boot12.bin B:
copy bin\main.com B:\startup.bin
pause
