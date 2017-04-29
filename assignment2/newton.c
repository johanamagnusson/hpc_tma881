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

int *attraction;
int *convergence;

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

static double complex complex_representation(int i, int l){
   double real_diff = (4./(double)(l-1))*(i%l);
   double imag_diff = (4./(double)(l-1))*(i/l);
   return (-2. + real_diff) + (2. - imag_diff)*I;
}

static struct newton newtons_method(double complex x_0, int d){
    int iteration = 0;
    double limit = 0.001;
    double toBig = 10000000000;
    double n = (double) d;
    double complex x_i = x_0 - ((cpow(x_0, n) - 1.0) / (n * cpow(x_0, n - 1.0)));
    //printf("test %d\n test2 %f + %fi\n ", iteration, creal(x_i), cimag(x_i));
    //first condition probably wrong, how do we know the root? (without computing it?).
    while( (cabs(cpow(x_i, n) - 1.0)  > limit) && (cabs(x_i - x_0) > limit) && (creal(x_i) < toBig) && (cimag(x_i) < toBig)){
        x_i = x_i - ((cpow(x_i, n) - 1.0) / (n * cpow(x_i, n - 1.0)));
        iteration++;
        //printf("test %d\n test2 %f + %fi\n ", iteration, creal(x_i), cimag(x_i));
    }
    
    double from_double_to_int_hash_accuracy = 40.;
    int root;
    if(cabs(cpow(x_i, n) - 1.) <= limit){
        root = (int) from_double_to_int_hash_accuracy * (carg(x_i)+3.141593);
    }else{
        root = 0;
    }
    if(cabs(cpow(x_0, n) - 1.) <= limit){
        iteration = 0;
    }
        
    //printf("root:  %d\n",  root);
    //printf("test %d\n test2 %f + %fi\n ", iteration, creal(x_i), cimag(x_i));
    struct newton a = {iteration, root};
    return a;
}

pthread_mutex_t stopIt;

void *Count(void *c)
{
    int current_Pix = 0;
    while(current_Pix < end){
        pthread_mutex_lock( &stopIt );
        
        //a[counter] = counter;
        current_Pix = counter;
        counter = counter + 1;
        //printf("Counter: %d\n", counter);
        pthread_mutex_unlock( &stopIt );

        complex x_0 = complex_representation(current_Pix,l);
        struct newton a = newtons_method(x_0, d);

        convergence[current_Pix] = a.iterations;
        attraction[current_Pix] = a.root;
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
    end = l*l;

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
    }

    int col = 255/max;


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

    int colourPairing[d][2];
    int k = 0;
    for (int i = 0; i < l*l; i++) {
        attraction[i]
    }

    char fname[PATH_MAX];
    snprintf(fname, PATH_MAX, "newton_attractors_x%d.ppm", d);
    FILE * fatt = fopen(fname, "w");
    fprintf(fatt, "P3\n%d %d\n255\n", l, l);
    
    for(int i = 0; i < l*l; i++){

        if((i+1)%l == 0){
            fprintf(fcon, "\n");
        }
    }

    fclose(fatt);

    snprintf(fname, PATH_MAX, "newton_convergence_x%d.ppm", d);
    FILE * fcon = fopen(fname, "w");
    fprintf(fcon, "P3\n%d %d\n255\n", l, l);
    
    for(int i = 0; i < l*l; i++){
        fprintf(fcon, "%d %d %d ", 255-convergence[i]*col, 255-convergence[i]*col, 255-convergence[i]*col);
        if((i+1)%l == 0){
            fprintf(fcon, "\n");
        }
    }    

    fclose(fcon);
    
    free(convergence);
    free(attraction);

    return 0;
}
