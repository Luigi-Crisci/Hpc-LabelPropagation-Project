#include <iostream>
#include <ctime>
#include <map>
#include <set>
#include <iterator>
#include "mtrnd.h"
#include <vector>

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
        hVertex = new std::vector<std::vector<int> *>(nVertex, new std::vector<int>(nEdge));
    }

    ~HyperGraph()
    {
        for (auto vet : *hVertex)
            delete vet;

        delete hVertex;
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
            (*h.hVertex)[i]->insert((*h.hVertex)[i]->begin() + j, 1);
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
    std::cout<<"Processing node "<<v<<std::endl;
    (*visited)[v] = 1;
    Int_Matrix hVertex = h.hVertex;
    for(int e = 0; e < h.nEdge; e++){
        if(hVertex->at(v)->at(e) == 1){

            for(int j=0; j < h.nVertex; j++){
                if((*visited)[j] == 1)
                    continue;
                if(hVertex->at(j)->at(e) == 1)
                    (*visited)[j] = 1;
                    connected_comp->push_back(j);
                    get_connected_component(h,visited,connected_comp,j);
            }
        }
        std::cout<<"Exiting edge "<<e<<" for node "<<v<<std::endl;
    }
    std::cout<<"Exiting node "<<v<<std::endl;
}

bool is_hypergraph_connected(HyperGraph h)
{
    std::vector<int> visited(h.nVertex);
    std::vector<int> connected_comp;

    get_connected_component(h, &visited, &connected_comp, 0);
    
    std::cout<<"Size: "<<connected_comp.size()<<std::endl;

    return connected_comp.size() == h.nVertex ? true : false;
}

/*
        function get_connected_components(h::Hypergraph)
            visited = falses(nhv(h))
            cc = Vector{Int}[]
                for i in 1:nhv(h)
                    if !visited[i]
                        s = Int[]
                        _walk!(h, s, i, visited)
                        push!(cc, s)
                end
            end
            cc
        end



        function _walk!(h::Hypergraph, s::AbstractVector{Int}, i::Int, visited::AbstractVector{Bool})
            visited[i] && return
            visited[i] = true
            push!(s, i)
            for he in keys(gethyperedges(h, i))
                for j in keys(getvertices(h, he))
                    _walk!(h, s, j, visited)
                end
            end
        end
    */