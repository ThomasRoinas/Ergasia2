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

    int fd[2];
    int errno;
    char buf;

    
    
    
    
    
    eturn 0;

}