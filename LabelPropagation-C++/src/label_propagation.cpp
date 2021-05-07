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

void shuffle(int *array, size_t n, MT::MersenneTwist rng);

typedef std::vector<std::vector<int> *> * Int_Matrix;

/*
    Hyper-graph structure
*/
typedef struct HyperGraph
{
    int nVertex,nEdge;
    Int_Matrix hVertex;

    HyperGraph(int nVertex, int nEdge)
    {
        this->nVertex = nVertex;
        this->nEdge = nEdge;
        hVertex = new std::vector<std::vector<int> *>(nVertex);
        for(int i = 0; i < nVertex; i++){
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
    HyperGraph h(10,2);

    for(int i = 0; i < h.nVertex; i++){
        for(int j = 0; j < h.nEdge; j++){
            if(rng.genrand_real1() <= 0.8 ){
                (*h.hVertex)[i]->insert((*h.hVertex)[i]->begin() + j, 1);
            }
        }
    }

    for(int i = 0; i < h.nVertex; i++){
        for(int j = 0; j < h.nEdge; j++){
            std::cout<<h.hVertex->at(i)->at(j)<<" ";
        }
        std::cout<<std::endl;
    }

    if(is_hypergraph_connected(h))
        std::cout<<"Azz"<<std::endl;

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

void findCommunities(HyperGraph h)
{
    assert(is_hypergraph_connected(h));

    
}

int computeVertexLabel(HyperGraph h, int v, std::map<int, int> vlabel, std::map<int, int> helabels, MT::MersenneTwist rng)
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

int computeEdgeLabel(HyperGraph h, int v, std::map<int, int> vlabel, std::map<int, int> helabels, MT::MersenneTwist rng)
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

void shuffle(int *array, size_t n, MT::MersenneTwist rng)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + GENRANDOM(rng) / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void get_connected_component(HyperGraph h, std::vector<int> * visited, std::vector<int>* connected_comp, int v)
{
    (*visited)[v] = 1;
    connected_comp->push_back(v);

    Int_Matrix hVertex = h.hVertex;
    for(int e = 0; e < h.nEdge; e++){
        if(hVertex->at(v)->at(e) == 1){

            for(int j=0; j < h.nVertex; j++){
                if((*visited)[j] == 1)
                    continue;
                if(hVertex->at(j)->at(e) == 1)
                    get_connected_component(h,visited,connected_comp,j);
            }
        }
    }
}

bool is_hypergraph_connected(HyperGraph h)
{
    std::vector<int> visited(h.nVertex);
    std::vector<int> connected_comp;

    get_connected_component(h, &visited, &connected_comp, 0);
    
    return connected_comp.size() == h.nVertex ? true : false;
}