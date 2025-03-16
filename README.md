# server-client-pipes #

Code for creating inter-process communication (IPC) among server and client replicating an e-shop process. The inter-process comunication is done through pipes (pipeline) in a UNIX based system. 

>[!NOTE]
>The server and client code is implemented in one single file (server-client.c). The declaration of the functions and structs is done in the header file (server-client.h). Makefile contains the code for compiling and running the program with the source code and header file attached.

## Code explanation ##

The details for each product is declared in the structure __product__. The __init_catalog__ function is used for initializing an array containing 20 products and setting each detail to store a value. 

For handling the incoming orders the __parent_orders()__ function recieves orders (50) from the clients through pipe read() checks if the product selected is in stock and sends an update to the client, write() pipe, about their purchase. The amount of orders __sum_parag__ increases by 1, the request for the specific product is also increased by 1 and depending on whether the ordered product is in stock the successful orders increases (__sum_succparag__), in the total cost (__sum_price__) the price of the selected product is added, the product stock (__item.count__) decreases by 1 and the amount of the products sales (__temaxia.sell__) is increased. 

The __child_orders()__ function accomodates the process for sumbiting an order. Each client performs 10 orders (loops 10 times for each order) sending a request for a product to the server (__parent_orders__) by writing to the pipe, containing the specific product number (__arithmos_prod__). The next process is recieving the update message from the server, through pipe read(), about the order status. Then the client name (plus his corresponding number (__client_arithmos__)) next to the status message of the purchase is shown on the screen. 

After the completions of all the 50 orders the function __report()__ prints the description, requests and total sales of each product ans the function __statistics()__ prints total number of orders, the amount of succesful and unsuccesful orders and the total cost.

Lastly, int the __main()__ function the product catalog array is intialized, the 2 pipes __p1, p2__, for the inter-process communication between the server and the clients, are created, the fork() system call is then called 5 times creating 5 different child processes (5 clients). Each child process (client) calls the __child_orders()__ function to execute orders. After each child has finished the parent process calls the __parent_orders()__ function to handle the orders. After the completion of each child process, the __report()__ and __statistics()__ functions are called and the program exits considering everything went well.

Comments in the code of each file is also provided for better understanding of the contents.

[!NOTE]
>Another version of the code implemented with sockets is availiable [here](https://github.com/ThomasRoinas/server-client-sockets)

## Mandatory files to run the program ##

- __server-client.h__   
- __server-client.c__    
- __makefile__

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

