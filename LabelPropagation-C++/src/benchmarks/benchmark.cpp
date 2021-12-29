#include <headers/label_propagation.h>
#include <iostream>
#include <fstream>
#include "utils.hpp"

#define MAXITER 100
#define SEED 1234

int main(int argc, char *argv[]){

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    MT::MersenneTwist rng;
    rng.init_genrand(SEED);

    CFLabelPropagationFinder parameters;
    parameters.seed = SEED;
    parameters.max_iter = MAXITER;

    HyperGraph *hypergraph = read_hypergraph(argv[1]);
    std::cout << "Starting [" << argv[0] << "] with vertex [" << hypergraph->nVertex << "] "<< "and with edges ["<< hypergraph->nEdge << "]" << std::endl;

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    find_communities_struct *return_find_comm = find_communities(hypergraph, parameters);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    
    std::cout<<"Find Communities time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0<<std::endl;

    #ifdef DEBUG
        print_find_communities_struct(return_find_comm);
    #endif // DEBUG
    return 0;
}