#include "headers/label_propagation.h"
#include <iostream>
#include <fstream>

#define MAXITER 100
#define SEED 1234

void populate_from_file(HyperGraph *hyper_graph, std::string file_name)
{
    std::ifstream file;
    std::string str;
    file.open(file_name);

    if (!file.is_open())
    {
        perror("Error Open");
        exit(EXIT_FAILURE);
    }

    while (getline(file, str)){}

    file.close();

    int counter = 0;
    for (int i = 0; i < hyper_graph->nVertex; i++)
    {
        for (int j = 0; j < hyper_graph->nEdge; j++)
        {
            if (str[counter++] == '1')
            {
                hyper_graph->v2he[i].set(j, true);
                hyper_graph->he2v[j].set(i, true);
            }
        }
    }
}

int main(int argc, char *argv[]){

    if (argc < 2)
    {
        std::cout << "Missing some arguments!!" << std::endl;
        std::cout << "Usage: \n\t" << argv[0] << " [size] [density]" << std::endl;
        std::cout << std::endl;
        std::cout << "INFO:\tsize: s (small) - m (medium) - l (large)" << std::endl;
        std::cout << "\tdensity: s (sparse) - n (normal) - d (dense)" << std::endl;
        exit(0);
    }

    MT::MersenneTwist rng;
    rng.init_genrand(SEED);

    CFLabelPropagationFinder parameters;
    parameters.seed = SEED;
    parameters.max_iter = MAXITER;

    int nEdge, nVertex;
    std::string file_name;

    if(*argv[1] == 's'){
        nVertex = 5000;
        nEdge = 300;
        file_name = "../resources/h_test_hypergraph_5000_300";
    }else if(*argv[1] == 'm'){
        nVertex = 10000;
        nEdge = 600;
        file_name = "../resources/h_test_hypergraph_10000_600";
    }
    else{
        nVertex = 20000;
        nEdge = 1000;
        file_name = "../resources/h_test_hypergraph_20000_1000";
    }

    if(*argv[2] == 's'){
        file_name += "_0.300000.txt";
    }else if(*argv[2] == 'n'){
        file_name += "_0.500000.txt";
    }
    else{
        file_name += "_0.700000.txt";
    }

    std::cout << "Starting [" << argv[0] << "] with size [" << *argv[1] << "] and density [" << *argv[2] << "]" << std::endl;

    HyperGraph *hypergraph = new HyperGraph(nVertex, nEdge);
    populate_from_file(hypergraph, file_name);

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    find_communities_struct *return_find_comm = find_communities(hypergraph, parameters);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    
    std::cout<<"Find Communities time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0<<std::endl;

    #ifdef DEBUG
        print_find_communities_struct(return_find_comm);
    #endif // DEBUG

    return 0;
}