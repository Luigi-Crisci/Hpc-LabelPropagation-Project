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

    while (getline(file, str)){ }

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

    if (argc < 3)
    {
        std::cout << "Missing some arguments!!" << std::endl;
        std::cout << "Usage: \n\t"<<argv[0]<<" [vertex] [edge] [density]" << std::endl;
        std::cout << std::endl;
        exit(0);
    }

    MT::MersenneTwist rng;
    rng.init_genrand(SEED);

    CFLabelPropagationFinder parameters;
    parameters.seed = SEED;
    parameters.max_iter = MAXITER;

    int nEdge, nVertex;
    float p;
    std::string file_name;

    nVertex = std::stoi(argv[1]);
    nEdge = std::stoi(argv[2]);
    p = std::stof(argv[3]);

    file_name = "../resources/h_test_hypergraph_"+std::to_string(nVertex)+"_"+std::to_string(nEdge)+"_"+std::to_string(p)+".txt";
    std::cout<<file_name<<std::endl;

    std::cout << "Starting [" << argv[0] << "] with vertex [" << argv[1] << "] "<< ", with edges ["<< argv[2] << "] and density [" << argv[3] << "]" << std::endl;


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