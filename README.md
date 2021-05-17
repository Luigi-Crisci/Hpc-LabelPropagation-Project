# Hpc-LabelPropagation-Project

## Test

### To-do list

- [x] Manage different code versions(release github or cmake);
- [ ] First parallelisation with OpenMp(for) assai lazy;
- [ ] Profile time spent in memory access(componente connessa);
- [ ] Optimisation of data structures;
- [ ] Other optimisation(ask);
- [ ] Perform the tests;
- [ ] Producing charts;
- [ ] Making a presentation;
- [ ] Writing the report(main.tex).

### Test size

- Small: 1000 x 60
- Medium: 5000 x 300
- Big*: 20000 x 1000
- Big: 50000 x 3000

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
