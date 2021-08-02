SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))
KERNEL := markOS.elf

.PHONY: all clean


all: $(KERNEL)

$(KERNEL): $(OBJS)
	gcc -Wall -ffreestanding -c $(OBJS) -o $@

clean:
	rm -rf $(KERNEL) $(OBJS)
