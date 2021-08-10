all: shutdown gdt font

shutdown: shutdown.asm
	nasm -f elf64 shutdown.asm -o shutdown.o

gdt: gdt.asm
	nasm -f elf64 gdt.asm -o gdt.o

font: font.asm
	nasm -f elf64 font.asm -o font.o