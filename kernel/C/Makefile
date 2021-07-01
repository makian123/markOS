SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))
TARGET := temp

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -c $(OBJS) -o $@

clean:
	rm -rf $(TARGET) *.o
