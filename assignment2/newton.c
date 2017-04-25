#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <complex.h>
#include <math.h>

struct arguments
{
    int t;
    int l;
    int d;
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
        case 'l':
            {
                arguments -> l = atoi(arg);
                break;
            }
        case ARGP_KEY_ARG:
            {
                if (state -> arg_num >= 1)
                    argp_failure (state, 1, 0, "too many arguments");
                arguments -> d = atoi(arg);
            }
            break;
        case ARGP_KEY_END:
            {
                if (state -> arg_num < 1)
                    argp_failure (state, 1, 0, "too few arguments");
            }
            break;
    }
    return 0;
}

struct newton{
    int iterations;
    int root; //might be represented as the complex root:W
};

static double complex complex_representation(int i, int l){
   double real_diff = (4./(double)(l-1))*(i%l);
   double imag_diff = (4./(double)(l-1))*(i/l);
   return (-2. + real_diff) + (2. - imag_diff)*I;
}

static struct newton newtons_method(double complex x_0, int d){
    int iteration = 1;
    double limit = 0.001;
    double toBig = 10000000000;
    double n = (double) d;
    double complex x_i = x_0 - ((cpow(x_0, n) - 1.0) / (n * cpow(x_0, n - 1.0)));
    printf("test %d\n test2 %f + %fi\n ", iteration, creal(x_i), cimag(x_i));
    //first condition probably wrong, how do we know the root? (without computing it?).
    while( (cabs(cpow(x_i, n) - 1.0)  > limit) && (cabs(x_i - x_0) > limit) && (creal(x_i) < toBig) && (cimag(x_i) < toBig) && iteration < 20 ){
        x_i = x_i - ((cpow(x_i, n) - 1.0) / (n * cpow(x_i, n - 1.0)));
        iteration++;
        printf("test %d\n test2 %f + %fi\n ", iteration, creal(x_i), cimag(x_i));
    }
    struct newton a = {iteration, 1};
    return a;
}



struct argp_option options[] =
{
    {"threads", 't', "NUM", 0, "The number of threads to use"},
    {"lines", 'l', "NUM", 0, "The size of the picture"},
    {0}
};

struct argp argp = { options, parse_opt, 0, 0 };

int main(int argc, char **argv)
{
    
    struct arguments arguments;

    argp_parse (&argp, argc, argv, 0, 0, &arguments); 

    int tvalue = arguments.t;
    int lvalue = arguments.l;
    int dvalue = arguments.d;
    int i;

    printf("t = %d, l = %d, d = %d\n", tvalue, lvalue, dvalue);
    
    double complex z = -3.0 + 1.0 * I;
    struct newton n = newtons_method(z, dvalue);
    printf("iterations = %d\n", n.iterations);


    return 0;
}
