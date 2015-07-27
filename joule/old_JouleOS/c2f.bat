
rem PARTCOPY bootf.bin 0 3 -f1 0
rem PARTCOPY bootf.bin 3E 1C2 -f1 3E
rem objcopy -O binary juleos32v2.bin
objcopy -O binary jouleos.bin
copy jouleos.bin B:\kernel.bin


