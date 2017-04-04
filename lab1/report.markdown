# Assignment 1
## Ulf Hjohlman
### Time

### Inlining
The complex math was handeled using the standard complex.h library, and rand() was used to generate the real and imaginary parts of each b and c entry. When doing the compuations in an external function the total runtime for running the program 3000 times was around 21s, and with inlining by hand it was reduced to around 20s. In other words it was around 5% faster. Much of the runtime is probably expended when generating the arrays, and the exact performance improvement of using inlining is greater than this.
