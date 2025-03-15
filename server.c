// Code for the server-client 
//Both server and client are in the same file, so the code is for both the server and the client

// Libraries
#include <stdio.h>        
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

// Struct for the product
typedef struct                
{
    char description[100];    
    double price;             
    int item_count;           
    int aithmata;           
    int temaxia_sell;  
} product;

// Function to initialize the catalog
void init_catalog(product catalog[])    
{
    for(int i=0; i<20; i++)          
    {
        sprintf(catalog[i].description, "Product(%d)", i+1);   
        catalog[i].price = i*3;                  
        catalog[i].item_count = 2;               
        catalog[i].aithmata = 0;                
        catalog[i].temaxia_sell = 0;            
    }
}

// Function for the parent process handling the orders
void parent_orders(product catalog[], int p1[2], int p2[2], int *sum_parag, int *sum_succparag, int *sum_failparag, double *sum_price)
{            
    int i;
    
    // Close the end of the pipes that are not used
   close(p1[0]);                
   close(p2[1]);                

   // Loop for the 50 orders to be processed
   for(i=0; i<50; i++)          
   { 
        char buff[100];   // Buffer for the messages       
        int arithmos_prod;      

        // Read the product number from the pipe
        read(p2[0], &arithmos_prod, sizeof(arithmos_prod));       
        (*sum_parag) = (*sum_parag) + 1;                   
        catalog[arithmos_prod].aithmata++;                  

        // Check if the product is available
        if(catalog[arithmos_prod].item_count > 0)          
        {
            (*sum_succparag) = (*sum_succparag) + 1;       
            (*sum_price) = (*sum_price) + catalog[arithmos_prod].price;     
            catalog[arithmos_prod].item_count--;                            
            catalog[arithmos_prod].temaxia_sell++;                         
            // Write the message for updating the client about the order
            sprintf(buff, "Purchase complete, your total is %.2lf", catalog[arithmos_prod].price);   
            write (p1[1], buff, sizeof(buff));     // Write the message to the pipe                                                    
        }
        else  
        {   
           (*sum_failparag) = (*sum_failparag) + 1;    // Increase the number of failed orders
           write (p1[1], "Products unavailable, request failed", sizeof("Products unavailable, request failed"));  
        }
        // Sleep for 1 second before the next order
        sleep(1);   
    }  
    // Close the ends of the pipes that were used      
    close(p1[1]);   
    close(p2[0]);  
}

// Function for the child processes executing the orders
void child_orders(int p1[2], int p2[2], int client_arithmos)
{
    int i;
    int arithmos_prod;      

    // Close the ends of the pipes that are not used
    close(p1[1]);         
    close(p2[0]);         

    // Seed for the random number generator
    srand(time(NULL));    

    for(i=0; i<10; i++)         
    {
        // Generate a random product number
        arithmos_prod = rand() % 20; 

        // Write the product number of the order to the pipe
        write(p2[1], &arithmos_prod, sizeof(arithmos_prod));  

        char buf[100];   // Buffer for the messages                     
        read(p1[0], buf, sizeof(buf));       // Read the response from the server about thr order placed    

        printf("Client %d: %s\n", client_arithmos, buf);  

        sleep(1);       // Sleep for 1 second before the next order
    }
    // Close the ends of the pipes that were used
    close(p1[0]);    
    close(p2[1]);    

    exit(0);  // Exit the child process    
}

// Function for the report
void report(product catalog[])
{
    int i;

    for(i=0; i<20; i++)   // Loop for the 20 products
    {
        printf("\nPerigrafi Proiontos %d: %s\n", i+1, catalog[i].description);  
        printf("Aithmata gia agora: %d\n", catalog[i].aithmata);                
        printf("Temaxia Agorastikan: %d\n", catalog[i].temaxia_sell);           
    }
}

// Function for the statistics
void statistics(int sum_parag, int sum_succparag, int sum_failparag, double sum_price)
{
    printf("\nSunolikos arithmos paraggeliwn: %d\n", sum_parag);   
    printf("Epituxhmenes Paraggelies: %d\n", sum_succparag);       
    printf("Apotuxhmenes Paraggelies: %d\n", sum_failparag);       
    printf("Sunoliko kostos: %.2lf\n", sum_price);                 
}

// Main function
int main()
{    
    // Initialize the catalog
    product catalog[20];    
    init_catalog(catalog);  

    int i;
    int p1[2];   // Pipe 1    
    int p2[2];   // Pipe 2  

    // Variables for the statistics set to 0
    int sum_parag = 0;          
    int sum_succparag = 0;      
    int sum_failparag = 0;      
    double sum_price = 0;       
    
    // Create the pipes
    if(pipe(p1) == -1)   
    {
        perror("Error in pipe1\n");  
        return -1;                   
    }

    if(pipe(p2) == -1)   
    {
        perror("Error in pipe2\n");   
        return -1;                    
    }
    
    // Create the child processes
    for(i=0; i<5; i++)     // Loop for the 5 child processes (5 clients)  
    {
        pid_t pid = fork();     

        if(pid < 0)         
        {
            perror("Error in fork\n");   
            return -1;
        }

        else if(pid == 0)    
        {                   
            child_orders(p1, p2, i+1);   
        }                                
    }
    // Parent process
    parent_orders(catalog, p1, p2, &sum_parag, &sum_succparag, &sum_failparag, &sum_price);   

    // Wait for the child processes to finish
    for(i=0; i<5; i++)   
    {
        wait(NULL);      
    }

    // call the report and statistics functions
    report(catalog);   
    statistics(sum_parag, sum_succparag, sum_failparag, sum_price);  

    return 0;   // Exit the program
}