#pragma once 

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
#include <random>
#include <bitset>

#include "mtrnd.h"

#define GENRANDOM(rng) static_cast<unsigned long>(rng.genrand_real1() * RAND_MAX)
#define IS_CONNECTED_TO_EDGE(h, v, e) h->v2he[v].test(e)
#define IS_CONNECTED_TO_VERTEX(h, e, v) h->he2v[e].test(e)
#define GET_EDGES(h, v) &(h->v2he[v])
#define GET_VERTICES(h, e) &(h->he2v[e])
#define GET_EDGE_VERTICES_CONNECTED_NUMBER(h,e) h->he2v[e].count()
#define GET_VERTEX_EDGES_CONNECTED_NUMBER(h,v) h->v2he[v].count()
#define IS_EDGE_EMPTY(h, e) GET_VERTEX_EDGES_CONNECTED_NUMBER(h,e) == 0 ? true : false

#define MAX_SIZE 100000
typedef struct CFLabelPropagationFinder
{
    int max_iter;
    int seed;
} CFLabelPropagationFinder;

/*
    Hyper-graph structure
*/
struct HyperGraph
{
    int nVertex, nEdge;
    std::bitset<MAX_SIZE> *v2he, *he2v;

    HyperGraph(size_t nVertex, size_t nEdge)
    {
        this->nVertex = nVertex;
        this->nEdge = nEdge;
        v2he = (std::bitset<MAX_SIZE> *)calloc(nVertex, sizeof(std::bitset<MAX_SIZE>));
        he2v = (std::bitset<MAX_SIZE> *)calloc(nEdge, sizeof(std::bitset<MAX_SIZE>));
    }

    ~HyperGraph()
    {
        free(v2he);
        free(he2v);
    }

    void add_vertex_to_edge(int v, int e)
    {
        v2he[v].set(e);
        he2v[e].set(v);
    }

};

typedef struct find_communities_struct
{

    std::unordered_set<std::unordered_set<int> *> *np;
    std::unordered_set<std::unordered_set<int> *> *hep;
    int *vLabel;
    int *heLabel;
    int vLabel_size;
    int heLabel_size;
    int iter;

    find_communities_struct(std::unordered_set<std::unordered_set<int> *> *_np, std::unordered_set<std::unordered_set<int> *> *_hep,
                            int *_vLabel,
                            int vsize,
                            int *_heLabel,
                            int hsize,
                            int _iter)
    {
        np = _np;
        hep = _hep;
        vLabel = _vLabel;
        vLabel_size = vsize;
        heLabel = _heLabel;
        heLabel_size = hsize;
        iter = _iter;
    }

} find_communities_struct;

find_communities_struct *find_communities(HyperGraph *h, CFLabelPropagationFinder parameters);

void shuffle(int *array, int size, MT::MersenneTwist rng);

std::unordered_map<int, std::unordered_set<int> *> *reverse_map(std::unordered_map<int, int> *map);

int compute_vertex_label(HyperGraph *h, int v, std::unordered_map<int, int> *vlabel, std::unordered_map<int, int> *heLables, MT::MersenneTwist rng);

int compute_edge_label(HyperGraph *h, int e, std::unordered_map<int, int> *vlabel, std::unordered_map<int, int> *heLables, MT::MersenneTwist rng);

void get_connected_component(HyperGraph *h, std::vector<int> *visited, std::vector<int> *connected_comp, int v);

bool is_hypergraph_connected(HyperGraph *h);

void free_hypergraph(HyperGraph *s);

void print_find_communities_struct(find_communities_struct *res);

int *get_vertices_indices(std::bitset<MAX_SIZE> *b);
