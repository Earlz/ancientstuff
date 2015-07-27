cd source\
rem currently "localized" for my pc setup
tcc -1 -IC:\tc\include -LC:\tc\lib -mt -lt -f87 main.c 
cd \..\
bin\bootable bin\boot12.bin B:
copy source\main.com bin\main.com
del source\main.com
copy bin\main.com B:\startup.bin