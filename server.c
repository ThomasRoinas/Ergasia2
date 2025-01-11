#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>

typedef struct 
{
    char description[100];
    int price;
    int item_count;
    int aithmata;
    int temaxiasell;
    char xrhstesfail[100];
    int fail_count;
} product;

void init_catalog(product catalog[])    
{
    for(int i=0; i<20; i++)
    {
        sprintf(catalog[i].description, "Product(%d)", i);
        catalog[i].price = i*3;
        catalog[i].item_count = 2;
        catalog[i].aithmata = 0;
        catalog[i].temaxiasell = 0;
        catalog[i].fail_count = 0;
    }
}

void parent_orders(product catalog[], int p1[], int p2[], int *sum_parag, int *sum_succparag, int *sum_failparag, int *sum_price)
{
    int i;

   close(p1[0]);
   close(p2[1]);

   for(i=0; i<50; i++)
   { 

        char buff[100];
        char client_id[100];
        int arithmos;
        read(p2[0], &arithmos, sizeof(arithmos));
        (*sum_parag) = (*sum_parag) + 1;
        catalog[arithmos].aithmata++;

        if(arithmos >= 0 && arithmos < 20)
        {
            if(catalog[arithmos].item_count > 0)
            {
                (*sum_succparag)++;
                (*sum_price) = (*sum_price) + catalog[arithmos].price;
                catalog[arithmos].item_count--;
                catalog[arithmos].temaxiasell++;
                
                sprintf(buff, "Purchase complete, your total is %d", catalog[arithmos].price);
                write (p1[1], buff, sizeof(buff));
            }
            
            else
            {   
                (*sum_failparag)++;
                write (p1[1], "Products unavailable, request failed", sizeof("Products unavailable, request failed"));
            }
        }

        else
        {
            (*sum_failparag)++;
            write (p1[1], "fail", sizeof("fail"));
        }

        sleep(1);
    }  
    
    close(p1[1]);
    close(p2[0]);
}

void child_orders(int p1[2], int p2[2], product catalog[], int client_id)
{
    int i;
    int arithmos;

    close(p1[1]);
    close(p2[0]);

    srand(time(NULL));

    for(i=0; i<10; i++)
    {
        arithmos = rand() % 20;

        write(p2[1], &arithmos, sizeof(arithmos));

        char buf[100];
        read(p1[0], buf, sizeof(buf));

        printf("Client %d: %s\n", client_id, buf);

        sleep(1);
    }

    close(p1[0]);
    close(p2[1]);

    exit(0);
}

void anafora(product catalog[], int sum_parag, int sum_succparag, int sum_failparag, int sum_price)
{
    int i;

    for(i=0; i<20; i++)
    {
        printf("\nProduct %d: %s\n", i, catalog[i].description);
        printf("Aithmata: %d\n", catalog[i].aithmata);
        printf("Temaxia sell: %d\n", catalog[i].temaxiasell);
    }
}

void statistics(int sum_parag, int sum_succparag, int sum_failparag, int sum_price)
{
    printf("\nParaggelies: %d\n", sum_parag);
    printf("Paraggelies success: %d\n", sum_succparag);
    printf("Paraggelies fail: %d\n", sum_failparag);
    printf("Kostsos: %d\n", sum_price);
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
    int sum_price = 0;
    
    if(pipe(p1) == -1)
    {
        printf("Error in pipe1\n");
        return -1;
    }

    if(pipe(p2) == -1)
    {
        printf("Error in pipe2\n");
        return -1;
    }
    
    for(i=0; i<5; i++)
    {
        pid_t pid = fork();

        if(pid < 0)
        {
            printf("Error in fork\n");
            return -1;
        }

        else if(pid == 0)
        {
            child_orders(p1, p2, catalog, i+1); 
        }
    }

    parent_orders(catalog, p1, p2, &sum_parag, &sum_succparag, &sum_failparag, &sum_price);

    for(i=0; i<5; i++)
    {
        wait(NULL);
    }

    anafora(catalog, sum_parag, sum_succparag, sum_failparag, sum_price);
    statistics(sum_parag, sum_succparag, sum_failparag, sum_price);

    return 0;
}