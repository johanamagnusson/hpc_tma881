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

When running the program, we get the following output:

==24266== Memcheck, a memory error detector
==24266== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==24266== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==24266== Command: ./leak
==24266== Parent PID: 24265
==24266==
==24266==
==24266== HEAP SUMMARY:
==24266==     in use at exit: 4,000 bytes in 1 blocks
==24266==   total heap usage: 2 allocs, 1 frees, 5,024 bytes allocated
==24266==
==24266== 4,000 bytes in 1 blocks are definitely lost in loss record 1 of 1
==24266==    at 0x4C2DB8F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==24266==    by 0x400577: alloc (in /home/hpc10/hpc_tma881/assignment1/valgrind/leak)
==24266==    by 0x400598: main (in /home/hpc10/hpc_tma881/assignment1/valgrind/leak)
==24266==
==24266== LEAK SUMMARY:
==24266==    definitely lost: 4,000 bytes in 1 blocks
==24266==    indirectly lost: 0 bytes in 0 blocks
==24266==      possibly lost: 0 bytes in 0 blocks
==24266==    still reachable: 0 bytes in 0 blocks
==24266==         suppressed: 0 bytes in 0 blocks
==24266==
==24266== For counts of detected and suppressed errors, rerun with: -v
==24266== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

Here, we can see under HEAP SUMMARY that there still is some memory in use when the program is done. Since this is 4000 bytes, we know that this is exactly the memory we allocated. The total heap usage is for our for loop and the summation.

Under LEAK SUMMARY, we can see that the space we allocated is definitly lost. When reading up on valgrid, definitly lost means that there is no pointer to this block when the file is done. This is not so strange, since we only allocate the space, but do not use it.

# Profiling
