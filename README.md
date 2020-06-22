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
| `[ind_rep]`    | (integer) Independent repetitions of the algorithm, for sensitivity analysis |
| `[rep]`    | (integer) Number of repetions of the algorithm. The best of rep is returned by the algorithm. |
| `[print]`    | (string) If true, prints the solutions (centers and assigned centers). Otherwise, only prints the solution size |
| `[instance_format]`    | (string) Type of instance to use ('tsplib' or 'orlib' are supported). |

# Example
```
$ ./ckc_heuristic 'pr124.tsp' 124 10 13 1 1 false tsplib
```

### Output information
The execution report a output with the following relevant information:

```
Average size: 3054.91
Standard deviation: 0

Total time: 0.077
Time per running: 0.077
```
# Another example

```
$ ./ckc_heuristic 'pr124.tsp' 124 10 13 1 1 true tsplib
```

### Output information
The execution reports the following relevant information:

```
Average size: 3054.91
Standard deviation: 0

Total time: 0.394
Time per running: 0.394

{"centers":[{"center":4,"nodes":[0,7,1,2,3,6,5,26,31,27,25,30,28]},{"center":8,"nodes":[15,16,43,44,53]},{"center":9,"no
des":[50,49,12,48,11,14,13,47,10,46,52,51,45]},{"center":23,"nodes":[24,37,21,22,38,39,20,19,18,17,40,41,42]},{"center":
60,"nodes":[82,66,64,61,59,57,58,56,55,35,34,54,36]},{"center":65,"nodes":[29,33,32,62,63,67,68,69,70,71,72,73,74]},{"ce
nter":76,"nodes":[75,77,80,81,83,114,115,116,117,118,119,120]},{"center":78,"nodes":[123,99,98,97,122,96,95,94,93,92,91,
121,79]},{"center":84,"nodes":[85,86,87,89,90,100]},{"center":104,"nodes":[113,112,111,110,109,105,108,106,107,101,102,1
03,88]}],"instance":"\\dataset\\pr124.tsp"}
 ```
  

