all: server

server: server.o
	cc -o server server.o

server.o : server.c eshop.h
	cc -c -o server.o server.c

clean :
	rm *.o server

