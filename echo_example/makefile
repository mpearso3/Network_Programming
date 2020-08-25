CC = gcc

all: echoclient echoserver

echoclient: client.o
	$(CC) $< -o $@

echoserver: server.o
	$(CC) $< -o $@

client.o: client.c
	$(CC) -c $<

server.o: server.c
	$(CC) -c $<

clean:
	rm echoclient echoserver
