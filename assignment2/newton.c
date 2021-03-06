#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <complex.h>
#include <math.h>
#include <pthread.h>

#define MEMORY_POOL_NR_OF_ROWS 120

int counter = 0;

int l;
int t;
int d;
double complex *roots;
int rowsWritten = 0;
int thrshld = 40;
int max = 0;

int *memoryPoolAttraction;
int *memoryPoolConvergence;

double n;
double LIMIT   = 0.001;
double LIMITSQ = 0.000001;
double TOBIG   = 10000000000;

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
    int root;
};

static double complex complex_representation(int i){
   double real_diff = (4./(double)(l-1))*(i%l);
   double imag_diff = (4./(double)(l-1))*(i/l);
   return (-2. + real_diff) + (2. - imag_diff)*I;
}

static int root_as_int(double complex root){
    double arg = carg(root);
    if(arg < 0){
        arg += 2*M_PI;
    }
    int ret = (int) ((arg/(2*M_PI/ (double) d)) + 1.5);
    return ret;
}

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

double iabs(complex double x) {
    double real = creal(x);
    double imag = cimag(x);
    double ret = real*real + imag*imag;
    return ret;
}

static struct newton newtons_method(double complex x_0) {
    int root;
    int iterations = 0;
    int running = 1;
    double factor2 = 1./n;
    double factor1 = 1.-factor2;
    int d_prim = d-1;
        
    double complex x_i = x_0;
    while(running) {
        if(iabs(x_i) < LIMITSQ || creal(x_i) > TOBIG || cimag(x_i) > TOBIG) {
            root = d;
            iterations = 255;
            running = 0;
            break;
        } else {
            for (int i = 0; i < d; i++) {
                if(iabs(roots[i] - x_i) < LIMITSQ) {
                    root = i;
                    running = 0;
                    iterations--;
                    break;
                }
            }
        }
        x_i = factor1*x_i + factor2*(1./ipow(x_i, d_prim));
        iterations++;
    }
    
    struct newton ret = {iterations, root};
    
    return ret;
}

static struct newton newtons_method2(double complex x_0) {
    int root;
    int iterations = 0;
    int running = 1;
    double factor2 = 0.5;
    double factor1 = 0.5;
        
    double complex x_i = x_0;
    while(running) {
        if(iabs(x_i) < LIMITSQ || creal(x_i) > TOBIG || cimag(x_i) > TOBIG) {
            root = d;
            iterations = 255;
            running = 0;
            break;
        } else {
            for (int i = 0; i < d; i++) {
                if(iabs(roots[i] - x_i) < LIMITSQ) {
                    root = i;
                    running = 0;
                    iterations--;
                    break;
                }
            }
        }
        x_i = factor1*x_i + factor2*(1./x_i);
        iterations++;
    }
    
    struct newton ret = {iterations, root};
    
    return ret;
}

struct newton (*newtonPtr)(double complex);

pthread_mutex_t stopIt = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitCond = PTHREAD_COND_INITIALIZER;


void *Write(void *w)
{
    int i;
    int writeRow = 0;
    int max = 255;
    int col = 1;

    char *colorStrings[7];
    colorStrings[0] = "255 0 0 ";
    colorStrings[1] = "0 255 0 ";
    colorStrings[2] = "0 0 255 ";
    colorStrings[3] = "255 255 0 ";
    colorStrings[4] = "255 128 0 ";
    colorStrings[5] = "0 255 255 ";
    colorStrings[6] = "204 0 204 ";

    char fname[PATH_MAX];
    snprintf(fname, PATH_MAX, "newton_attractors_x%d.ppm", d);
    FILE * fatt = fopen(fname, "w");
    fprintf(fatt, "P3\n%d %d\n255\n", l, l);

    int num;
    char **convergenceStrings = (char **) malloc((max+1) * sizeof(char *));
    for (i = 0; i <= max; i++) {
        convergenceStrings[i] = (char *) malloc(20 * sizeof(char));
        if (i > 255) {
            num = 0;
        } else {
            num = 255 - i*col;
        }
        snprintf(convergenceStrings[i], 20 * sizeof(char), "%d %d %d ",
                num,
                num,
                num);
    }

    snprintf(fname, PATH_MAX, "newton_convergence_x%d.ppm", d);
    FILE * fcon = fopen(fname, "w");
    fprintf(fcon, "P3\n%d %d\n255\n", l, l);
    
    while(rowsWritten < l) {
        if(rowsWritten + thrshld < counter || rowsWritten > l - thrshld) {
            for(i = writeRow*l; i < writeRow*l + l; i++) {
                if (memoryPoolAttraction[i] == d) {
                    fprintf(fatt, "%d %d %d ", 0, 0, 0);
                } else {
                    fprintf(fatt, "%s", colorStrings[memoryPoolAttraction[i]]);
                }
                if (memoryPoolConvergence[i] > 255) {
                    fprintf(fcon, "%s", convergenceStrings[max]);
                } else {
                    fprintf(fcon, "%s", convergenceStrings[memoryPoolConvergence[i]]);
                }
            }
            //Tell the calculating how far we have been writing
            pthread_mutex_lock(&stopIt);
            rowsWritten += 1;
            pthread_cond_signal(&waitCond);
            pthread_mutex_unlock(&stopIt);
            writeRow = (writeRow + 1) % MEMORY_POOL_NR_OF_ROWS;
        }
    }
    fclose(fatt);
    fclose(fcon);
    for (int i = 0; i < max; i++) {
        free(convergenceStrings[i]);
    }
    pthread_exit(NULL);
}


void *Count(void *c)
{
    int current_Pix = 0;
    int saveRow;
    int saveIndex;
    int i;
    int j;

    while(1){
        pthread_mutex_lock( &stopIt );
        
        current_Pix = counter;
        counter = counter + 1;
        
        //If the write to file thread is to slow, we wait for it        
        
        while(rowsWritten < current_Pix - 2*thrshld)
        {
            pthread_cond_wait(&waitCond, &stopIt);
        }
        pthread_mutex_unlock( &stopIt );

        if(current_Pix >= l){
            break;
        }


        saveRow = current_Pix % MEMORY_POOL_NR_OF_ROWS;

        for (i = 0; i < l; i++) {
            j = current_Pix*l + i;
            complex x_0 = complex_representation(j);
            struct newton a;

            a = (*newtonPtr)(x_0);

            memoryPoolConvergence[saveRow*l + i] = a.iterations;
            memoryPoolAttraction[saveRow*l + i] = a.root;
        }
    }
    pthread_exit(NULL);
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

    t = arguments.t;
    l = arguments.l;
    d = arguments.d;

    n = (double) d;

    roots = (double complex *) malloc(d*sizeof(double complex));

    for (int i = 0; i < d; i++) {
        double arg = (2 * M_PI / n) * i;
        roots[i] = cexp(arg * I);
    }

    printf("t = %d, l = %d, d = %d\n", t, l, d);
    

    memoryPoolAttraction = (int *) malloc(MEMORY_POOL_NR_OF_ROWS*l*sizeof(int));
    memoryPoolConvergence = (int *) malloc(MEMORY_POOL_NR_OF_ROWS*l*sizeof(int));

    int NUM_THREADS = t;

    if (d == 2) {
        newtonPtr = &newtons_method2;
    } else {
        newtonPtr = &newtons_method;
    }

    pthread_mutex_init(&stopIt, NULL);
    
    pthread_t threads[NUM_THREADS];
    pthread_t writeThread;
    int val;
    long r;
    for(r = 0; r<NUM_THREADS; r++){
        val = pthread_create(&threads[r], NULL, Count, (void *)r);
    }
    val = pthread_create(&writeThread, NULL, Write, (void *)r);

    for(int i = 0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    pthread_join(writeThread, NULL);


    free(memoryPoolAttraction);
    free(memoryPoolConvergence);
    free(roots);

    return 0;
}
