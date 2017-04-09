#include <stdio.h>
#include <unistd.h>


int main(int argc, char **argv)
{
    int tvalue = 0;
    int lvalue = 0;
    int dvalue = 0;
    int c;
    int i;

    while ((c = getopt(argc, argv, "t l (d)")) != -1)
        switch(c)
        {
            case 't':
                tvalue = optarg;
                break;
            case 'l':
                lvalue = optarg;
                break;
            default:
                abort ();
        }

    for (i = optind; i < argc; i++)
    {
        dvalue = argv[i];
    }
    printf("t = %d, l = %d, d = %d", tvalue, lvalue, dvalue);
}
