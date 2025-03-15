# server-client-pipes #

Code for creating inter-process communication among server and client replicating an eshop. The inter-process comunication is done through pipes (pipeline) in UNIX based system. 
The server and client code is implemented in one single file (server-client). 
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

