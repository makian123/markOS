KERNEL := kernel.bin
SOURCEDIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

CC = gcc
CPP = g++
AS = nasm

CFLAGS = -O3 -pipe
CPPFLAGS = -O3 -pipe
ASMFLAGS = 

INTERNALLDFLAGS :=            \
	-fno-pic -fpie        \
	-Wl,-static,-pie,--no-dynamic-linker,-ztext \
	-static-pie           \
	-nostdlib             \
	-T$(SOURCEDIR)/linker.ld    \
	-z max-page-size=0x1000

INTERNALCFLAGS  :=            \
	-I$(SOURCEDIR)        \
	-std=gnu11            \
	-ffreestanding        \
	-fno-stack-protector  \
	-fno-pic -fpie        \
	-mno-80387            \
	-mno-mmx              \
	-mno-3dnow            \
	-mno-red-zone		  \
	-mno-sse             \
	-mno-sse2            

INTERNALCPPFLAGS  :=          \
	-I$(SOURCEDIR)        \
	-std=c++14            \
	-ffreestanding        \
	-fno-stack-protector  \
	-fno-pic -fpie        \
	-mno-80387            \
	-mno-mmx              \
	-mno-3dnow            \
	-mno-red-zone		  \
	-mno-sse             \
	-mno-sse2            

ASMFILES = $(shell find $(SOURCEDIR)/ -type f -name '*.asm')
CPPFILES = $(shell find $(SOURCEDIR)/ -type f -name '*.cpp')
CFILES = $(shell find $(SOURCEDIR)/ -type f -name '*.c')
OBJ = $(CFILES:.c=.o)
OBJ += $(CPPFILES:.cpp=.o)
OBJ += $(ASMFILES:.asm=_asm.o)

.PHONY: all

all: $(KERNEL) iso clean
	@echo Done!

bios: $(KERNEL) iso clean
	@echo Done!

$(KERNEL): $(OBJ)
	@$(CC) $(INTERNALLDFLAGS) $(OBJ) -o $@

%.o: %.c
	@$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

%.o: %.cpp
	@$(CPP) $(CPPFLAGS) $(INTERNALCPPFLAGS) -c $< -o $@

%_asm.o: %.asm
	@$(AS) $(ASMFLAGS) $^ -f elf64 -o $@

iso:
	@cp $(KERNEL) $(SOURCEDIR)/../limine.cfg $(SOURCEDIR)/../limine/limine.sys \
		$(SOURCEDIR)/../limine/limine-cd.bin $(SOURCEDIR)/../limine/limine-eltorito-efi.bin $(SOURCEDIR)/../iso_root/

	@xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-eltorito-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(SOURCEDIR)/../iso_root -o $(SOURCEDIR)/../image.iso > /dev/null

	@$(SOURCEDIR)/../limine/limine-install $(SOURCEDIR)/../image.iso > /dev/null

clean:
	@rm -rf $(KERNEL) $(OBJ) $(SOURCEDIR)/../iso_root/*
