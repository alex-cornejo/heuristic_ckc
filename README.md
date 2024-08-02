# OHCKC: A constructive heuristic for the uniform capacitated vertex $k$-center problem

## Citing
### BibTex
```
@article{ohckc2023,
author = {Cornejo-Acosta, Jos\'{e} Alejandro and Garc\'{\i}a-D\'{\i}az, Jes\'{u}s and P\'{e}rez-Sansalvador, Julio C\'{e}sar and R\'{\i}os-Mercado, Roger Z. and Pomares-Hern\'{a}ndez, Sa\'{u}l Eduardo},
title = {A Constructive Heuristic for the Uniform Capacitated Vertex $k$-center Problem},
year = {2023},
issue_date = {December 2023},
publisher = {Association for Computing Machinery},
address = {New York, NY, USA},
volume = {28},
issn = {1084-6654},
url = {https://doi.org/10.1145/3604911},
doi = {10.1145/3604911},
abstract = {The uniform capacitated vertex k-center problem is an 𝒩𝒫-hard combinatorial optimization problem that models real situations where k centers can only attend a maximum number of customers, and the travel time or distance from the customers to their assigned center has to be minimized. This article introduces a polynomial-time constructive heuristic algorithm that exploits the relationship between this problem and the minimum capacitated dominating set problem. The proposed heuristic is based on the one-hop farthest-first heuristic that has proven effective for the uncapacitated version of the problem. We carried out different empirical evaluations of the proposed heuristic, including an analysis of the effect of a parallel implementation of the algorithm, which significantly improved the running time for relatively large instances.},
journal = {ACM J. Exp. Algorithmics},
month = {aug},
articleno = {1.6},
numpages = {26},
keywords = {capacitated k-center, parallel algorithm, Combinatorial optimization, heuristic search}
}
```


**If this implementation is useful for you, please cite our paper.
This implementation should be used just for research/academic purposes.**

The following instructions were tested for Ubuntu 20.04.5 LTS.

## Install c++ boost libraries
```
sudo apt install libboost-all-dev
```

## Install cmake
```
sudo apt install cmake
```

## Install MPICH for parallel computing
```
sudo apt install libmpich-dev
```

## Compile program
```
cmake -DCMAKE_BUILD_TYPE=Release .
cmake --build .
```
The binary file will be generated onto the root folder with the name ```heuristic_ckc```

## Run
```
./heuristic_ckc [file] [n] [k] [L] [ind_rep] [rep] [print] [instance_format]
```

## Where,

|  Parameter |                                          Description                                          |
|----------|---------------------------------------------------------------------------------------------|
| `[file]` | (string) Instance file path with a valid format, defined here                                    |
| `[n]`    | (integer) Number of vertices.  |
| `[k]`    | (integer) Number of centers.   |
| `[L]`    | (integer) Uniform capacity.    |
| `[ind_rep]`    | (integer) Independent repetitions of the algorithm, for sensitivity analysis. |
| `[rep]`    | (integer) Number of repetitions of the algorithm. The best found solution is returned by the algorithm. |
| `[print]`    | (string) If true, prints the solutions. Otherwise, only prints the solution size. |
| `[instance_format]`    | (string) Type of instance to use ('tsplib' or 'orlib' are supported only in the examples format). |

## Example 1
```
./heuristic_ckc dataset/URDI/URDI-3000/URDI-3000-01.tsp 3000 20 149 10 1 false tsplib
```

### output 1
The execution report a output with the following relevant information:

`bestFitness`, `averageFitness`, `standardDeviation`, `timePerRunning`, `timePerRunning` (parallel sections), `averageNeighborhoodSize`
```
2110,2554,191.261,2.5,2.4875,737
```
## Example 2 (execution with 3 cores)
```
mpirun -np 3 ./heuristic_ckc dataset/URDI/URDI-3000/URDI-3000-01.tsp 3000 20 149 10 1 false tsplib
```

### output 2
```
2110,2554,191.261,0.890625,0.876563,737
```
  
## Example 3 (printing best found solution)
```
mpirun -np 3 ./heuristic_ckc dataset/URDI/URDI-150/URDI-150-01.tsp 150 5 29 10 1 true tsplib
```

### output 3
```
1891,2111.5,332.72,0.0015625,0.0015625,40
{"centers":[{"center":120,"nodes":[109,0,107,30,129,106,145,60,86,63,87,131,16,47,77,108,20,22,24,144,73,133,28,126,139,99,41,29,36]},{"center":111,"nodes":[37,7,14,42,25,2,104,21,12,78,114,19,130,66,68,32,80,110,118,125,103,136,97,34,69,79,8,65,4]},{"center":59,"nodes":[116,61,62,121,53,15,134,94,31,112,148,117,101,102,132,124,135,84,38,146,142,70,140,81,11,17,10,113,18]},{"center":71,"nodes":[13,85,143,26,56,74,52,147,27,137,100,51,35,83,141,67,23,40,88,6,93,57,44,76,105,49,9,122,127]},{"center":92,"nodes":[1,5,33,39,43,45,46,48,50,54,55,58,64,72,75,82,89,90,91,3,95,96,98,115,119,123,128,138,149]}],"instance":"dataset/tsplib/URDI-150/URDI-150-01.tsp","type":"cvkp"}
```

## Example 4 (ORLIB instance)
```
mpirun -np 4 ./heuristic_ckc dataset/ORLIB/pmed21.txt -1 -1 99 30 1 false orlib
```
For ORLIB instances, $n$ and $k$ are taken form the instance file.

### output 4
```
47,49.1333,1.52527,0.0121319,0.0116038,211
```

# Contact
- alexcornejo@inaoep.mx 
