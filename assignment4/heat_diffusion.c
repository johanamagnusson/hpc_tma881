#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <math.h>


struct arguments
{
    long double i;
    double d;
    int n;
};

static int
parse_opt(int key, char *arg,
        struct argp_state *state)
{
    struct arguments *arguments = state -> input;
    switch (key)
    {
        case 'i':
            {
                arguments -> i = atof(arg);
                break;
            }
        case 'd':
            {
                arguments -> d = atof(arg);
                break;
            }
        case 'n':
            {
                arguments -> n = atoi(arg);
                break;
            }
    }
    return 0;
}

struct argp_option options[] =
{
    {"initial central value", 'i', "NUM", 0, "The mid point starting value"},
    {"diffusion constant", 'd', "NUM", 0, "The diffusion constant"},
    {"iterations", 'n', "NUM", 0, "The number of iterations"},
    {0}
};

struct argp argp = { options, parse_opt, 0, 0 };

int main(int argc, char **argv)
{   
    int width;
    int height;
    long double initCentValue;
    double diffusionConst;
    int iterations;
    struct arguments arguments;

    width = atoi(argv[1]);
    height = atoi(argv[2]);

    argp_parse (&argp, argc, argv, 0, 0, &arguments); 

    initCentValue = arguments.i;
    diffusionConst = arguments.d;
    iterations = arguments.n;
    
    printf("Width                 : %d\n", width);
    printf("Height                : %d\n", height);
    printf("Initial central value : %Le\n", initCentValue);
    printf("Diffusion constant    : %.2f\n", diffusionConst);
    printf("Number of iterations  : %d\n", iterations);


    return 0;
}
