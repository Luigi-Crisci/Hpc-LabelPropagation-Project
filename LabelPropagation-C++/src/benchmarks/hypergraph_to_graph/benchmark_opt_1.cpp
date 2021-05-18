#include "label_propagation.h"
#include <iostream>
#include <fstream>
#include <omp.h>
#include <chrono>

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

    while (getline(file, str))
    {
    }

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

int main(int argc, char *argv[])
{

    HyperGraph *small_hypergraph = new HyperGraph(1000, 60);
    populate_from_file(small_hypergraph, "../../../resources/h_test_hypergraph_5000_300.txt");
    
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    size_t nv = small_hypergraph->nVertex;
    size_t ne = small_hypergraph->nEdge;
    std::vector<std::vector<bool>> matrix(ne,std::vector<bool>(ne));
    
    std::bitset<MAX_SIZE>* b1,*b2;

    for (int i = 0; i < ne; i++)
    {
        b1 = &(small_hypergraph->he2v[i]);
        for (int j = 0; j < ne; j++)
        {
            b2 = &(small_hypergraph->he2v[j]);
            if((*b1&=*b2).any())
                matrix[i][j] = true;
        }
        
    }

    


    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout<<"Time to convert: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0<<std::endl;

    return 0;
}