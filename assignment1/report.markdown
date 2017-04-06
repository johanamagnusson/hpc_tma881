# Assignment 1 TMA881
## Holger Lindström, 930916, holgerlindstrom93@gmail.com
## Andreas Magnusson, 900410, andreas.magnusson90@gmail.com
## Rickard Johansson, 930415, rickjoh@student.chalmers.se

# Time

When running our sum program with the different flags from our makefile, we get the following estimates when taking an average over 1000 times:

Time -O0 2986.69 ms

Time -O1 336.06 ms

Time -O2 0.00039 ms

Time -O3 0.00036 ms

Time -Os 0.00036 ms

Time -Og 333.92 ms

As we can see, -O0 is in a instance for itself (terrible), -O1 and -Og is average and -O2 -O3 and -Os is very good.

When looking into the assembler files, this is what we see in timeO0.s:
```
.L5:
    movq    $0, -72(%rbp)
    call    clock
    movq    %rax, -32(%rbp)
    movq    $1, -64(%rbp)
    jmp .L3
.L4:
    movq    -64(%rbp), %rax
    addq    %rax, -72(%rbp)
    addq    $1, -64(%rbp)
.L3:
    movq    -64(%rbp), %rax
    cmpq    -40(%rbp), %rax
    jbe .L4
    call clock
```
As we can see, every time we add a new value in our loop we also make a movq (move a 64-bit) which takes a lot of time compared to for example add-operators. Doing this one billion times takes a substantial amount time.

In -O1 and -Og

timeO1.s:
```
.L3:
    call    clock
    movq    %rax, %rbp
    movl    $1, %eax
.L2:
    addq    $1, %rax
    cmpq    $1000000001, %rax
    jne .L2
    call    clock
```

timeOg.s:
```
.L5:
    call    clock
    movq    %rax, %r12
    movl    $1, %eax
    movl    $0, %ebx
    jmp .L3
.L4:
    addq    $rax, %rbx
    addq    $1, %rax
.L3:
    cmpq    $1000000000, %rax
    jbe .L4
    call    clock
```
Here we can see that we don't make any movq or movl in the loop which is good. The only thing that really takes time is the initial movq and movl, and then the actual loop with addq and compare.

Now for the best ones, -O2, -O3 and -Os, the assembler code around call clock all look the same:

```
.L2:
    call    clock
    movq    %rax, %rbp
    call    clock
```

The movq we make here is more or less only for the clock function itself. As we can see, no add is made and no compare, so basically the sum is not done. But for these optimizations the compiler is clever enough to realize that we are making an arithmetic sum, so it already knows what the answer will be. Therefore the only thing the clock calls  measures is the clock itself and the time measurement here is not really saying anything about the performance. If we used bash time instead we would have gotten what actually takes time for this program, which are the allocations.

# Inlining

When running the code, measuring the time it takes for multiplying in the main file using a function in the main file and a separate file for the function, we get the following measurements:

Time measured when multiplying directly in code: 0.225 ms

Time measured when multiplying in a function in the main file: 0.516ms

Time measured when multiplying in a separate file: 0.294ms

The time was calculated by doing the multiplication for the 30000 values 10000 times and then taking the average.

As we can see doing it in the same file is the fastest way. Since this is a simple multiplication, it is not so strange that this is the fastest way. About the other two we do not really know why it is faster to call on a function in a separate file then a function in the main file. It might be that when we include the separate file, we get to know the functions of that file, and it is faster to find those functions implemented then those we have to "search" for in the main file.

# Locality

The average time for the functions row\_sum, col\_sum and fast\_col\_sum (the faster implementation of col\_sum) were averaged over 100 runs each. This was done for with the flags: -O0 and -O3 and the matrix sizes: 1000\*1000 and 10000\*10000. The results are shown in the following table:  

| Function     | Average time (flag: -O0, size: 1000) | Average time (flag: -O3, size: 1000) | Average time (flag: -O0, size: 10000) | Average time (flag: -O3, size: 10000) |
|--------------|--------------------------------------|--------------------------------------|---------------------------------------|---------------------------------------|
| row\_sum      | 3.014390 ms                          | 0.981410 ms                          | 303.767630 ms                         | 114.247040 ms                         |
| col\_sum      | 3.269760 ms                          | 1.20576 ms                           | 531.859950 ms                         | 376.524280 ms                         |
| fast\_col\_sum | 3.021940 ms                          | 0.992780 ms                          | 312.184540 ms                         | 118.912390 ms                         |

Running the functions in different order gave different results due to cold cache. Therefore loops without timings were first run to make the cache hot and the measurements non biased. As expected the -O3 flag performed better than -O0. That the function col\_sum performed worst was also expected. That is because the rows of the matrix are stored after each other in memory. The L1d cache for Ozzy is 32kB and can therefore store four rows of the matrix at a time (sizeof(double)\*1000 = 8kB). col\_sum will as for one value from each row of the matrix at a time. Since only four matrix rows are stored in L1d the rest of the matrix will be found in L2 and L3 and slow down the process of fetching data. row\_sum and fast\_col\_sum are faster because they are able to directly fetch the matrix values from L1 since they only use one matrix row at a time. 
With larger matrix size the time difference increases between the fast and the naive implementation of summing the columns. This is because that even more data has to be fetched from memory of higher level. 
Even if all the result were anticipated, the time difference between the fast and naive implementations were expected to be larger since it takes 5 more clock cycles to fetch data from L3 than from L1. 

# Indirect addressing
Running the program with O0 and O3 gives the following output:
```
O0 (no optimization)
--- Indirect addressing ---
Time measured: 4.54487520 ms

--- Direct addressing ---
Time measured: 2.79877630 ms

O3 (full optimization)
--- Indirect addressing ---
Time measured: 0.92400360 ms

--- Direct addressing ---
Time measured: 0.36380260 ms
```
As expected it runs faster with full optimization, but using direct addressing as opposed to indirect also have a significant effect on speed. The reason for this is that for direct addressing the address is contained in the instruction as while for indirect addressing it is stored in a register and therefore has to be fetched before used. To read from the register obviously takes more time.
(To acquire this information we used Google and found a very nice post on Quora explaining it to us. The link to the answers: https://www.quora.com/What-is-the-difference-between-a-direct-and-an-indirect-address-instruction)


# Valgrind

When running the program, we get the following output:
```
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
```

Here, we can see under HEAP SUMMARY that there still is some memory in use when the program is done. Since this is 4000 bytes, we know that this is exactly the memory we allocated. The total heap usage is for our for loop and the summation.

Under LEAK SUMMARY, we can see that the space we allocated is definitely lost. When reading up on valgrind, definitely lost means that there is no pointer to this block when the file is done. This is not so strange, since we only allocate the space, but do not use it.

# Profiling

##gprof

gprof predicts that the program will take 1.91 seconds. Running the program with bash time gives run time 0.135 seconds which is much faster than predicted. gprof also predicts that col\_sum, row\_sum will and fast\_col\_sum will take respectively 3.25 ms, 3.25 ms and 3.05 ms. This is very close to the result given in the table in section locality (for -O0 and size 1000).

##gcov

The lines of code that are run second nestled for-loop int the sums functions (lines 7, 17 and 27). They are all run 200200000 times. The reason why the line initializing the loop is run more than the lines inside the code (200000000) is because at the last iteration of the loop, the if statement that terminates the loop is runs and makes sure to skip the code inside the loop. 








