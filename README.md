# server-client-pipes #

Code for creating inter-process communication among server and client replicating an eshop. The inter-process comunication is done through pipes (pipeline) in a UNIX based system. 

The server and client code is implemented in one single file (server-client). The declaration of the functions and structs is done in the header file (server-client.h). Makefile contains the code for compiling and running the program with the source code and header file attached.
The server handles orders executed by the 5 clients. Each client performs 10 orders in total 

* Header File: __server-client.h__   
* Source Code: __server-client.c__    
* Makefile: __makefile__

## Makefile instructions ##
```
make server-client
```
```
./server-client   
```
```
make clean
``` 

