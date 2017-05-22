# Assignment 4 TMA881
## Holger Lindström, 930916, holgerlindstrom93@gmail.com
## Andreas Magnusson, 900410, andreas.magnusson90@gmail.com
## Rickard Johansson, 930415, rickjoh@student.chalmers.se

## Program layout
The first "relevant" choice we make in our program is that we check how much space we need to allocate. Since if the number of iterations is less then the siza of the matrix, we do not need to allocate the whole matrix, but simply the part that in the end will differ from 0.
When this is done, we still allocate the required amount of memory, plus one extra space alongst every "edge". This is so we can use this for updating the temperatures but without having to do a lot of "if checkes" if we are on the ledge or not.
We then let the kernels run over the specified number of iterations.
When calculating the average and standard deviation, we make sure to calculate it for the whole square, regardless if we made a smaller box in the beginning or not.

## Benchmarking done by us

|    Width * Height    | 100 * 100 | 10000 * 10000 | 100000 * 100 |
|:--------------------:|:---------:|:-------------:|:------------:|
|     Initial value    |    1e20   |      1e10     |     1e10     |
|  Diffusion constant  |    0.01   |      0.02     |      0.6     |
| Number of iterations |   100000  |      1000     |      200     |
|    Time in seconds   |   7.86s   |     10.17s    |     6.24s    |

As we can see, the time is a little slow in the first case. However, se do not know how to make it faster. We have been thinking that maybe there is a part in our OpenCL part that can be faster but we don't have the knowledge, but we are not sure.
