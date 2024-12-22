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

                

            }

           
        }
        

    }




    
    
    
    
    return 0;

}