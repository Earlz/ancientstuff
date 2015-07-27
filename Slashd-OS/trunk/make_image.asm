[org 0]
sector1:
jmp word .bootcode
dd (1440*1024)
dw 1
dw 2
.bootcode:
incbin "boot/boot.bin"
times 510-($-sector1)  db 0
boot_signature: dw 0xaa55 ;boot signature

sector2:
;directory stuff...
times 512-($-sector2)  db 0

sector3:
;first file
.stype: db 0x0F
.sectors_used: dd 1
.filesize: dd 512
.attrib: dw 0
.name_size: db 5
.directory: dd 0
.fname: db "dd.d",0
.fdata:
db "Hello there Mr. World!",0

times 512-($-sector3) db 0

sector4:
;second file
.stype: db 0x0F
.sectors_used: dd 1
.filesize: dd 512
.attrib: dw 0
.name_size: db 6
.directory: dd 0
.fname: db "sfs.d",0
.fdata:
db "The second file, w00t!",0
db "The second file, w00t!",0
db "The second file, w00t!",0
times 512-($-sector4) db 0


sector5:
;third file- the kernel
.stype: db 0x0F
.sectors_used: dd 40
.filesize: dd (512*40)
.attrib: dw 0x8010
.name_size: db 13
.directory: dd 0
.fname: db "kernel16.bin",0
.fdata:
incbin "kernel/kernel16.bin"


end_image:

times (1440*1024)-($-sector1)  db 66

