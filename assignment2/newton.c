#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <complex.h>
#include <math.h>
#include <pthread.h>


int counter = 0;
//int *a;

int l;
int t;
int d;
int end;
double complex *roots;

int *attraction;
int *convergence;

double n;
double LIMIT = 0.001;
double TOBIG = 10000000000;

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
    return ret; // add 1 to make root = 1.0 => nbr 1. add 0.5 to correctly round double to int
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


static struct newton newtons_method(double complex x_0) {
    int root;
    int iterations = 0;
    int running = 1;
    double factor2 = 1./n;
    double factor1 = 1.-factor2;
    int d_prim = d-1;
        
    double complex x_i = x_0;
    while(running) {
        if(cabs(x_i) < LIMIT || creal(x_i) > TOBIG || cimag(x_i) > TOBIG) {
            root = d;
            running = 0;
            break;
        } else {
            for (int i = 0; i < d; i++) {
                if(cabs(roots[i] - x_i) < LIMIT) {
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
    //printf("%d\n", iterations);
    return ret;
}

pthread_mutex_t stopIt;

//pthread_mutex_t write1;

//pthread_mutex_t write2;

//void start_Thread(){
//    int writer;
//    pthread_t k;
//    writer = pthread_create(&k, NULL, Write, (void *)1);
//}

//void *Write(void *w)
//{
//    while(1){

//        pthread_mutex_lock( &write1 );
//        printf("Write from first memory pool %d\n", counter);
//        pthread_mutex_unlock( &write1 );

//        pthread_mutex_lock( &write2 );
//        printf("Write from second memory pool %d\n", counter);
//        pthread_mutex_unlock( &write2);

//        if(counter >= end){
//            break;
//        }

//   }
//}

void *Count(void *c)
{
    int current_Pix = 0;
    while(1){
        pthread_mutex_lock( &stopIt );
        
        //a[counter] = counter;
        current_Pix = counter;
        counter = counter + l;
        //printf("Counter: %d\n", counter);
        pthread_mutex_unlock( &stopIt );

        if(current_Pix >= end){
            //printf("Can we break?\n");
            break;
        }
        for (int i = current_Pix; i < current_Pix + l; i++) {
            complex x_0 = complex_representation(i);
            struct newton a = newtons_method(x_0);

            convergence[i] = a.iterations;
            attraction[i] = a.root;
        }
        //printf("Iterations: %d \n", a.iterations);
    }
    //printf("Yes we can!\n");
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
    end = l*l;

    n = (double) d;

    roots = (double complex *) malloc(d*sizeof(double complex));

    for (int i = 0; i < d; i++) {
        double arg = (2 * M_PI / n) * i;
        roots[i] = cexp(arg * I);
        //printf("%.8f\n", arg);
        //printf("%f + i%f\n", creal(roots[i]), cimag(roots[i]));
    }

    printf("t = %d, l = %d, d = %d\n", t, l, d);
    
    //double complex z = -3.0 + 1.0 * I;
    //struct newton n = newtons_method(z, d);
    //printf("iterations = %d\n", n.iterations);

    //a = (int *) malloc(2*sizeof(int));
    convergence = (int *) malloc(l*l*sizeof(int));
    attraction = (int *) malloc(l*l*sizeof(int));

    int NUM_THREADS = t;

    pthread_mutex_init(&stopIt, NULL);
    
    pthread_t threads[NUM_THREADS];
    int val;
    for(long r=0; r<NUM_THREADS; r++){
        val = pthread_create(&threads[r], NULL, Count, (void *)r);

    }   

    for(int i = 0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    //printf("Here is the array\n%d\n", a[0]);
    //printf("%d\n", a[1]);
    int max=0;
    for(int i = 0; i < l*l; i++){
        if(max < convergence[i])
            max = convergence[i];
            //printf("hallå %d\n", max); 
    }
    int col;
    if (max > 255) {
        col = 1;
    } else {
        col = (int) (255 / max);
    }

    printf("%d, %d\n", col, max);

    int colour[7][3];

    for(int i = 0; i<7; i++){
        for(int j = 0; j<3; j++){
            colour[i][j] = 0;
        }
    }

    for(int i = 0; i<3; i++){
        colour[i][i] = 255;
    }

    colour[3][0] = 255;
    colour[3][1] = 255;
    colour[4][0] = 255;
    colour[4][1] = 128;
    colour[5][1] = 255; 
    colour[5][2] = 255;
    colour[6][0] = 204; 
    colour[6][2] = 204;

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
    for(int i = 0; i < l*l; i++){
        if (attraction[i] == d) {
            fprintf(fatt, "%d %d %d ", 0, 0, 0);
        } else {
            fprintf(fatt, "%s", colorStrings[attraction[i]]);
            //fprintf(fatt, "%d %d %d ",
             //       colour[attraction[i]][0],
               //     colour[attraction[i]][1],
                 //   colour[attraction[i]][2]);
        }
    }
    fclose(fatt);
    
    /*
    snprintf(fname, PATH_MAX, "newton_attractors_x%d.txt", d);
    FILE * ftxt = fopen(fname, "w");
    for(int i = 0; i < l*l; i++){
        if (attraction[i] == d) {
            fprintf(ftxt, "%d ", 0);
        } else {
            fprintf(ftxt, "%d ", attraction[i]);
        }
        if((i+1)%l == 0){
            fprintf(ftxt, "\n");
        }
    }
    fclose(ftxt);
    */
   
    int num;
    char **convergenceStrings = (char **) malloc((max+1) * sizeof(char *));
    for (int i = 0; i <= max; i++) {
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
    
    for(int i = 0; i < l*l; i++){
        fprintf(fcon, "%s", convergenceStrings[convergence[i]]);
       // fprintf(fcon, "%d %d %d ", 255-convergence[i]*col,
       //                            255-convergence[i]*col,
       //                            255-convergence[i]*col);
    }    

    fclose(fcon);
    

    free(convergence);
    free(attraction);
    free(roots);
    //free(colorStrings);
    for (int i = 0; i < max; i++) {
        free(convergenceStrings[i]);
    }

    return 0;
}
