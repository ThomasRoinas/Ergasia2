#Makefile for server

#instructions for make: (command line / terminal)
# 1. type make 
# 2. type ./server
# 3. type make clean to remove the object files and the executable

all: server

server: server.o
	cc -o server server.o

server.o : server.c server.h
	cc -c -o server.o server.c

clean :
	rm *.o server
