#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void printGraph(int **graph, size_t nbrOfEdges, int width)
{   
    size_t i;
    for (i = 0; i < nbrOfEdges; i++)
    {
        printf("%*d ", width, graph[i][0]);
        printf("%*d ", width, graph[i][1]);
        printf("%*d ", width, graph[i][2]);
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
    size_t  nbrOfEdges;
    size_t  nbrOfVertices;
    int     **graph;
    size_t  i;
    int     ret, lineIdx;

    const size_t source      = atoi(argv[1]);
    const size_t destination = atoi(argv[2]);
    fileName                 = argv[3];
    
    graphFile = fopen(fileName, "r");
    nbrOfEdges = 0;
    lineIdx = 0;
    while (ch != EOF)
    {
        ch = fgetc(graphFile);
        if (ch == '\n') {
            nbrOfEdges++;
            lineIdx = 0;
        } else {
            lineStr[lineIdx] = ch;
            lineIdx++;
        }
    }
    sscanf(lineStr, "%d", &i1);
    nbrOfVertices = i1 + 1;

    graph = (int **) malloc(nbrOfEdges * sizeof(int *));
    for (i = 0; i < nbrOfEdges; i++)
    {
        graph[i] = (int *) malloc(3 * sizeof(int));
    }
    
    fseek(graphFile, 0, SEEK_SET);
    for (i = 0; i < nbrOfEdges; i++)
    {
        ret = fscanf(graphFile, "%d", &graph[i][0]);
        ret = fscanf(graphFile, "%d", &graph[i][1]);
        ret = fscanf(graphFile, "%d", &graph[i][2]);
    }
    fclose(graphFile);

    printf("Number of edges    : %lu\n", nbrOfEdges);
    printf("Number of vertices : %lu\n", nbrOfVertices);
    //printGraph(graph, nbrOfEdges, 2);
    
    for (i = 0; i < nbrOfEdges; i++)
        free(graph[i]);

    return 0;
}
