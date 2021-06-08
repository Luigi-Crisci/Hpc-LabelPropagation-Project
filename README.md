<br/>
<p align="center">
    <br/>
    <b>Optimisation Find communities</b>
    <br/>
    <b>Department of Computer Science</b>
    <br/>
    <b>Project of high performance computing</b>
    <br/>
    <b>University of Salerno</b>
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

##### Julia

To run the julia code as a script:

```bash
$ julia add_dependencies.jl

$ julia run.jl <vertex_num> <edge_num> <density_prob> 
```
and the result will be printed on standard output.

##### C++

```bash
$ git clone https://github.com/Luigi-Crisci/Hpc-LabelPropagation-Project.git 

$ cd Hpc-LabelPropagation-Project/LabelPropagation-C++

$ ./compile.sh
```

##### Create file for test

In the `cd /resources` folder there are files ready for testing. But if you want to create other file types:

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

Example:

```bash
Usage: 
        bin/benchmark_opt_1 20000 1200 0.5
```

### Benchmarks

We ran various benchmarks with different file sizes and densities. Below are the values on one specific size only:

- Size: 20000 vertex, 1200 nodes, p: 0.5

| Version | Implementation                          | Running Time  | Relative Speedup | Absolute Speedup|
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
- [x] Optimisation of data structures;
  - [x] BFS with bitset (get_connected_component) (**OPT 2**)
  - [x] Compute Edge and Vertex Label with bitset
- [x] First parallelisation with OpenMp(lazy);
- [x] Specific parallel optimization
  - [x] Parallel BFS
  - [x] Array Padding (check if there is false sharing)
- [x] Other optimisation(ask);
- [x] Perform the tests;
- [x] Producing charts;
- [x] Making a presentation(25 May);
- [x] Writing the report(main.tex).
