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

# Compile program

```
$ cmake -DCMAKE_BUILD_TYPE=Release .
$ cmake --build .
```
The binary file will be generated onto the "bin/" folder. Get onto it with ```cd bin/```

# Run

```
$ ./ckc_heuristic [file] [n] [k] [L] [out_perc] [multicenter] [ind_rep] [rep] [print] [instance_format]
```

## Where,

|  Parameter |                                          Description                                          |
|----------|---------------------------------------------------------------------------------------------|
| `[file]` | (string) Instance file path with a valid format, defined here                                    |
| `[n]`    | (integer) Number of vertices  |
| `[k]`    | (integer) Number of centers   |
| `[L]`    | (integer) Uniform capacity    |
| `[out_perc]` | (float) Percentage of outliers between 0 and 1  |
| `[multicenter]` | (string) multicenters allowed = true; otherwise false  |
| `[ind_rep]`    | (integer) Independent repetitions of the algorithm, for sensitivity analysis |
| `[rep]`    | (integer) Number of repetions of the algorithm. The best of rep is returned by the algorithm. |
| `[print]`    | (string) If true, prints the solutions (centers and assigned centers). Otherwise, only prints the solution size |
| `[instance_format]`    | (string) Type of instance to use ('tsplib' or 'orlib' are supported). |

# Example
```
$ ./ckc_heuristic 'pr124.tsp' 124 10 13 0 false 1 1 false tsplib
```

### Output information
The execution report a output with the following relevant information:

```
Average size: 4086.03
Standard deviation: 0

Total time: 0.002204
Time per running: 0.002204
```
# Another example

```
$ ./ckc_heuristic 'pr124.tsp' 124 10 13 0.03 false 1 1 true tsplib
```

### Output information
The execution reports the following relevant information:

```
Average size: 4086.03
Standard deviation: 0

Total time: 0.003079
Time per running: 0.003079

{"centers":[{"center":0,"nodes":[29,33,32,21,28,24,22,34,23,38,35,58,37]},
{"center":4,"nodes":[0,7,1,2,3,6,5,26,4,31,27,25,30]},
{"center":10,"nodes":[12,50,11,49,10,13,14,48,47,9,46,8,15]},
{"center":18,"nodes":[36,39,54,20,19,55,18,17,40,16,41,42,43]},
{"center":57,"nodes":[60,56,57,59,61,62,63,65,64,66,82,80,83]},
{"center":72,"nodes":[123,122,121,99,98,97,96,95,94,93,92,91,79]},
{"center":75,"nodes":[67,68,69,70,71,72,73,74,75,76,77,78,81]},
{"center":76,"nodes":[114,115,116,117,118,119,120]},
{"center":85,"nodes":[110,111,112,103,113,102,101,88,100,87,90,89,86]},
{"center":87,"nodes":[52,84,85,104,105,106,107,108,109,44,45,51,53]}],
"instance":"../dataset/a280.tsp","outliers":[]}
 ```
  

