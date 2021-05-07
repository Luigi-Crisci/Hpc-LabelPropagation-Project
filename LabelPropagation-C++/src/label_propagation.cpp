#include <iostream>
#include <ctime>
#include <map>
#include <set>
#include <iterator>
#include "mtrnd.h"
#include <vector>
#include <assert.h>

// Defines values used by the algorithm according to the paper
#define MAXITER 100
#define SEED time(NULL) // ¯\_(ツ)_/¯

//Macros
#define GETHYPEREDGES(h, v) std::cout << "h v=" << v << std::endl
#define GENRANDOM(rng) static_cast<unsigned long>(rng.genrand_real1() * RAND_MAX)
#define IS_EDGE_EMPTY(h, e) get_vertices_number(h, e) == 0 ? true : false

void shuffle(int *array, size_t n, MT::MersenneTwist rng);

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

bool is_hypergraph_connected(HyperGraph h);

int main(int, char **)
{

    //initialize the Mersenne Twister.
    MT::MersenneTwist rng;
    rng.init_genrand(SEED);

    //create hypergraph
    HyperGraph h(10, 2);

    for (int i = 0; i < h.nVertex; i++)
    {
        for (int j = 0; j < h.nEdge; j++)
        {
            if (rng.genrand_real1() <= 0.8)
            {
                (*h.hVertex)[i]->insert((*h.hVertex)[i]->begin() + j, 1);
            }
        }
    }

    for (int i = 0; i < h.nVertex; i++)
    {
        for (int j = 0; j < h.nEdge; j++)
        {
            std::cout << h.hVertex->at(i)->at(j) << " ";
        }
        std::cout << std::endl;
    }

    if (is_hypergraph_connected(h))
        std::cout << "Azz" << std::endl;

    return 0;

    int vec[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (int i = 0; i < 10; i++)
    {
        std::cout << "i=" << vec[i] << std::endl;
    }

    shuffle(vec, 10, rng);

    std::cout << std::endl
              << std::endl
              << std::endl
              << std::endl;

    for (int i = 0; i < 10; i++)
    {
        std::cout << "i=" << vec[i] << std::endl;
    }

    return 0;
}

std::vector<int> *get_vertices(HyperGraph *h, int edge)
{
    std::vector<int> *vertices = new std::vector<int>;
    for (int i = 0; i < h->nVertex; i++)
        if (h->hVertex->at(i)->at(edge) == 1)
            vertices->push_back(i);
    return vertices;
}

int get_vertices_number(HyperGraph *h, int edge)
{
    int count = 0;
    for (int i = 0; i < h->nVertex; i++)
        if (h->hVertex->at(i)->at(edge) == 1)
            count++;
    return count;
}

void findCommunities(HyperGraph *h, CFLabelPropagationFinder parameters)
{
    assert(is_hypergraph_connected(h));

    MT::MersenneTwist rng;
    rng.init_genrand(parameters.seed);

    std::map<int, int> *vLabel = new std::map<int, int>;
    std::map<int, int> *eLabel = new std::map<int, int>;

    std::vector<int> *vertices = new std::vector<int>;
    std::vector<int> *edges = new std::vector<int>;

    for (int i = 0; i < h->nVertex; i++)
    {
        vertices->push_back(i);
        vLabel->insert({i, i});
    }

    for (int i = 0; i < h->nEdge; i++)
        edges->push_back(i);

    bool stop = false;
    int current_iter = 0;
    int max_iter = parameters.max_iter;

    bool stop = false;
    for (int current_iter = 0; !stop && current_iter < parameters.max_iter; current_iter++)
    {
        stop = true;
        shuffle(edges, rng);

        for (int i = 0; i < h->nEdge; i++)
        {
            int current_edge = edges->at(i);
            if (IS_EDGE_EMPTY(h, current_edge))
                continue;
            eLabel->insert({current_edge, compute_edge_label(h, current_edge, vLabel, eLabel, rng)});
        }

        shuffle(vertices, rng);
        for (int i = 0; i < h->nVertex; i++)
        {
            int current_vertex = vertices->at(i);
            int new_label = compute_vertex_label(h, current_vertex, vLabel, eLabel, rng);
            if (new_label != vLabel->at(current_vertex))
                stop = false;
            vLabel->insert({current_vertex, new_label});
        }
    }

    
}

int compute_vertex_label(HyperGraph *h, int v, std::map<int, int> *vlabel, std::map<int, int> *helabels, MT::MersenneTwist rng)
{

    //hesᵥ = gethyperedges(h, v)

    std::map<int, int> vL;

    int max = 0;
    std::set<int> maxL;
    std::set<int>::iterator maxLiter;

    // for v in shuffle!(rng, collect(keys(vₑ)))

    maxLiter = maxL.begin();
    return *maxLiter;
}

int compute_edge_label(HyperGraph *h, int e, std::map<int, int> *vlabel, std::map<int, int> *helabels, MT::MersenneTwist rng)
{
    std::vector<int>* vertices = get_vertices(h,e);
    std::map<int,int>* edge_label_list = new std::map<int,int>;
    int max = 0,current_label,current_vertex;
    std::set<int>* max_edge_label_found = new std::set<int>;

    shuffle(vertices,rng);
    for (int i = 0, size = vertices->size(); i < size; i++)
    {
        current_vertex = vertices->at(i);
        current_label = vlabel->at(current_vertex);

        if(edge_label_list->count(current_label) == 1)
            edge_label_list->insert( { current_label , edge_label_list->at(current_label) + 1 } );
        else
            edge_label_list->insert({current_label, 1});

        if(edge_label_list->at(current_label) == max)
            max_edge_label_found->insert(current_label);
        else if(edge_label_list->at(current_label) > max){
            max = edge_label_list->at(current_label);
            max_edge_label_found->erase(max_edge_label_found->begin(),max_edge_label_found->end());
            max_edge_label_found->insert(current_label);
        }
    }
    
    if(helabels->count(e) && max_edge_label_found->find(helabels->at(e)) != max_edge_label_found->end() )
        return helabels->at(e);
    return *(max_edge_label_found->begin());

}

void shuffle(std::vector<int> *array, MT::MersenneTwist rng)
{
    int n = array->size();
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + GENRANDOM(rng) / (RAND_MAX / (n - i) + 1);
            int t = array->at(j);
            array[j] = array[i];
            (*array)[i] = t;
        }
    }
}

void get_connected_component(HyperGraph *h, std::vector<int> *visited, std::vector<int> *connected_comp, int v)
{
    (*visited)[v] = 1;
    connected_comp->push_back(v);

    Int_Matrix hVertex = h->hVertex;
    for (int e = 0; e < h->nEdge; e++)
    {
        if (hVertex->at(v)->at(e) == 1)
        {

            for (int j = 0; j < h->nVertex; j++)
            {
                if ((*visited)[j] == 1)
                    continue;
                if (hVertex->at(j)->at(e) == 1)
                    get_connected_component(h, visited, connected_comp, j);
            }
        }
    }
}

bool is_hypergraph_connected(HyperGraph *h)
{
    std::vector<int> visited(h->nVertex);
    std::vector<int> connected_comp;

    get_connected_component(h, &visited, &connected_comp, 0);

    return connected_comp.size() == h->nVertex ? true : false;
}