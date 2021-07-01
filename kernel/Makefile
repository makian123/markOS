SRCC := $(wildcard C/*.o)
SRCA := $(wildcard asm/*.o)
TAR := kernel

$(TAR): $(SRCC) $(SRCA)
	ld -n -o kernel.bin -T linker.ld $(SRCC) $(SRCA)
	cp kernel.bin ../iso/boot/kernel.bin
	cp linker.ld ../iso/boot/linker.ld
	grub-mkrescue -d /usr/lib/grub/i386-pc -o os.iso ../iso