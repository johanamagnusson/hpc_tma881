#include <stdio.h>
#include <stdlib.h>
#include "mulcpx.h"
#include <time.h>

void mul_cpx(
            double * a_re,
            double * a_im,
            double * b_re,
            double * b_im,
            double * c_re,
            double * c_im)
{
    *a_re = (*b_re) * (*c_re) - (*b_im) * (*c_im);
    *a_im = (*b_re) * (*c_im) + (*b_im) * (*c_re);
}

int main()
{
	int n = 30000;
    
    time_t start,end;
    double difference;

	double a[n][2];
	double b[n][2];
	double c[n][2];

    for ( int i=0; i<n; ++i )
    {
        b[i][0] = (rand() % n);
        b[i][1] = (rand() % n);
        c[i][0] = (rand() % n);
        c[i][1] = (rand() % n);
    }

    start = time(NULL);

    for ( int i=0; i<n; ++i)
    {
        mul_cpx(&a[i][0], &a[i][1], &b[i][0], &b[i][1], &c[i][0], &c[i][1]);
    }

    end = time(NULL);

    difference = difftime(end,start);
    printf("%d seconds\n", (int)difference);

    start = time(NULL);

    for ( int i=0; i<n; ++i)
    {
        mulcpx(&a[i][0], &a[i][1], &b[i][0], &b[i][1], &c[i][0], &c[i][1]);
    }

    end = time(NULL);

    difference = difftime(end,start);
    printf("%d seconds\n", (int)difference);
}

