SRCC := $(wildcard C/*.o)
SRCA := $(wildcard asm/*.o)
TAR := kernel

$(TAR): $(SRCC) $(SRCA)
	ld -n -o kernel.elf -T linker.ld -nostdlib --no-dynamic-linker $(SRCC) $(SRCA)
	cp kernel.elf ../iso/boot/kernel.elf
	cp linker.ld ../iso/boot/kernelLinker.ld