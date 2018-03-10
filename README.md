# Configuration
edit the Makefile to point to your ADIOS and MPI install

# Exercise
edit the files `put.cpp` and `get.cpp` replace `TODO` items with ADIOS code
```
$ make exercise
```
# Testing
## Terminal 1
```
mpiexec -np 2 ./put test.bp FLEXPATH 10 1 1
```
## Terminal 2
```
mpiexec -np 2 ./get test.bp FLEXPATH
```

# Solution
The solution can be made by
```
make solution
```
