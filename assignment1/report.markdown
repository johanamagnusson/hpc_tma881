# Assignment 1 TMA881
## Holger Lindström, 930916, holgerlindstrom93@gmail.com
## Andreas Magnusson, 900410, andreas.magnusson90@gmail.com
## Rickard Johansson, 930415, rickjoh@student.chalmers.se

# Time

# Inlining

When running the code, measuring the time it takes for multiplying in the main file, in a function in the main file and a separate file for the function, we get the following measurments:

Time measured when multiplying directly in code: 0.225 ms

Time measured when multiplying in a function in the main file: 0.516ms

Time measured when multiplying in a separate file: 0.294ms

The time was calculated by doing the multiplication for the 30000 values 10000 times, and then taking the averege.

As we can see, doing it in the same file is the fastes way. Since this is a simple multiplication, it is not so strange that this is the fastes way. About the other two, we do not really know why it is faster to call on a function in a separate file then a function in the main file. It might be that when we include the separate file, we get to know the functions of that file, and it is faster to find those functions implemented then those we have to "search" for in the main file.

# Locality

# Indirect adressing

# Valgrind

# Profiling
