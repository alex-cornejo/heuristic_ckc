# ckc-heuristic
A constructive heuristic for the Capacitated Vertex K-Center Problem

The first time it is need to install the conan package manager with the respective libraries included in the conanfile.txt

# Install conan and libraries

```
pip3 install conan
```

## Reload local path
```
source ~/.profile
```

## Compiler configuration for GCC compiler >= 5.1 :
```
conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default
```

## Install conan libraries
```
mkdir build && cd build
conan install ..
```

NOTE: Previous steps only are needed to be executed once. Once conan libraries are installed, to compile and run the program, get back to the root project folder with ```cd ..```

# Install cmake
```
sudo apt install cmake
```

# Install MPICH for parallel computing
```
sudo apt install libmpich-dev
```

# Compile program

```
cmake -DCMAKE_BUILD_TYPE=Release .
cmake --build .
```
The binary file will be generated onto the "bin/" folder. Get onto it with ```cd bin/```

# Run

```
./bin/heuristic_ckc [file] [n] [k] [L] [ind_rep] [rep] [print] [instance_format]
```

## Where,

|  Parameter |                                          Description                                          |
|----------|---------------------------------------------------------------------------------------------|
| `[file]` | (string) Instance file path with a valid format, defined here                                    |
| `[n]`    | (integer) Number of vertices  |
| `[k]`    | (integer) Number of centers   |
| `[L]`    | (integer) Uniform capacity    |
| `[ind_rep]`    | (integer) Independent repetitions of the algorithm, for sensitivity analysis |
| `[rep]`    | (integer) Number of repetions of the algorithm. The best of rep is returned by the algorithm. |
| `[print]`    | (string) If true, prints the solutions (centers and assigned centers). Otherwise, only prints the solution size |
| `[instance_format]`    | (string) Type of instance to use ('tsplib' or 'orlib' are supported). |

# Example 1
```
./bin/heuristic_ckc dataset/URDI-3000/URDI-3000-01.tsp 3000 20 149 10 1 false tsp lib
```

### Output information
The execution report a output with the following relevant information:

`bestFitness`, `averageFitness`, `standardDeviation`, `timePerRunning`
```
1818,2297.9,203.745,7.03125
```
# Example 2 (execution with 3 cores)
```
mpirun -np 3 ./bin/heuristic_ckc dataset/URDI-3000/URDI-3000-01.tsp 3000 20 149 10 1 false tsplib
```

### Output information
```
1818,2297.9,203.745,3.65
 ```
  
# Example 3 (printing best found solution)
```
mpirun -np 3 ./bin/heuristic_ckc dataset/URDI-150/URDI-150-01.tsp 150 5 29 10 1 true tsplib
```

### Output information
```
1891,2067.6,226.917,0.0015625
{"centers":[{"center":120,"nodes":[109,0,107,30,129,106,145,60,86,63,87,131,16,47,77,108,20,22,24,144,73,133,28,126,139,99,41,29,36]},{"center":11
1,"nodes":[37,7,14,42,25,2,104,21,12,78,114,19,130,66,68,32,80,110,118,125,103,136,97,34,69,79,8,65,4]},{"center":59,"nodes":[116,61,62,121,53,15,
134,94,31,112,148,117,101,102,132,124,135,84,38,146,142,70,140,81,11,17,10,113,18]},{"center":71,"nodes":[13,85,143,26,56,74,52,147,27,137,100,51,
35,83,141,67,23,40,88,6,93,57,44,76,105,49,9,122,127]},{"center":92,"nodes":[1,5,33,39,43,45,46,48,50,54,55,58,64,72,75,82,89,90,91,3,95,96,98,115
,119,123,128,138,149]}],"instance":"dataset/URDI-150/URDI-150-01.tsp","type":"cvkp"}
 ```
