#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BENCHMARK_ITERATIONS 10000

int main()
{
    int kx, jx, ix, a, i;
    const int n = 1000000;
    const int m = 1000;

    int *p;
    int *x;
    int *y;

    double diffTime;
    double totalTime;
    double averageTime;
    clock_t start;
    clock_t end;

    ix = 0;
    p = (int *) malloc (n * sizeof(int));
    for (ix = 0; ix < n; ++ix)
    {
        p[ix] = ix;
    }

    x = (int *) malloc (n * sizeof(int));
    y = (int *) malloc (n * sizeof(int));
    for (ix = 0; ix < n; ++ix)
    {
        x[ix] = ix % 10;
        y[ix] = 0;
    }

    a = 5;
    totalTime = 0.0;
    for (i = 0; i < BENCHMARK_ITERATIONS; i++)
    {
        start = clock();
        for (kx = 0; kx < n; ++kx)
        {
            jx = p[kx];
            y[jx] += a * x[jx];
        }
        end = clock();
        diffTime = ((end - start) / (double) CLOCKS_PER_SEC) * 1000;
        totalTime += diffTime;
    }
    averageTime = totalTime / BENCHMARK_ITERATIONS;
    
    printf("--- Indirect addressing ---\n");
    printf("Time measured: %.8f ms\n", averageTime);
    printf("\n");

    totalTime = 0.0;
    for (i = 0; i < BENCHMARK_ITERATIONS; i++)
    {
        start = clock();
        for (kx = 0; kx < n; ++kx)
        {
            y[kx] += a * x[kx];
        }
        end = clock();
        diffTime = ((end - start) / (double) CLOCKS_PER_SEC) * 1000;
        totalTime += diffTime;
    }
    averageTime = totalTime / BENCHMARK_ITERATIONS;

    free(p); free(x); free(y);
    
    printf("--- Direct addressing ---\n");
    printf("Time measured: %.8f ms\n", averageTime);
    printf("\n");
    return 0;
}
