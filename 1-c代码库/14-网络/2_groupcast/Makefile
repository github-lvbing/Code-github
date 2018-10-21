CC=gcc
CFGLASS=-Wall -g -c -O2

all:recv send

recv:recv.o
	$(CC) $< -o $@

send:send.o
	$(CC) $< -o $@

%.o:%.c
	$(CC) $(CFGLASS) $< -o $@

.PHONY:
	clean

clean:
	$(RM) -rf *.o a.out recv send
