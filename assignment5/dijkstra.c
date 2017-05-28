#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <mpi.h>

#define INF INT_MAX/2

void dijkstra(
        int **graph,
        int nbrOfVertices,
        int degree,
        size_t source,
        size_t target
        )
{   
    unsigned int *dist = (unsigned int *) malloc(nbrOfVertices * sizeof(int));
    int *visited       = (int *) calloc(nbrOfVertices,  sizeof(int));
    int *prev          = (int *) malloc(nbrOfVertices * sizeof(int));
    unsigned int minDist, alt, retDist;
    size_t i;
    int currentNode, minNode, idx, nbrOfVisited, pathLength;

    for (i = 0; i < nbrOfVertices; i++)
    {
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

    free(dist);
    free(prev);
    free(visited);

    //return retDist;
}
void theOnlyOne(int argc, char **argv)
{

    FILE         *graphFile;
    char         *fileName;
    char         ch;
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
    while (ch != EOF)
    {
        ch = fgetc(graphFile);
        if (ch == '\n') {
            nbrOfEdges++;
        }
    }
    
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
    
    for (i = 0; i < nbrOfEdges; i++)
        free(graph[i]);
    free(graph);


}
int main(int argc, char **argv)
{   
    int myrank, nbr_mpi_proc;
    MPI_Init(&argc, &argv);   /* initialize MPI */
    MPI_Comm_rank(MPI_COMM_WORLD,   /* always use this */&myrank);      /* process rank, 0 thru N-1 */
    MPI_Comm_size(MPI_COMM_WORLD, &nbr_mpi_proc);

    if (nbr_mpi_proc == 1) {
        theOnlyOne(argc, argv);
    } else {
        FILE         *graphFile;
        char         *fileName;
        char         ch;
        char         lineStr[128];
        int          i1, i2, w;
        int          nbrOfEdges;
        int          nbrOfVertices;
        int          *graph, *nums;
        int          i, j;
        int          ret, lineIdx, degree, wtf, wth, block, adjSize;

        const int source = atoi(argv[1]);
        const int target = atoi(argv[2]);
        fileName            = argv[3];
            
        graphFile = fopen(fileName, "r");
        nbrOfEdges = 0;
        lineIdx = 0;
        while (1)
        {
            ch = fgetc(graphFile);
            if (ch == EOF) {
               break; 
            } else if (ch == '\n') {
                nbrOfEdges++;
                lineIdx = 0;
            } else {
                lineStr[lineIdx] = ch;
                lineIdx++;
            }
        }

        sscanf(lineStr, "%d", &i1);
        nbrOfVertices = i1 + 1;
        degree = nbrOfEdges / nbrOfVertices;
        wtf = nbrOfEdges / nbr_mpi_proc;
        wth = degree / nbr_mpi_proc;
        block = nbrOfVertices / nbr_mpi_proc;
        graph = (int *) malloc(nbrOfEdges * 3 * sizeof(int));
        fseek(graphFile, 0, SEEK_SET);
        for (i = 0; i < nbrOfEdges * 3; i += 3)
        {
            ret = fscanf(graphFile, "%d", &graph[i]);
            ret = fscanf(graphFile, "%d", &graph[i+1]);
            ret = fscanf(graphFile, "%d", &graph[i+2]);
        }
        fclose(graphFile);
    
        if (degree < 1000)
        {
            adjSize = 200;
        }
        else
        {
            adjSize = 1200;
        }

        int **scatterGraph;
        scatterGraph = (int **) malloc(nbrOfVertices * sizeof(int *));
        for (i = 0; i < nbrOfVertices; i++)
        {
            scatterGraph[i] = (int *) malloc(adjSize * 2 * sizeof(int));
        }
        
        int start = myrank * block;
        int end = start + block;
        nums = (int *) calloc(nbrOfVertices, sizeof(int));
        for (i = 0; i < nbrOfEdges * 3; i += 3)
        {
            if (graph[i+1] >= start && graph[i+1] < end)
            {
                scatterGraph[graph[i]][nums[graph[i]]] = graph[i+1];
                scatterGraph[graph[i]][nums[graph[i]]+1] = graph[i+2];
                nums[graph[i]] += 2;
            }
        }
        
        free(graph);
        
        // Here we begin with Dijkstra 
        int *dist = (int *) malloc(nbrOfVertices * sizeof(int));
        int *prev;
        int *prevBlock = (int *) malloc(block * sizeof(int));
        int *inCluster= (int *) calloc(nbrOfVertices, sizeof(int));
        int alt, retDist;
        int minDistLoc[2];
        int minDist[2];
        int currentNode, minNode, idx, nbrVisited, pathLength, currentDist;

        
        for (i = 0; i < nbrOfVertices; i++)
        {
            dist[i] = INF;
        }
        dist[source] = 0;

        currentNode = source;

        
        for (nbrVisited = 0; nbrVisited < nbrOfVertices; nbrVisited++)
        {
            minDistLoc[0] = INF;
            for (i = start; i < end; i++)
            {
                if (!inCluster[i])
                {
                    if (dist[i] < minDistLoc[0])
                    {
                        minDistLoc[0] = dist[i];
                        minDistLoc[1] = i;
                    }
                }
            }
            MPI_Allreduce(minDistLoc, minDist, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
            currentNode = minDist[1];
            currentDist = minDist[0];
            inCluster[currentNode] = 1;

            for (i = 0; i < nums[currentNode]; i += 2)
            {
                if (!inCluster[scatterGraph[currentNode][i]])
                {
                    alt = currentDist + scatterGraph[currentNode][i+1];
                    if (alt < dist[scatterGraph[currentNode][i]])
                    {
                        dist[scatterGraph[currentNode][i]] = alt;
                        idx = scatterGraph[currentNode][i] % block;
                        prevBlock[idx] = currentNode;
                    }
                }

            }

        }
        MPI_Allreduce(MPI_IN_PLACE, dist, nbrOfVertices, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
        if (myrank == 0)
        {
            prev = (int *) malloc(nbrOfVertices * sizeof(int));
        }
        MPI_Gather(prevBlock, block, MPI_INT, prev, block, MPI_INT, 0, MPI_COMM_WORLD);

        if (myrank == 0)
        {
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
            printf("%d\n", target);
            
            free(prev);
        }
        
        free(dist);
        free(inCluster);
        free(prevBlock);
        for (i = 0; i < nbrOfVertices; i++)
            free(scatterGraph[i]);
        free(scatterGraph);
    }
    
    MPI_Finalize();       /* cleanup MPI */
    return 0;
}

