# Assignment 5 TMA881
## Holger Lindström, 9309160738, holgerlindstrom93@gmail.com
## Andreas Magnusson, 900410, andreas.magnusson@gmail.com
## Rickard Johansson, 930415, rickjoh@student.chalmers.se

## Program layout

Since we started by simply doing Dijkstras algorithm, we just let it stand for itself for only one process. Simply reading in the file and then solving the classical Dijkstra.

When looking at multiple processes, we start similarly by reading the file. Then, we split the graph given into the different processes, using MPI Scatter, in such a way that every process holds a fair part of infomation about every node. This is to make the processes work in parallell. 
Now, when applying Dijkstra, we let every process calculate a suggest to what should be the next current node. Then, we gather all the suggestions, and letting everyone know which is the next one, using MPI Allgather with the MPI MINLOC command. This works fine, and we can calculate the distances correctly.

However, for the parallell processes, we are not yet able to find the correct path. We tried in a few ways, all of them either incorrect or to slow. Suggestions of a way would really come in handy!

## Benchmarking done by us

Some benchmarking and paths found by us:


|    File   |            graph_de1_ne3_we2            |          graph_de2_ne4_we2         | graph_de2_ne4_we2 | graph_de2_ne5_we2 | graph_de3_ne5_we3 |
|:---------:|:---------------------------------------:|:----------------------------------:|:-----------------:|:-----------------:|:-----------------:|
| Processes |                    1                    |                  1                 |         4         |         10        |         20        |
|   Start   |                    9                    |                 17                 |         17        |        107        |         4         |
|    End    |                    82                   |                 40                 |         18        |         0         |         5         |
|    Time   |                  0.13s                  |                1.28s               |        1.42       |       58.48s      |       2m26s       |
|    Path   | 9, 433, 742, 313, 41, 668, 360, 379, 82 | 17, 395, 480, 2565, 2807, 1245, 40 |                   |                   |                   |
|  Distance |                   112                   |                 12                 |         8         |         18        |         21        |
