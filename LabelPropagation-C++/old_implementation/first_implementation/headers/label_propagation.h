#include <map>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "mtrnd.h"

typedef std::vector<std::vector<int> *> *Int_Matrix;
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
    Int_Matrix hVertex;

    HyperGraph(int nVertex, int nEdge)
    {
        this->nVertex = nVertex;
        this->nEdge = nEdge;
        hVertex = new std::vector<std::vector<int> *>(nVertex);
        for (int i = 0; i < nVertex; i++)
        {
            (*hVertex)[i] = new std::vector<int>(nEdge);
        }
    }

} HyperGraph;

typedef struct find_communities_struct
{

    std::unordered_set<std::unordered_set<int> *> *np;
    std::unordered_set<std::unordered_set<int> *> *hep;
    std::vector<int> *vLabel;
    std::vector<int> *heLabel;
    int iter;

    find_communities_struct(std::unordered_set<std::unordered_set<int> *> *_np, std::unordered_set<std::unordered_set<int> *> *_hep, std::vector<int> *_vLabel,
                            std::vector<int> *_heLabel, int _iter)
    {
        np = _np;
        hep = _hep;
        vLabel = _vLabel;
        heLabel = _heLabel;
        iter = _iter;
    }

} find_communities_struct;

find_communities_struct* find_communities(HyperGraph *h, CFLabelPropagationFinder parameters);

void shuffle(std::vector<int> *array, MT::MersenneTwist rng);

std::unordered_map<int, std::unordered_set<int> *> *reverse_map(std::unordered_map<int, int> *map);

std::vector<int> *get_edges(HyperGraph *h, int vertices);

std::vector<int> *get_vertices(HyperGraph *h, int edge);

int get_vertices_number(HyperGraph *h, int edge);

int compute_vertex_label(HyperGraph *h, int v, std::unordered_map<int, int> *vlabel, std::unordered_map<int, int> *heLables, MT::MersenneTwist rng);

int compute_edge_label(HyperGraph *h, int e, std::unordered_map<int, int> *vlabel, std::unordered_map<int, int> *heLables, MT::MersenneTwist rng);

void get_connected_component(HyperGraph *h, std::vector<int> *visited, std::vector<int> *connected_comp, int v);

bool is_hypergraph_connected(HyperGraph *h);

void free_hypergraph(HyperGraph* s);

void print_find_communities_struct(find_communities_struct* res);

void free_hypergraph(HyperGraph* s);