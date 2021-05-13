#include <map>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "mtrnd.h"

#define GENRANDOM(rng) static_cast<unsigned long>(rng.genrand_real1() * RAND_MAX)
#define IS_EDGE_EMPTY(h, e) get_vertices_number(h, e) == 0 ? true : false

#define IS_CONNECTED_TO_EDGE(h, v, e) h->v2he[v]->count(e) == 1 ? true : false
#define IS_CONNECTED_TO_VERTEX(h, e, v) h->he2v[e]->count(v) == 1 ? true : false
#define GET_EDGES(h, v) h->v2he[v]
#define GET_VERTICES(h, e) h->he2v[e]



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
    std::map<int, bool> **v2he, **he2v;

    HyperGraph(int nVertex, int nEdge)
    {
        this->nVertex = nVertex;
        this->nEdge = nEdge;
        v2he = (std::map<int, bool> **)calloc(nVertex, sizeof(std::map<int, bool> *));
        he2v = (std::map<int, bool> **)calloc(nEdge, sizeof(std::map<int, bool> *));

        for (int i = 0; i < nVertex; i++)
            v2he[i] = new std::map<int, bool>;
        for (int i = 0; i < nEdge; i++)
            he2v[i] = new std::map<int, bool>;
    }

} HyperGraph;

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

int get_vertices_number(HyperGraph *h, int edge);

int compute_vertex_label(HyperGraph *h, int v, std::unordered_map<int, int> *vlabel, std::unordered_map<int, int> *heLables, MT::MersenneTwist rng);

int compute_edge_label(HyperGraph *h, int e, std::unordered_map<int, int> *vlabel, std::unordered_map<int, int> *heLables, MT::MersenneTwist rng);

void get_connected_component(HyperGraph *h, std::vector<int> *visited, std::vector<int> *connected_comp, int v);

bool is_hypergraph_connected(HyperGraph *h);

void free_hypergraph(HyperGraph *s);

void print_find_communities_struct(find_communities_struct *res);

void free_hypergraph(HyperGraph *s);

int *collapse_map(std::map<int, bool> *map);