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
        //printf("Number of processes: %d\n", nbr_mpi_proc);
        //printf("I am your master!");
        FILE         *graphFile;
        char         *fileName;
        char         ch;
        char         lineStr[128];
        int          i1, i2, w;
        size_t       nbrOfEdges;
        size_t       nbrOfVertices;
        int          *graph;
        size_t       i, j;
        int          ret, lineIdx, degree, wtf, wth;

        const size_t source = atoi(argv[1]);
        const size_t target = atoi(argv[2]);
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
        wth = degree/nbr_mpi_proc;
        graph = (int *) malloc(nbrOfEdges * 3 * sizeof(int));
        if (myrank == 0){ 
            fseek(graphFile, 0, SEEK_SET);
            for (i = 0; i < wtf*3; i += 3)
            {
                for (j = i; j < nbrOfEdges * 3; j += wtf * 3)
                {
                    ret = fscanf(graphFile, "%d", &graph[j]);
                    ret = fscanf(graphFile, "%d", &graph[j+1]);
                    ret = fscanf(graphFile, "%d", &graph[j+2]);
                }
            }
            fclose(graphFile);
        }

        //printf("Degree is for worker %d = %d\n", myrank, degree);
        int *scatterGraph;
        scatterGraph = (int *) malloc(wtf * degree * 3 * sizeof(int));


        /* Now let's scatter the test matrix*/
       
        MPI_Scatter(graph, wtf*3, MPI_INT,scatterGraph, wtf*3, MPI_INT, 0, MPI_COMM_WORLD);
        free(graph);
        /*
        for (i = 0; i < wtf*3; i += 3)
        {
            printf("Rank: %d, %d %d %d\n", myrank,
                    scatterGraph[i],
                    scatterGraph[i+1],
                    scatterGraph[i+2]);
        }
        */
        // Here we begin with Dijkstra 
        unsigned int *dist = (unsigned int *) malloc(nbrOfVertices * sizeof(int));
        int *visited = (int *) calloc(nbrOfVertices, sizeof(int));
        int *prev = (int *) malloc(nbrOfVertices * sizeof(int));
        int *update = (int *) calloc(nbrOfVertices, sizeof(int));
        unsigned int minDist, alt, retDist;
        int currentNode, minNode, idx, nbrVisited, pathLength;
        
        
        for (i = 0; i < nbrOfVertices; i++)
        {
            dist[i] = INF;
        }
        dist[source] = 0;
        nbrVisited = 0;

        //printf("%lu, %lu, %d, %d\n", nbrOfVertices, nbrOfEdges, wtf, wth);

        while( nbrVisited < nbrOfVertices )
        {
            if(myrank == 0)
            {
                minDist = INF;
                for(i = 0; i < nbrOfVertices; i++)
                {
                    if ( dist[i] < minDist && !visited[i]){
                        minDist = dist[i];
                        minNode = i;
                    }
                }
                currentNode = minNode;
                visited[currentNode] = 1;
            }
            
            nbrVisited++;
            MPI_Bcast(visited, nbrOfVertices, MPI_INT, 0, MPI_COMM_WORLD);        
            MPI_Bcast(update, nbrOfVertices, MPI_INT, 0, MPI_COMM_WORLD);        
            MPI_Bcast(&currentNode, 1, MPI_INT, 0, MPI_COMM_WORLD);
            
            for (i = 0; i < wth*3; i+=3)
            {
                idx = currentNode*wth*3 + i;
                alt = dist[currentNode] + scatterGraph[idx+2];
                if ( alt < dist[scatterGraph[idx+1]] && !visited[scatterGraph[idx+1]]){
                    dist[scatterGraph[idx+1]] = alt;
                    update[scatterGraph[idx+1]] = 1;
                }
            }
            
            MPI_Allreduce(MPI_IN_PLACE, dist, nbrOfVertices, MPI_UNSIGNED, MPI_MIN, MPI_COMM_WORLD);
            MPI_Allreduce(MPI_IN_PLACE, update, nbrOfVertices, MPI_INT, MPI_LOR, MPI_COMM_WORLD);

            if (myrank == 0)
            {
                for (i = 0; i < nbrOfVertices; i++)
                {
                    if (update[i])
                    {
                        prev[i] = currentNode;
                        update[i] = 0;
                    }
                }
            }
            
        }

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
            printf("%lu\n", target);
            
            printf("Shortest distance: %d\n", retDist);
        }
        
        free(dist);
        free(visited);
        free(prev);
        free(update);
        free(scatterGraph);
    }
    
    MPI_Finalize();       /* cleanup MPI */
    return 0;
}

