# Assignment 2 TMA881
## Holger Lindström, 930916, holgerlindstrom93@gmail.com
## Andreas Magnusson, 900410, andreas.magnusson90@gmail.com
## Rickard Johansson, 930415, rickjoh@student.chalmers.se

## Program layout

When we run our program, we dedicate one thread to writing and the rest of the threads to calculate newtons method. We let the calculating threads handle one row each in the system.

When we allocate the memory, we mallocate only 120 rows (so 120 times length of rows times size of int). Then, when we run the program, we let the calculating threads overwrite this memory over and over, always saving the calculations to one of the 120 rows with %120. However, at the same time, we make sure to write the calcualtions to the files before the computations gets overwritten. This handles the saving of memory needed.

To make sure that we never try to access the same memory, we put a lock on the calculators and the writer, never to write or calculate if we are to close to the other. In other words, if the writer gets close to uncalculated rows, or the calculators get close to rows that are not yet written, they pause. This threashhold was set to 40, so there should not be any overlap.

## Benchmarking done by us

When computing our program using the simple time command, looking at the different instances spedified in the assignment, we get the following values:

#Single threaded, 1000 lines, polynomial x^d-1 (here we show the longest time when running ~10 times)

| d     | 1         | 2         | 5         | 7         |
|------ |--------   |--------   |--------   |--------   |
| Time  | 0.151s    | 0.246s    | 0.981s    | 1.216s    |


#Multithreaded, 1000 lines, x^5-1

| Number of threads     | 1          | 2         | 3         | 4         |
|-------------------    |--------    |--------   |--------   |--------   |
| Time                  | 0.981s     | 0.454s    | 0.314s    | 0.367s    |


#Ten threads, varying number of lines, x^7-1

| Number of lines   | 1000      | 50000     |
|-----------------  |--------   |-------    |
| Time              | 0.256s    | 4m36s     |

As we can see, our program computes the pictures within the time marginal.
