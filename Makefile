CC = gcc
CFLAGS = -O2 -march=native -pipe -fno-omit-frame-pointer
LDFLAGS = 
LDLIBS = -lm

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

.phony: clean
clean:
	rm -f *.o main
