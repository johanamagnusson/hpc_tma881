#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <math.h>
#include <omp.h>


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

//int *get_pair(int index)

int main(int argc, char **argv)
{
    FILE *cellFile;
    int i, j, ret;
    double distance;
    char ch;
    struct arguments arguments;
    unsigned long int numberOfPoints, numberOfDistances, count;
    
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

    double **points =  malloc(numberOfPoints * sizeof(double *));
    for (i = 0; i < numberOfPoints; i++)
    {
        points[i] = malloc(3 * sizeof(double));
    }
    
    numberOfDistances = numberOfPoints * (numberOfPoints - 1) / 2;
    short *distances = (short *) malloc(numberOfDistances * sizeof(short));


    fseek(cellFile, 0, SEEK_SET);
    for (i = 0; i < numberOfPoints; i++)
    {
        ret = fscanf(cellFile, "%lf", &points[i][0]);
        ret = fscanf(cellFile, "%lf", &points[i][1]);
        ret = fscanf(cellFile, "%lf", &points[i][2]);

        /*
        printf("%.3f %.3f %.3f\n",
                points[i][0],
                points[i][1],
                points[i][2]);
        */
    }
    fclose(cellFile);

    count = 0;
    for (i = 0; i < numberOfPoints; i++)
    {
        for (j = i + 1; j < numberOfPoints; j++)
        {   
            distance = compute_distance(
                    points[i][0],
                    points[j][0],
                    points[i][1],
                    points[j][1],
                    points[i][2],
                    points[j][2]
                    );
            distances[count] = (short) (distance * 100.0 + 0.5);
            count++;
        }
    }


    printf("Number of threads: %d\n", NUM_THREADS);
    printf("Number of points: %lu\n", numberOfPoints);
    printf("Number of distances: %lu\n", numberOfDistances);

    for (i = 0; i < numberOfPoints; i++)
    {
        free(points[i]);
    }
    free(distances);

    return 0;
}
