CC = gcc
CFLAGS = -g #-Wall
LDFLAGS = -g
LDLIBS = -lm

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

main: $(OBJS)

.phony: clean
clean:
	rm -f *.o main