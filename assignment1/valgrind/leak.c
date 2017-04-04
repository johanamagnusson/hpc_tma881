#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"

int main()
{

    unsigned long int sum=0;

    alloc();

    for ( int i=1; i<=1000000000; i++)
    {
        sum += i;
    }

    printf("Sum = %lu\n",sum);
}
