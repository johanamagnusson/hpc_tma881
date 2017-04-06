#include <stdio.h>
#include <time.h>

#define NUMBER_OF_ITERATIONS 1000

int main()
{
    double diffTime;

    clock_t start;
    clock_t end;

    const unsigned long int num = 1000000000;

    unsigned long int sum;
    unsigned long int i;
    unsigned long int j;
    double totalTime;
    double averageTime;
    
    totalTime = 0.0;
    for (j = 0; j < NUMBER_OF_ITERATIONS; j++)
    {
        sum = 0;
        start = clock();
        for (i = 1; i <= num; i++)
        {
            sum += i;
        }
        end = clock();
        diffTime = ((end - start) / (double) CLOCKS_PER_SEC) * 1000;
        totalTime += diffTime;
    }
    
    averageTime = totalTime / NUMBER_OF_ITERATIONS;
    printf("Sum = %lu\n", sum);
    printf("Time measured: %.8f ms\n", averageTime);
    printf("\n");

    return 0; 
    
}
