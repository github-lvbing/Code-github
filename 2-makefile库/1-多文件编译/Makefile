CC = gcc
CFLAGS = -c -Wall -O2
OBJS = main

$(OBJS):$(OBJS).o add.o sub.o
	$(CC) $^ -o $@

$(OBJS).o:main.c
	$(CC) $(CFLAGS) $< -o $@

add.o:add.c
	$(CC) $(CFLAGS) $< -o $@

sub.o:sub.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY:
	clear

clear:
	rm *.o $(OBJS)
