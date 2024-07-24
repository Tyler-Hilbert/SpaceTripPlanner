Calculates the optimal path of galaxies to stop at while travleing across the universe using parallel BFS on CUDA.  

EdgeCreator.py uses RA, DEC and light years from Earth to compute a graph of galaxies that can be travelled between.  
An example dataset generated in python:  
![](https://raw.githubusercontent.com/Tyler-Hilbert/SpaceTripPlanner/master/Report/Images/GraphVisual.png)

ParallelBFS.cu performs a BFS in parallel in CUDA to find the best path between 2 given galaxies.  

These results are compared to a sequential BFS algorithm.  

[The report can be found here.](https://github.com/Tyler-Hilbert/SpaceTripPlanner/blob/master/Report/Report.pdf)  

[(Random blog post that uses this project)](https://medium.com/@tylerjoshuahilbert/chatgpt-as-a-code-comprehension-tool-af86f878ed04)  

The project refactored into Metal https://github.com/Tyler-Hilbert/GPU-Metal-DistanceCalc  
