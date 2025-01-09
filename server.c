#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct 
{
    char description[100];
    int price;
    int item_count;
} product;

void init_catalog(product catalog[])    
{
    for(int i = 0; i<20; i++)
    {
        strcpy(catalog[i].description, "Product(i)");
        catalog[i].price = i*3;
        catalog[i].item_count = 2;
    }
}

void parent_orders(product catalog[], int p1[], int p2[], int sum_parag, int sum_succparag, int sum_failparag, int sum_price)
{
    int i;

   close(p1[0]);
   close(p2[1]);

   for(i=0; i<20; i++)
   {
        int arithmos; 
        read(p2[0], &arithmos, sizeof(arithmos));
        sum_parag = sum_parag + 1;

        if(arithmos >= 0 && arithmos < 20)
        {
            if(catalog[arithmos].item_count > 0)
            {
                sum_succparag++;
                sum_price = sum_price + catalog[arithmos].price;
                catalog[arithmos].item_count--;
                write (p1[1], "success", sizeof("success"));
            }
            
            else
            {
                sum_failparag++;
                write (p1[1], "fail", sizeof("fail"));
            }
        }

        else
        {
            sum_failparag++;
            write (p1[1], "fail", sizeof("fail"));
        }

        sleep(1);
    }  
    //////////////////////////
}

void child_orders(int p1[2], int p2[2])
{
    int i;
    int arithmos;

    close(p1[1]);
    close(p2[0]);

    for(i=0; i<10; i++)
    {
        arithmos = rand() % 20;

        write(p2[1], &arithmos, sizeof(arithmos));

        char buf[100];
        read(p1[0], buf, sizeof(buf));

        printf("Child[%d] ordered product %d: %s\n", getpid(), arithmos, buf);

        sleep(1);
    }
    //////////////////////////
}

int main()
{
    product catalog[20];
    init_catalog(catalog);

    int i;
    int p1[2], p2[2];
    char buf;

    pipe(p1);
    pipe(p2);

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

    printf("Parent process\n");

    int sum_parag = 0;
    int sum_succparag = 0;
    int sum_failparag = 0;
    int sum_price = 0;

    int pid[5];

    for(int i=0; i<5; i++)
    {
        pid[i] = fork();

        if(pid[i] < 0)
        {
            printf("Error in fork\n");
            return -1;
        }

        else if(pid[i] == 0)
        {
            printf("Child[%d] process\n", i);

            child_orders(p1, p2);

            exit(0);
        }
    }

    parent_orders(catalog, p1, p2, sum_parag, sum_succparag, sum_failparag, sum_price);  
    
    printf("Results:\n");
    printf("Paraggelies: %d\n", sum_parag);
    printf("Success paraggelies: %d\n", sum_succparag);
    printf("Fail paraggelies: %d\n", sum_failparag);
    printf("Kostos: %d\n", sum_price);

    for(i=0; i<5; i++)
    {
        wait(NULL);
    }

    return 0;
}