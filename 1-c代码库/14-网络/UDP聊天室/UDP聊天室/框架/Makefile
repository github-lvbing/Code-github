CC=gcc
CFLAGS=-Wall -g -O2 -c
OBJ=server
OBJS=client

all: server client

$(OBJ):$(OBJ).o
	$(CC) $^ -o $@

$(OBJS):$(OBJS).o
	$(CC) $^ -o $@

$(OBJ).o:server.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJS).o:client.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY:
	clean

clean:
	$(RM) *.o $(OBJ) $(OBJS)
