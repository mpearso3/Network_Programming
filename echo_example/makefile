CC = gcc
CFLAGS = -Werror

all: echoclient echoserver

echoclient: client.o
	$(CC) $(CFLAGS) $< -o $@

echoserver: server.o
	$(CC) $(CFLAGS) $< -o $@

client.o: client.c
	$(CC) -c $<

server.o: server.c
	$(CC) -c $<

clean:
	rm echoclient echoserver
