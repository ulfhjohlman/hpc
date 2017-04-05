# Assignment 1
## Ulf Hjohlman
### Time
Using optimization level zero the program runs a couple of magnitudes slower, while using any of the other optimization levels results in about equal performance. Taking a look at the assembler code reveals the reason why; with optimization levels above zero the complier seems to figure out that the loop can be calculated at once with just a few lines of instructions. This is in contrast to optimization level zero where there are multiple move, add, and jump instructions in what appears to be the inefficient loop structure that we originally programmed. We also see there is almost no difference between the assembler programs when using the other optimization flags, and thus their similar performance is understandable.

### Inlining
The complex math was handled using the standard complex.h library, and rand() was used to generate the real and imaginary parts of each b and c entry. When doing the computations in an external function the total runtime for running the program 3000 times was around 21s, and with inlining by hand it was reduced to around 20s. In other words it was around 5% faster. Much of the runtime is probably expended when generating the arrays, and the exact performance improvement of using inlining is therefore greater than this.

### Locality
Looping over the deeper nested index of the matrix first seems to slow down the calculations. In order to avoid this in col_sums2, we instead loop over the row index first and cumulatively add to each element's respective columnsum. This gives comparable performance to the row_sums function. We can also note that with the higher optimization flags the compiler figures out this locality optimization by itself.  

| 300 Runs each                        | -O0   | -O1  | -O2  | -O3  |
|--------------------------------------|-------|------|------|------|
| Initialization + row_sums + col_sums | 11.2s | 9.2s | 5.9s | 5.5s |
| Initialization + row_sums            | 6.7s  | 5.8s | 5.5s | 5.4s |
| Initialization + col_sums            | 10.3s | 8.9s | 5.4s | 5.4s |
| Initialization + col_sums2           | 6.7s  | 5.8s | 5.7s | 5.6s |

### Indirect Addressing

### Valgrind

### Profiling
