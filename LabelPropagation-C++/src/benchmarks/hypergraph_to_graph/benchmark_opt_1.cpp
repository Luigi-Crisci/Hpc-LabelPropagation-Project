#include "label_propagation.h"
#include <iostream>
#include <fstream>
#include <omp.h>
#include <chrono>
#include <queue>

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

bool **hypergraph_to_graph(HyperGraph *h)
{
    size_t ne = h->nEdge;

    bool** matrix =(bool**) calloc(ne,sizeof(bool*));
    for (size_t i = 0; i < ne; i++)
        matrix[i] = (bool*) calloc(ne,sizeof(bool));
    
    std::bitset<MAX_SIZE> *b1, *b2;
    for (int i = 0; i < ne; i++)
    {
        b1 = &(h->he2v[i]);
        for (int j = 0; j < ne; j++)
        {
            b2 = &(h->he2v[j]);
            if ((*b1 & *b2).any())
                matrix[i][j] = true;
        }
    }

    return matrix;
}


int bfs(HyperGraph *h, int e)
{
    bool** graph = hypergraph_to_graph(h);
    size_t graph_size = h->nEdge;

    std::queue<int> frontier;
    std::vector<bool> checked(graph_size,false);
    frontier.push(e);
    
    int current = -1,count = 0;
    while(!frontier.empty()){
        current = frontier.front();
        frontier.pop();

        for (int i = 0; i < graph_size; i++)
        {  
            bool connected = graph[current][i];
            bool c = !checked[i];
            if(graph[current][i] && !checked[i]){
                checked[i] = true;
                frontier.push(i);
                count++;
            }
        }
    }
    return count;
}

bool is_hypergraph_connected(HyperGraph *h)
{
    return bfs(h,0) == h->nEdge;
}

int main(int argc, char *argv[])
{   
    
    HyperGraph *small_hypergraph = new HyperGraph(5000, 300);
    populate_from_file(small_hypergraph, "../../../resources/h_test_hypergraph_5000_300.txt");
    
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    bool x = is_hypergraph_connected(small_hypergraph);

    std::cout<<x<<std::endl;
    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout<<"Time to convert and bfs: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0<<std::endl;

    return 0;
}