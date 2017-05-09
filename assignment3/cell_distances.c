#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <math.h>
#include <omp.h>

double **points;
int distanceHist[3464];

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

struct argp_option options[] =
{
    {"threads", 't', "NUM", 0, "The number of threads to use"},
    {0}
};

struct argp argp = { options, parse_opt, 0, 0 };

double compute_distance(
        double x1,
        double x2,
        double y1,
        double y2,
        double z1,
        double z2
        )
{
    double ret;
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dz = z2 - z1;
    ret = sqrt(dx*dx + dy*dy + dz*dz);
    return ret;
}

int main(int argc, char **argv)
{
    const unsigned int numberOfDistances = 3464;
    FILE *cellFile;
    int i, j, ret, distanceIndex;
    double distance;
    double distances[3464];
    char ch;
    struct arguments arguments;
    unsigned long int numberOfPoints;

    
    argp_parse (&argp, argc, argv, 0, 0, &arguments); 
    int NUM_THREADS = arguments.t;

    cellFile = fopen("cell", "r");
    numberOfPoints = 0;
    do
    {
        ch = fgetc(cellFile);
        if (ch == '\n')
            numberOfPoints++;
    } while (ch != EOF);

    points =  malloc(numberOfPoints * sizeof(double *));
    for (i = 0; i < numberOfPoints; i++)
    {
        points[i] = malloc(3 * sizeof(double));
    }

    //distanceHist = (int *) malloc(numberOfDistances * sizeof(int));
    for (i = 0; i < numberOfDistances; i++)
    {
        distanceHist[i] = 0;
        distances[i] = (double) i * 0.01;
    }

    fseek(cellFile, 0, SEEK_SET);
    for (i = 0; i < numberOfPoints; i++)
    {
        ret = fscanf(cellFile, "%lf", &points[i][0]);
        ret = fscanf(cellFile, "%lf", &points[i][1]);
        ret = fscanf(cellFile, "%lf", &points[i][2]);
    }
    fclose(cellFile);
    
    omp_set_num_threads(4);
    #pragma omp parallel for private(k,j) shared() collapse(2) //reduction()
    
    printf("Number of threads: %d\n", NUM_THREADS);
    printf("Number of points: %lu\n", numberOfPoints);
    
    for (i = 0; i < numberOfPoints; i++)
    {
        for (j = 0; j < numberOfPoints; j++)
        {    
  
            distance = compute_distance(
                    points[i][0],
                    points[j][0],
                    points[i][1],
                    points[j][1],
                    points[i][2],
                    points[j][2]
                    );
            distanceIndex = (int) (distance * 100.0 + 0.5);
            distanceHist[distanceIndex]++;
        }
    }
    
    
    for (i = 0; i < numberOfDistances; i++)
    {   
        if (distanceHist[i] > 0)
        {
            printf("%02.2f %d\n", distances[i], distanceHist[i]);
        }
        
    }
    
    #pragma omp parallel end

    printf("Number of threads: %d\n", NUM_THREADS);
    printf("Number of points: %lu\n", numberOfPoints);

    for (i = 0; i < numberOfPoints; i++)
    {
        free(points[i]);
    }
    //free(distanceHist);

    return 0;
}
