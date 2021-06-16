nasm -f elf64 asm/boot.asm -o builds/boot.o
nasm -f elf64 asm/multiboot_header.asm -o builds/multiboot_header.o
nasm -f elf64 asm/long_mode_init.asm -o builds/long_mode_init.o
nasm -f elf64 asm/crti.asm -o builds/crti.o
nasm -f elf64 asm/crtn.asm -o builds/crtn.o

gcc -c C/kernel.c -o builds/kernel.o
gcc -c C/terminal.c -o builds/terminal.o
gcc -c C/conversion.c -o builds/conversion.o
gcc -c C/osMath.c -o builds/osMath.o
gcc -c C/char.c -o builds/char.o
gcc -c C/string.c -o builds/string.o
gcc -c C/malloc.c -o builds/malloc.o

ld -n -o kernel.bin -T linker.ld builds/multiboot_header.o builds/boot.o builds/long_mode_init.o builds/crti.o builds/crtn.o builds/kernel.o builds/terminal.o builds/osMath.o builds/conversion.o builds/char.o builds/malloc.o builds/string.o

cp kernel.bin ../iso/boot/kernel.bin
cp linker.ld ../iso/boot/linker.ld

grub-mkrescue -d /usr/lib/grub/i386-pc -o os.iso ../iso