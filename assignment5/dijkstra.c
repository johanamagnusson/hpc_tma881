#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <mpi.h>

#define INF INT_MAX

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
        printf("Number of processes %d:", nbr_mpi_proc);
        //printf("I am your master!");
        FILE         *graphFile;
        char         *fileName;
        char         ch;
        int          i1, i2, w;
        size_t       nbrOfEdges;
        size_t       nbrOfVertices;
        int          *graph;
        size_t       i;
        int          ret, lineIdx, degree;

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
            graph = (int *) malloc(nbrOfEdges* 3  * sizeof(int *));
            /*for (i = 0; i < nbrOfEdges; i++)
            {
                graph[i] = (int *) malloc(3 * sizeof(int));
            }*/
        if (myrank == 0){ 
            fseek(graphFile, 0, SEEK_SET);
            for (i = 0; i < nbrOfEdges*3; i+=3)
            {
                ret = fscanf(graphFile, "%d", &graph[i]);
                ret = fscanf(graphFile, "%d", &graph[i+1]);
                ret = fscanf(graphFile, "%d", &graph[i+2]);
            }
            fclose(graphFile);
            nbrOfVertices = graph[nbrOfEdges*3 -3] + 1;
            degree = nbrOfEdges / nbrOfVertices;
            
            for ( i = 1; i<nbr_mpi_proc; i++){
            
                //int dest_rank = 1; int tag = 1;
                MPI_Send(&degree, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                //printf( "MPI message sent from %d: %d\n", mpi_rank, msg );
            
            }
    
        } else {
            
                //int src_rank = 0; int tag = 1;
                MPI_Status status;
                MPI_Recv(&degree, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //printf( "MPI message received at %d: %d\n", mpi_rank, msg );

        }
        printf("Degree is for worker %d = %d\n", myrank, degree);
        int wtf = nbrOfEdges/nbr_mpi_proc;
        int *scatterGraph;
        scatterGraph = (int *) malloc(wtf * 3 * sizeof(int *));

        /* Now let's scatter the test matrix*/
       
        MPI_Scatter(graph, wtf*3, MPI_INT,scatterGraph, wtf*3, MPI_INT, 0, MPI_COMM_WORLD);
        free(graph);

        unsigned int shortestDist[nbrOfVertices];
        int visited[nbrOfVertices];
        int currentNode;
        
        for (i = 0; i < nbrOfVertices; i++)
        {
            shortestDist[i] = INF;
            visited[i] = 0;
        }
        currentNode = source;
        shortestDist[currentNode] = 0;
        visited[currentNode] = 1;

        while( currentNode != target )
        {

            for (i = 0; i < wtf*3; i++)
            {
                if( shortestDist[currentNode] > scatterGraph[i]) 
                    ;
            }

        }

        printf("Shortest distance: %d\n", shortestDist[currentNode]);
        
        free(scatterGraph);
    }
    MPI_Finalize();       /* cleanup MPI */
    return 0;
}