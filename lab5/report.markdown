#Assignment 5
##Ulf Hjohlman
###Program Description
The program runs Dijkstra's algorithm with the help of MPI.

First MPI initialization is done, and the graph attributes are parsed from the file name in a fairly crude manner (expects the input graph to follow the example graphs `../graph_deX_neX_weX` naming convention) to find degree and vertex number. Then the root process reads the input graph and scatters it to the workers in equal chunks, giving each worker a subset of the vertices to watch over. Next the program does a bunch of initialization of data before starting with Dijkstra's algorithm.

Dijkstra's algorithm is performed as normal with the addition of the nodes being split up between workers and therefore requiring some messaging to relay the relevant data between iterations. The worker caring for the `currentVert` to be expanded will locally mark the vertex as visited and broadcasts it's edges. All workers then look through the edges to see if they go to one of their local vertices and if so they check if this is a new shortest path to their vertex. If so they update it with a new distance and `localFromVert` in order to later rebuild the shortest path recursively.

Next each worker locally finds its unvisited vertex with the lowest distance from the start. The global is then found through a reduction call, and then used as the next vertex to expand.

Finally when reaching the `stop` vertex as the next vertex to expand the shortest path has been found and the program prints the path and length before terminating.

The program passed all performance goals with ease and found the same paths as the example program linked on the course homepage.
