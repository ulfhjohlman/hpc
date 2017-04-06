# Assignment 1
## Ulf Hjohlman
### Time
Using optimization level zero the program runs a couple of magnitudes slower, while using any of the other optimization levels results in about equal performance. Taking a look at the assembler code reveals the reason why; with optimization levels above zero the complier seems to figure out that the loop can be calculated at once with just a few lines of instructions. This is in contrast to optimization level zero where there are multiple move, add, and jump instructions in what appears to be the inefficient loop structure that we originally programmed. We also see there is almost no difference between the assembler programs when using the other optimization flags, and thus their similar performance is understandable.

### Inlining
The complex math was handled using the standard complex.h library, and rand() was used to generate the real and imaginary parts of each b and c entry. When doing the computations with the function in an external file the total runtime for running the program 3000 times was around 22.5s, with a function in the same file 22.0s, and with inlining by hand it was reduced to around 21.1s. Much of the runtime is expended when generating the arrays, and the exact performance improvement of using inlining is therefore greater than this. This was all using -O0 optimization and the results are expected as we are simply removing superfluous intermediate steps from the calculation.

When using higher level optimization the differences between the differences are much smaller but fastest by a small margin is the call to the function in the same file, followed by manual inlining, followed by function in an external file. Here the compiler has more freedom to do what it wants and the resulting code can thus be expected to be a lot more optimized and similar, which seems to be the case.

### Locality
Looping over the deeper nested index of the matrix first seems to slow down the calculations. In order to avoid this in col_sums2, we instead loop over the row index first and cumulatively add to each element's respective columnsum. This gives comparable performance to the row_sums function. We can also note that with the higher optimization flags the difference is minimal as the compiler figures out this locality optimization by itself.  

| 300 Runs each                        | -O0   | -O1  | -O2  | -O3  |
|--------------------------------------|-------|------|------|------|
| Initialization + row_sums + col_sums | 11.2s | 9.2s | 5.9s | 5.5s |
| Initialization + row_sums            | 6.7s  | 5.8s | 5.5s | 5.4s |
| Initialization + col_sums            | 10.3s | 8.9s | 5.4s | 5.4s |
| Initialization + col_sums2           | 6.7s  | 5.8s | 5.7s | 5.6s |

### Indirect Addressing

| 5000 Runs each                                           | -O0   |  -O3  |
|----------------------------------------------------------|-------|-------|
| Initialization + indirect_addressing + direct_addressing | 16.9s | 16.3s |
| Initialization + indirect_addressing                     | 16.1s | 15.8s |
| Initialization + direct_addressing                       | 15.8s | 15.6s |


### Valgrind

Valgrind tells us 'definitly lost: 4,000 bytes in 1 blocks' which corresponds to the 1000 ints (of 4 bytes each) we allocated and never freed in the code.

### Profiling
Using 'gprof' we once again see the difference in the row and column sum implimentations. Running all three versions 1000 times the time of the overall execution time taken by each sub function is listed in the tabel below (where col_sums2 is the faster implementaion)

| Averaged over 1000 runs |  -O0  |
|-------------------------|-------|
|row_sums                 |  16%  |
|col_sums                 |  45%  |
|col_sums2                |  19%  |

Looking at gcov we can see the number of times each line of code is executed, which is not all to suprising.
