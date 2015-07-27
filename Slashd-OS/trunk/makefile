#this is just so I can click the compile button in codeblocks



default:
	nasm -f bin make_image.asm -o slashd_image.img

make-all:
	make -C boot
	make -C kernel
	make
#make boot, then kernel, then make default(image)
