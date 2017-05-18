# Assignment 2 TMA881
## Holger Lindström, 930916, holgerlindstrom93@gmail.com
## Andreas Magnusson, 900410, andreas.magnusson90@gmail.com
## Rickard Johansson, 930415, rickjoh@student.chalmers.se

## Program layout

When we run our program, we dedicate one thread to writing and the rest of the threads to calculate newtons method.



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
