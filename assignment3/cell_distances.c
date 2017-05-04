#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <math.h>

int t;

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
        case ARGP_KEY_ARG:
            {
                if (state -> arg_num >= 0)
                    argp_failure (state, 1, 0, "too many arguments");
            }
            break;
        case ARGP_KEY_END:
            {
                if (state -> arg_num < 0)
                    argp_failure (state, 1, 0, "too few arguments");
            }
            break;
    }
    return 0;
}


struct argp_option options[] =
{
    {"threads", 't', "NUM", 0, "The number of threads to use"},
};

static float distance(float* p1, float* p2){
    float x = p1[0]-p2[0];
    float y = p1[1]-p2[1];
    float z = p1[2]-p2[2];
    float distance = sqrt(x*x + y*y + z*z);
    //printf("hej: %.3f\n", distance);
    return distance;
}

struct argp argp = { options, parse_opt, 0, 0 };

static void binary_add(float value, int first, int last, int *array){
    //detta lär aldrig fungera. får fundera på detta.
    //måste sortera arrayen på något sätt eller?
    int middle = (first+last)/2;

    while (first <= last) {
        if (array[middle] < value)
            first = middle + 1;    
        else if (array[middle] == value) {
            break;
        }
        else
            last = middle - 1;
            middle = (first + last)/2;
    }
}
static void sort_array(int n, float *number){
    for (int i = 0; i < n; ++i){
        for (int j = i + 1; j < n; ++j){
            if (number[i] > number[j]){
                float a =  number[i];
                number[i] = number[j];
                number[j] = a;
            }
        }

    }
}
int main(int argc, char **argv)
{
    
    struct arguments arguments;

    argp_parse (&argp, argc, argv, 0, 0, &arguments); 

    t = arguments.t;
    
    printf("t: %d\n", t);
    
    
    FILE *myfile;
    float myvariable;
    int i;
    int j;
    
    int length = 10;
    int combinations = length*(length-1)/2;
    float b[combinations];//kommer behöve malloca denna för att göra den global. 
    float a[length][3];
    float current_point[3];
    float compare_point[3];
    myfile=fopen("cells", "r");
    int HEIGHT = 10;
    int WIDTH = 3;
    for(i = 0; i < HEIGHT; i++){
        for (j = 0 ; j < WIDTH; j++){
            fscanf(myfile,"%f", &a[i][j]);
            //a[i][j] = myvariable;
            printf("%.3f ", a[i][j]);
        }
        printf("\n");
    }

    fclose(myfile);
    int k = 0;
    for(i = 0; i < HEIGHT; i++){
        for (j = i+1 ; j < HEIGHT; j++){
            b[k] = distance(&a[i][0], &a[j][0]);
            k++;
        }
    }
    sort_array(combinations, &b[0]);
    for(i = 0; i < combinations; i++){
        printf("distance: %.3f\n", b[i]);
    }
     
        
    return 0;
}
