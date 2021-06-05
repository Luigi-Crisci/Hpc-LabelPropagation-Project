<br/>
<p align="center">
    <br/>
    <b>Optimisation Label Propagation</b>
    <br/>
    <b>Project of high performance computing</b>
    <br/>
    <b>Prof. Biagio Cosenza</b>
    <br/>
    <br />
    <b>Luigi Crisci - Domenico Liguori - Giuseppe Di Palma</b>
    <br/>
    <br/>
</p>

### Goal

Optimise find community in the label propagation package written in Julia.

##### Prerequisites

- Clang++-11
- Cmake 3.16.3
- OpenMP 5.0

#### How to use

```bash
$ git clone https://github.com/Luigi-Crisci/Hpc-LabelPropagation-Project.git 

$ cd Hpc-LabelPropagation-Project/LabelPropagation-C++/build/

$ ./compile.sh
```

##### Create file for test

In `build` directory run:
```bash
$ bin/create_h

Usage: 
        bin/create_h [vertex_number] [edges_number] [p]
```

Example:

```bash
$ bin/create_h

Usage: 
        bin/create_h 1200 600 0.5
```

These files will be created in the `$ cd ../resources/`

*(If this directory does not exist, it must be created before run)*

##### Run

All optimisations require the same type of parameters:

```bash
Usage: 
        bin/benchmark_opt_1 [vertex] [edge] [density]
```

Optimizations:

```bash
$ bin/benchmark_sequential
$ bin/benchmark_opt_1
$ bin/benchmark_opt_2
$ bin/benchmark_opt_3
$ bin/benchmark_opt_4
```

**opt_3** and **opt_4** are multicore, to set the number of cores give the following command (before running) in the command line:

```bash
export OMP_NUM_THREADS=4
```

### Benchmarks

We ran various benchmarks with different file sizes and densities. Below are the values on one specific size only:

- Size: 20000 vertex, 1200 nodes, p: 0.5

| Version | Implementation                          | Running Time​  | Relative Speedup​  | Absolute Speedup​  |
|---------|-----------------------------------------|---------------|-------------------|-------------------|
| 1       | Julia                                   | 1091,54s      | 1                 | 1                 |
| 2       | C++ Naive                               | 774,40s       | 1,41              | 1,41              |
| 3       | + optimization flags                    | 163,95s       | 4,72              | 6,66              |
| 4       | + data structure changes & BFS on Graph | 10,63s        | 15,42             | 102,68            |
| 5       | + OpenMP(4t)                            | 5,56s         | 1,91              | 196,32            |
| 6       | + padding alignment(4t)                 | 5,89s         | 0,94              | 185,32            |

## To-do list

- [x] Manage different code versions(release github or cmake);
- [x] Funciton profiling
- [ ] Optimisation of data structures;
  - [x] BFS with bitset (get_connected_component) (**OPT 2**)
  - [ ] Compute Edge and Vertex Label with bitset
- [x] First parallelisation with OpenMp(lazy);
- [ ] Specific parallel optimization
  - [ ] Parallel BFS
  - [ ] Array Padding (check if there is false sharing)
- [x] Other optimisation(ask);
- [x] Perform the tests;
- [x] Producing charts;
- [x] Making a presentation(25 May);
- [ ] Writing the report(main.tex).