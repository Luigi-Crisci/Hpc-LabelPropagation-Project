<br/>
<p align="center">
    <br/>
    <b>Optimisation Label Propagation</b>
    <br/>
    <b>Department of Computer Science</b>
    <br/>
    <b>University of Salerno</b>
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
- [ ] Writing the report(main.tex).

### Test size

- Small: 5000 x 300
- Medium: 10000 x 600
- Big: 20000 x 1200

#### Density(p)

- 0.3
- 0.5
- 0.7

### Benchmarks

- Size: 20000 vertex, 1200 nodes
- p: 0.5

| Version | Implementation                          | Running Time  | Relative Speedup | Absolute Speedup|
|---------|-----------------------------------------|---------------|-------------------|-------------------|
| 1       | Julia                                   | 1091,54s      | 1                 | 1                 |
| 2       | C++ Naive                               | 774,40s       | 1,41              | 1,41              |
| 3       | + optimization flags                    | 163,95s       | 4,72              | 6,66              |
| 4       | + data structure changes & BFS on Graph | 10,63s        | 15,42             | 102,68            |
| 5       | + OpenMP(4t)                            | 5,56s         | 1,91              | 196,32            |
| 6       | + padding alignement                    | 5,89s         | 0.94              | 185,32            |
