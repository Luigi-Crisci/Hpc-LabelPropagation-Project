# Hpc-LabelPropagation-Project

## Test
### To-do list

- [x] Manage different code versions(release github or cmake);
- [x] Funciton profiling
- [ ] Optimisation of data structures;
  - [x] BFS with bitset (get_connected_component) (**OPT 2**)
  - [ ] Compute Edge and Vertex Label with bitset
- [ ] First parallelisation with OpenMp(for) assai lazy;
- [ ] Specific parallel optimization
  - [ ] Parallel BFS
  - [ ] Array Padding (check if there is false sharing)
- [ ] Other optimisation(ask);
- [ ] Perform the tests;
- [ ] Producing charts;
- [ ] Making a presentation;
- [ ] Writing the report(main.tex).

### Test size

- Small: 5000 x 300
- Medium: 10000 x 600
- Big: 20000 x 1000

### Test overview

- Run test on Julia
- Run test on C++ (Naive)

### Test Result Julia

|  Vertex  |      Edges      | p   | Total time |
|:--------:|:---------------:|:---:|:-----------|
|  1000    |        60       | 0.3 |            |
|    5000  |       300       | 0.3 |            |
|   50000  |       3000      | 0.3 |            |

### Test Result C++

Media


#### Basic

|  Vertex  |      Edges      | p   | Total time |
|:--------:|:---------------:|:---:|:-----------|
|  1000    |        60       | 0.3 |            |
|    5000  |       300       | 0.3 |            |
|   50000  |       3000      | 0.3 |            |

#### OpenMP

|  Vertex  |      Edges      | p   | Total time |
|:--------:|:---------------:|:---:|:-----------|
|  1000    |        60       | 0.3 |            |
|    5000  |       300       | 0.3 |            |
|   50000  |       3000      | 0.3 |            |

#### Authors

- Luigi Crisci
- Domenico Liguoro
- Giuseppe Di Palma
