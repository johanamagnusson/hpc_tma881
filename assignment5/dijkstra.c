#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void printGraph(int **graph, size_t nbrOfPoints, int width)
{   
    size_t i, j;
    for (i = 0; i < nbrOfPoints; i++)
    {
        for (j = 0; j < nbrOfPoints; j++)
            printf("%*d ", width, graph[i][j]);
        printf("\n");
    }
}


int main(int argc, char **argv)
{   
    FILE    *graphFile;
    char    *fileName;
    char    ch;
    char    lineStr[128];
    int     i1, i2, w;
    size_t  nbrOfPoints;
    size_t  nbrOfVertices;
    int     **graph;
    size_t  i;
    int     ret, lineIdx;

    const size_t source      = atoi(argv[1]);
    const size_t destination = atoi(argv[2]);
    fileName                 = argv[3];
    
    graphFile = fopen(fileName, "r");
    nbrOfVertices = 0;
    lineIdx = 0;
    while (ch != EOF)
    {
        ch = fgetc(graphFile);
        if (ch == '\n') {
            nbrOfVertices++;
            lineIdx = 0;
        } else {
            lineStr[lineIdx] = ch;
            lineIdx++;
        }
    }
    sscanf(lineStr, "%d", &i1);
    nbrOfPoints = i1 + 1;

    graph = (int **) malloc(nbrOfPoints * sizeof(int *));
    for (i = 0; i < nbrOfPoints; i++)
    {
        graph[i] = (int *) malloc(nbrOfPoints * sizeof(int));
    }
    
    fseek(graphFile, 0, SEEK_SET);
    for (i = 0; i < nbrOfVertices; i++)
    {
        ret = fscanf(graphFile, "%d", &i1);
        ret = fscanf(graphFile, "%d", &i2);
        ret = fscanf(graphFile, "%d", &w);
        graph[i1][i2] = w;
    }
    fclose(graphFile);

    printf("Number of vertices : %lu\n", nbrOfVertices);
    printf("Number of Points   : %lu\n", nbrOfPoints);
    printGraph(graph, nbrOfPoints, 2);
    
    for (i = 0; i < nbrOfPoints; i++)
        free(graph[i]);

    return 0;
}
