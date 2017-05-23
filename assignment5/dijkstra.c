#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define INF INT_MAX

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

void dijkstra(
        int **graph,
        int nbrOfVertices,
        int degree,
        size_t source,
        size_t target
        )
{   
    int *q             = (int *) malloc(nbrOfVertices * sizeof(int));
    unsigned int *dist = (unsigned int *) malloc(nbrOfVertices * sizeof(int));
    int *visited       = (int *) calloc(nbrOfVertices,  sizeof(int));
    int *prev          = (int *) malloc(nbrOfVertices * sizeof(int));
    unsigned int minDist, alt, retDist;
    size_t i;
    int currentNode, minNode, idx, nbrOfVisited, pathLength;

    for (i = 0; i < nbrOfVertices; i++)
    {
        q[i] = i;
        dist[i] = INF;
    }
    dist[source] = 0;
    
    nbrOfVisited = 0;
    while (nbrOfVisited < nbrOfVertices)
    {
        minDist = INF;
        for (i = 0; i < nbrOfVertices; i++)
        {
            if (dist[i] < minDist && visited[i] == 0) {
                minDist = dist[i];
                minNode = i;
            }
        }
        currentNode = minNode;
        visited[currentNode] = 1;
        nbrOfVisited++;

        for (i = 0; i < degree; i++)
        {   
            idx = currentNode*degree + i;
            alt = dist[currentNode] + graph[idx][2];
            if (alt < dist[graph[idx][1]] && visited[graph[idx][1]] == 0) {
                dist[graph[idx][1]] = alt;
                prev[graph[idx][1]] = currentNode;
            }
        }
    }
    retDist = dist[target];
    currentNode = target;
    pathLength = 1;
    while (currentNode != source)
    {
        pathLength++;
        currentNode = prev[currentNode];
    }

    int path[pathLength];
    currentNode = target;
    for (i = pathLength; i > 0; i--)
    {
        path[i-1] = currentNode;
        currentNode = prev[currentNode];
    }

    printf("Shortest distance: %d\n", retDist);
    for (i = 0; i < pathLength-1; i++)
        printf("%d -> ", path[i]);
    printf("%lu\n", target);

    free(q);
    free(dist);
    free(prev);
    free(visited);

    //return retDist;
}


int main(int argc, char **argv)
{   
    FILE         *graphFile;
    char         *fileName;
    char         ch;
    //char         lineStr[128];
    int          i1, i2, w;
    size_t       nbrOfEdges;
    size_t       nbrOfVertices;
    int          **graph;
    size_t       i;
    int          ret, lineIdx, degree;
    unsigned int shortestDist;

    const size_t source = atoi(argv[1]);
    const size_t target = atoi(argv[2]);
    fileName            = argv[3];
    
    graphFile = fopen(fileName, "r");
    nbrOfEdges = 0;
    //lineIdx = 0;
    while (ch != EOF)
    {
        ch = fgetc(graphFile);
        if (ch == '\n') {
            nbrOfEdges++;
            //lineIdx = 0;
        }
        /*
        else {
            lineStr[lineIdx] = ch;
            lineIdx++;
        }
        */
    }
    //sscanf(lineStr, "%d", &i1);
    //nbrOfVertices = i1 + 1;
    //degree = nbrOfEdges / nbrOfVertices;

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
    nbrOfVertices = graph[nbrOfEdges-1][0] + 1;
    degree = nbrOfEdges / nbrOfVertices;

    dijkstra(
            graph,
            nbrOfVertices,
            degree,
            source,
            target
            );

    //printf("Number of edges    : %lu\n", nbrOfEdges);
    //printf("Number of vertices : %lu\n", nbrOfVertices);
    //printf("Degree             : %d\n", degree);
    //printf("Shortest distance  : %d\n", shortestDist);
    //printGraph(graph, nbrOfEdges, 2);
    
    
    for (i = 0; i < nbrOfEdges; i++)
        free(graph[i]);
    free(graph);

    return 0;
}
