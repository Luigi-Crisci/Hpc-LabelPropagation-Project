#include <iostream>
#include <ctime>
#include <map>
#include <set>
#include <iterator>
#include "mtrnd.h"

// Defines values used by the algorithm according to the paper
#define MAXITER 100
#define SEED time(NULL) // ¯\_(ツ)_/¯ 
#define NMAX 1000 // max number, should be somewhat related to the size of the hgraph

//Macros
#define GETHYPEREDGES(h, v)  std::cout<<"h v="<<v<<std::endl
#define GENRANDOM(rng) static_cast<int>(rng.genrand_real1()*NMAX)


/*
    Hyper-vertex structure
*/
struct hVertex
{
    hVertex(int n)
    {
        
    }

    ~hVertex();
};

/*
    Hyper-edge structure
*/
struct hEdge
{
    
};

/*
    Hyper-graph structure
*/
struct HyperGraph
{
    //hVertex hVertex; 
    hEdge hEdges;

};


int main(int, char**) {
    
//initialize the Mersenne Twister.
    MT::MersenneTwist rng;
    rng.init_genrand(SEED); 
    
    //create hypergraph
    HyperGraph h;
    

    for(int i=0; i<10; i++){
        std::cout << GENRANDOM(rng) << std::endl;
    }


    return 0;
}
