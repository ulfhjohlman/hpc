#Assignment 4
##Ulf Hjohlman
###Program Description
The program is very straight forward. It starts by reading input and initiallizing all the relevant data structures and OpenCL objects. In the event of an even sized lattice the four central elements are set to a fourth of `i` each. 

The kernel is stored as a string within the `main.c` file and loaded directly. It works by checking if the neighbouring cells are outside the border, if so it stores their local value as zero. It the computes the diffusion for the current cell. The `clEnqueueNDRangeKernel` function is called as 1-dimensional, and the kernel works in 1-dimension by calculating the neighbour indexes from width and height of the lattice. 

Two buffer arrays are used to store the new and old values of the lattice, and they are interchanged between each call to the kernel. Once eough iterations have been computed the result is read from one of these buffer array, and the averages are computed CPU side non-parallel.

Initially the idea was to start make the kernel 2D and not use any local representation of it's neighbours, but instead use a buffering 0 border. However the given benchmarks were passed without issue with this setup. As there was no test script for this exercise the results were compared to what the example program produced which seems to be in accordance to around 4 significant digits. This deviation is assumed to be due to rounding errors and different use of `float` or `double` precision within the program.
