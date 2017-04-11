#include <stdio.h>
#include <stdlib.h>
#include <argp.h>



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

    return 0;
}
