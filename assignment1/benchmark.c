#include <stdio.h>
#include <time.h>

#define NUM_OF_EXECS 100000

int main()
{
    double diff;

    // Addition
    int i;
    long int x, y, sum;
    
    x = 10;
    y = 20;
    clock_t start = clock();
    for (i = 0; i < NUM_OF_EXECS; i++)
    {
        sum = x + y;
    }
    clock_t end = clock();
    diff = ((end - start) / (double) CLOCKS_PER_SEC) * 1000;
    printf("%.5f\n", diff);

    return 0; 
    
}
