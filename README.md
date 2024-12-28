# Parallel BFS
Calculates the optimal path of galaxies to stop at while travleing across the universe using parallel BFS on CUDA.  

# Details
`EdgeCreator.py` uses `RA`, `DEC` and `light years from Earth` to compute a graph of galaxies that can be travelled between.  
An example dataset generated in Python:  
![](https://raw.githubusercontent.com/Tyler-Hilbert/SpaceTripPlanner/master/Report/Images/GraphVisual.png)

`ParallelBFS.cu` performs a BFS in parallel in CUDA to find the best path between 2 given galaxies.   

# Report  
![](https://raw.githubusercontent.com/Tyler-Hilbert/SpaceTripPlanner/master/Report/Images/RuntimeComparison.png)
[The report can be found here.](https://github.com/Tyler-Hilbert/SpaceTripPlanner/blob/master/Report/Report.pdf)  

# Notes  
The project refactored into Metal (MSL) https://github.com/Tyler-Hilbert/GPU-Metal-DistanceCalc  
