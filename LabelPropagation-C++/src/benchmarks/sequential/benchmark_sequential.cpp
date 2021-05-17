#include "headers/label_propagation.h"
#include <iostream>
#include <fstream>

#define MAXITER 100
#define SEED 1234

void populate_from_file(HyperGraph *hyper_graph, std::string file_name){
    std::ifstream file;
    std::string str;
    file.open(file_name);

    if(!file.is_open()){
        perror("Error Open");
        exit(EXIT_FAILURE);
    }


    while(getline(file, str)){ }

    file.close();
    for (int i = 0; i < hyper_graph->nVertex; i++)
        {
            for (int j = 0; j < hyper_graph->nEdge; j++)
            {
                if (str[0] == '1')
                {
                    hyper_graph->v2he[i]->emplace(j, true);
                    hyper_graph->he2v[j]->emplace(i, true);
                    str.erase(0, 1);

                }else
                    str.erase(0, 1);
            }
        }

}

int main(int argc, char *argv[]){

    MT::MersenneTwist rng;
    rng.init_genrand(SEED);

    CFLabelPropagationFinder parameters;
    parameters.seed = SEED;
    parameters.max_iter = MAXITER;

    // SMALL SIZE BENCHMARK
    if(*argv[1] == 's'){
        std::cout<<"Starting benchmark_sequential on small size Hyper Graph"<<std::endl;
        HyperGraph *small_hypergraph = new HyperGraph(1000, 60);
        populate_from_file(small_hypergraph, "../resources/h_test_hypergraph_1000_60.txt");
        
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        find_communities_struct *return_find_comm = find_communities(small_hypergraph, parameters);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        
        std::cout<<"Find Communities time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0<<std::endl;
    }

    
    // MEDIUM SIZE BENCHMARK
    if(*argv[1] == 'm'){
        std::cout<<"Starting benchmark_sequential on medium size Hyper Graph"<<std::endl;
        HyperGraph *medium_hypergraph = new HyperGraph(5000, 300);
        populate_from_file(medium_hypergraph, "../resources/h_test_hypergraph_5000_300.txt");

        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        find_communities_struct *return_find_comm = find_communities(medium_hypergraph, parameters);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        
        std::cout<<"Find Communities time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0<<std::endl;
    }

    // LARGE SIZE BENCHMARK
    if(*argv[1] == 'l'){
        std::cout<<"Starting benchmark_sequential on large size Hyper Graph"<<std::endl;
        HyperGraph *large_hypergraph = new HyperGraph(50000, 3000);
        populate_from_file(large_hypergraph, "../resources/h_test_hypergraph_50000_3000.txt");

        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        find_communities_struct *return_find_comm = find_communities(large_hypergraph, parameters);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        
        std::cout<<"Find Communities time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0<<std::endl;
    }

    return 0;
}