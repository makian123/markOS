all: boot crti crtn lmi mbh shutdown

boot: boot.asm
	nasm -f elf64 boot.asm -o boot.o

crti: crti.asm
	nasm -f elf64 crti.asm -o crti.o

crtn: crtn.asm
	nasm -f elf64 crtn.asm -o crtn.o

lmi: long_mode_init.asm
	nasm -f elf64 long_mode_init.asm -o long_mode_init.o

mbh: multiboot_header.asm
	nasm -f elf64 multiboot_header.asm -o multiboot_header.o

shutdown: shutdown.asm
	nasm -f elf64 shutdown.asm -o shutdown.o

