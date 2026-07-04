CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

all: strace_0 strace_1 strace_2 strace_3

strace_0: strace_0.c
	$(CC) $(CFLAGS) -o strace_0 strace_0.c

strace_1: strace_1.c syscalls.h
	$(CC) $(CFLAGS) -o strace_1 strace_1.c

strace_2: strace_2.c syscalls.h
	$(CC) $(CFLAGS) -o strace_2 strace_2.c

strace_3: strace_3.c syscalls.h
	$(CC) $(CFLAGS) -o strace_3 strace_3.c

clean:
	rm -f strace_0 strace_1 strace_2 strace_3

.PHONY: all clean
