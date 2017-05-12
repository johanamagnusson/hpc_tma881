# Assignment 3 TMA881
## Holger Lindström, 930916, holgerlindstrom93@gmail.com
## Andreas Magnusson, 900410, andreas.magnusson90@gmail.com
## Rickard Johansson, 930415, rickjoh@student.chalmers.se

## Program layout
Our solution scans the file and saves the cell coordinates as floats in a
double pointer array. We prepare an integer array, a distance histogram,
large enough to represent all distances that exist with 2 decimals in a 20 by
20 cube, which are 0 to (int) sqrt(20^2+20^2+20^2)\*100 = 3464, meaning a
distance computed to be for example 23.42 is represented by index 2342 in the
array. This distance histogram is also created one per thread to have each
thread build one array each which is to be appended once all distances have
been computed. The computations happen in two separate for-loops, of which the
second has an if-statement to have the iterators i and j independent of one
another so that the OpenMP collapse statement will work. Each iteration a
distance is computed and rounded to an int. The value at this integers spot in
the distance histogram is then increaased by 1. Lastly each thread has a
critical section where the thread's own histogram is added to the total
histogram. The threads are then joined and terminated and the result is
printed.

## Benchmarking done by us
Using the linux "time" command we get the following results (doing at least 10
runs per configuration and presenting the longest time):
Number of points:           1e4     1e5     1e5     1e5
Number of threads           1       5       10      20
Maximal runtime in seconds  0.48    10.93   6.272   3.863
