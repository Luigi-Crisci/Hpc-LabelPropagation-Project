#include "headers/label_propagation.h"

void print_find_communities_struct(find_communities_struct *res)
{
    using namespace std;
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

void free_hypergraph(HyperGraph *s)
{
    for (int i = 0; i < s->nVertex; i++)
        delete (s->v2he[i]);
    for (int i = 0; i < s->nEdge; i++)
        delete (s->he2v[i]);

    delete (s->v2he);
    delete (s->he2v);
    delete (s);
}

int *collapse_map(std::map<int, bool> *map)
{
    int *vector = (int *)calloc(map->size(), sizeof(int));
    int i = 0;
    for (auto k : *map)
        vector[i] = k.first;
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