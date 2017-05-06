#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <math.h>
#include <omp.h>


struct arguments
{
    int t;
};

static int
parse_opt(int key, char *arg,
        struct argp_state *state)
{
    struct arguments *arguments = state -> input;
    switch (key)
    {
        case 't':
            {
                arguments -> t = atoi(arg);
                break;
            }
    }
    return 0;
}

/*
double complex ipow(complex double base, int exp)
{
    double complex result = 1;
    while (exp){
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}
*/

struct argp_option options[] =
{
    {"threads", 't', "NUM", 0, "The number of threads to use"},
    {0}
};

struct argp argp = { options, parse_opt, 0, 0 };

int main(int argc, char **argv)
{
    
    struct arguments arguments;
    argp_parse (&argp, argc, argv, 0, 0, &arguments); 
    int NUM_THREADS = arguments.t;

    printf("Number of threads: %d\n", NUM_THREADS);

    return 0;
}
