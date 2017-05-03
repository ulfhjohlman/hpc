#Assignment 2
##Ulf Hjohlman
###Program Description
The 'main.c' function reads input options, creates the input object with global information shared between the threads, and then creates and launches the specified number of threads along with one write thread. Afterwards it waits for these threads to finish, and exits.

The input object contains the two matrixes 'attractor' and 'nIterations' which store the enumeration of the root a pixel converges to and the number of iterations it takes to do so. These matrixes are allocated to store 'blockrows' number of rows. When the end of These matrixes are reached reading/writing to them loops around and starts at the top again, overwriting the previous data.

The calculation threads check 'nextRowToDo' (managed by a mutex) to find what row to compute next and perform the newton iteration procedure on that row, storing results to the matrixes. In order to not loop around, catch up to the writer thread, and overwrite unrecorded data they also check 'currentWriteRow' and sleep a little if they get to close. In the same way the writer thread prints to file while checking that it does not get within a certain buffer distance to the rows being calculated.

Running on Ozzy all the test requirements were passed, producing what appears to be correct results (although the 50000 row testcase produced image files to big to load and check).
