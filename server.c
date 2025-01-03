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

void parent_orders(product catalog[], int p1[], int p2[])
{
    int i;
    int sum_parag;
    int sum_succparag;
    int sum_failparag;
    int sum_price;

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
}

void child_orders(int p1[2], int p2[2])
{
    int i;

    close(p1[1]);
    close(p2[0]);

    for(i=0; i<20; i++)
    {
        
    }
}

int main()
{
    product catalog[20];
    init_catalog(catalog);

    int p1[2], p2[2];
    int errno;
    char buf;

    pipe1(p1);
    pipe2(p2);

    int pid1;

    pid1 = fork();

    if(pipe(p1) == -1)
    {
        printf("Error in pipe1\n");
        return -1;
    }

    if(pid1 < 0)
    {
        printf("Error in fork\n");
        return -1;
    }

    else if(pid1 > 0)
    {
        printf("Parent process\n");



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

                for(i=0; i<10; i++)
                {
                    int arithmos;

                    arithmos = rand() % 20;


                }

            }

           
        }
        

    }


    
    
    
    
    return 0;

}