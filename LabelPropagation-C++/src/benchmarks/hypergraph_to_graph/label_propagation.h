#include <map>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <iterator>
#include <assert.h>
#include <iostream>
#include <chrono>
#include <utility>
#include <fstream>
#include <sstream>
#include "random"
#include <bitset>

#define GENRANDOM(rng) static_cast<unsigned long>(rng.genrand_real1() * RAND_MAX)
#define IS_EDGE_EMPTY(h, e) get_vertices_number(h, e) == 0 ? true : false
#define IS_CONNECTED_TO_EDGE(h, v, e) h->v2he[v]->count(e) == 1 ? true : false
#define IS_CONNECTED_TO_VERTEX(h, e, v) h->he2v[e]->count(v) == 1 ? true : false
#define GET_EDGES(h, v) h->v2he[v]
#define GET_VERTICES(h, e) h->he2v[e]

#define MAX_SIZE 100000

typedef struct CFLabelPropagationFinder
{
    int max_iter;
    int seed;
} CFLabelPropagationFinder;

/*
    Hyper-graph structure
*/
typedef struct HyperGraph
{
    int nVertex, nEdge;
    std::bitset<MAX_SIZE> *v2he, *he2v;

    HyperGraph(size_t nVertex, size_t nEdge)
    {
        this->nVertex = nVertex;
        this->nEdge = nEdge;
        v2he = (std::bitset<MAX_SIZE> *)calloc(nVertex, sizeof(std::bitset<MAX_SIZE>));
        he2v = (std::bitset<MAX_SIZE> *)calloc(nEdge, sizeof(std::bitset<MAX_SIZE>));

        //TODO: Should this be pointers?
        // for (int i = 0; i < nVertex; i++)
        //     (*v2he)[i] = new std::bitset<nEdge>();
        // for (int i = 0; i < nEdge; i++)
        //     he2v[i] = new std::map<int, bool>;
    }

} HyperGraph;