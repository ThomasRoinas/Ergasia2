#include <stdio.h>      
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

typedef struct 
{
    char description[100];
    double price;
    int item_count;
    int aithmata;
    int temaxia_sell;
} product;

void init_catalog(product catalog[])    
{
    for(int i=0; i<20; i++)
    {
        sprintf(catalog[i].description, "Product(%d)", i+1);
        catalog[i].price = i;
        catalog[i].item_count = 2;
        catalog[i].aithmata = 0;
        catalog[i].temaxia_sell = 0;
    }
}

void parent_orders(product catalog[], int p1[], int p2[], int *sum_parag, int *sum_succparag, int *sum_failparag, double *sum_price)
{
    int i;

   close(p1[0]);
   close(p2[1]);

   for(i=0; i<50; i++)
   { 
        char buff[100];
        int arithmos_prod;

        read(p2[0], &arithmos_prod, sizeof(arithmos_prod));
        (*sum_parag) = (*sum_parag) + 1;
        catalog[arithmos_prod].aithmata++;

        if(catalog[arithmos_prod].item_count > 0)
        {
            (*sum_succparag)++;
            (*sum_price) = (*sum_price) + catalog[arithmos_prod].price;
            catalog[arithmos_prod].item_count--;                
            catalog[arithmos_prod].temaxia_sell++;
                
            sprintf(buff, "Purchase complete, your total is %.2lf", catalog[arithmos_prod].price);
            write (p1[1], buff, sizeof(buff));
        }
            
        else
        {   
           (*sum_failparag)++;
           write (p1[1], "Products unavailable, request failed", sizeof("Products unavailable, request failed"));
        }

        sleep(1);
    }  
    
    close(p1[1]);
    close(p2[0]);
}

void child_orders(int p1[2], int p2[2], int client_arithmos)
{
    int i;
    int arithmos_prod;

    close(p1[1]);
    close(p2[0]);

    srand(time(NULL));

    for(i=0; i<10; i++)
    {
        arithmos_prod = rand() % 20;

        write(p2[1], &arithmos_prod, sizeof(arithmos_prod));

        char buf[100];
        read(p1[0], buf, sizeof(buf));

        printf("Client %d: %s\n", client_arithmos, buf);

        sleep(1);
    }

    close(p1[0]);
    close(p2[1]);

    exit(0);
}

void anafora(product catalog[])
{
    int i;

    for(i=0; i<20; i++)
    {
        printf("\nPerigrafi Proiontos %d: %s\n", i+1, catalog[i].description);
        printf("Aithmata gia agora: %d\n", catalog[i].aithmata);
        printf("Temaxia Agorastikan: %d\n", catalog[i].temaxia_sell);
    }
}

void statistics(int sum_parag, int sum_succparag, int sum_failparag, double sum_price)
{
    printf("\nSunolikos arithmos paraggeliwn: %d\n", sum_parag);
    printf("Epituxhmenes Paraggelies: %d\n", sum_succparag);
    printf("Apotuxhmenes Paraggelies: %d\n", sum_failparag);
    printf("Sunoliko kostos: %.2lf\n", sum_price);
}

int main()
{    
    product catalog[20];
    init_catalog(catalog);

    int i;
    int p1[2];
    int p2[2];

    int sum_parag = 0;
    int sum_succparag = 0;
    int sum_failparag = 0;
    double sum_price = 0;
    
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
    
    for(i=0; i<5; i++)
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

    parent_orders(catalog, p1, p2, &sum_parag, &sum_succparag, &sum_failparag, &sum_price);

    for(i=0; i<5; i++)
    {
        wait(NULL);
    }

    anafora(catalog);
    statistics(sum_parag, sum_succparag, sum_failparag, sum_price);

    return 0;
}