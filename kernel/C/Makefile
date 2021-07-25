SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))
CFLAGS := -Wall -Wextra -pipe
KERNEL := markOS.elf

.PHONY: all clean


all: $(KERNEL)

$(KERNEL): $(OBJS)
	gcc -c $(OBJS) -o $@

clean:
	rm -rf $(KERNEL) $(OBJS)
