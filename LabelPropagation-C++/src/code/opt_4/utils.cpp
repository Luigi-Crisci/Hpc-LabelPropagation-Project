#include "headers/label_propagation.h"

void print_find_communities_struct(find_communities_struct *res)
{
    using namespace std;

    std::cout<<"Num iterations: "<<res->iter<<std::endl;

    std::cout << "Vertex labels: [";
    for (int i = 0; i < res->vLabel_size; i++)
    {
        std::cout << res->vLabel[i] << ", ";
    }
    std::cout << "]" << std::endl;

    std::cout << "Edge labels: [";
    for (int i = 0; i < res->heLabel_size; i++)
    {
        std::cout << res->heLabel[i] << ", ";
    }
    std::cout << "]" << std::endl;

    std::cout << "Vertex communities: " << std::endl;
    for (auto set : *(res->np))
    {
        std::cout << "- [";
        for (auto value : *set)
            std::cout << value << ", ";
        std::cout << "]" << std::endl;
    }

    std::cout << "Edge communities: " << std::endl;
    for (auto set : *(res->hep))
    {
        std::cout << "- [";
        for (auto value : *set)
            std::cout << value << ", ";
        std::cout << "]" << std::endl;
    }
}


int *get_vertices_indices(std::bitset<MAX_SIZE> *b,int limit)
{
    int size = b->count();
    int *vector = (int *)calloc(size, sizeof(int));
    for (int i = 0,j=0; i < limit; i++)
        if(b->test(i))
            vector[j++] = i;
    return vector;
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

inline int next_multiple(int n, int base){
    return n + (base - n % base);
}