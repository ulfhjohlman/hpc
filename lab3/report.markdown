#Assignment 2
##Ulf Hjohlman
###Program Description
The program is based around efficiently calculating the distances between points in parallel and increasing a shared counter for each occurence of a distance (what points correlate to each distance is not stored). Parallelization is done with openmp and the shared `count` is split into private objects for each thread and then calculated through reduction at the end in an `#pragma omp critical`. Scheduling is done through `schedule(dynamic)` because early loop iterations will take longer than the later ones. The output is then printed as specified.
