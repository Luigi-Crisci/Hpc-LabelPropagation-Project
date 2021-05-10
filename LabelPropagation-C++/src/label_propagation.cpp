#include <iostream>
#include <ctime>
#include <map>
#include <set>
#include <iterator>
#include "mtrnd.h"
#include <vector>
#include <assert.h>

#include "label_propagation.h"

// Defines values used by the algorithm according to the paper
#define MAXITER 100
#define SEED time(NULL) // ¯\_(ツ)_/¯

//Macros
#define GETHYPEREDGES(h, v) std::cout << "h v=" << v << std::endl
#define GENRANDOM(rng) static_cast<unsigned long>(rng.genrand_real1() * RAND_MAX)
#define IS_EDGE_EMPTY(h, e) get_vertices_number(h, e) == 0 ? true : false


void shuffle(std::vector<int> *array, MT::MersenneTwist rng);
bool is_hypergraph_connected(HyperGraph *h);
void get_connected_component(HyperGraph *h, std::vector<int> *visited, std::vector<int> *connected_comp, int v);
int compute_edge_label(HyperGraph *h, int e, std::map<int, int> *vlabel, std::map<int, int> *heLables, MT::MersenneTwist rng);
int compute_vertex_label(HyperGraph *h, int v, std::map<int, int> *vlabel, std::map<int, int> *heLables, MT::MersenneTwist rng);
std::map<int, std::set<int> *> *reverse_map(std::map<int, int> *map);

// int main(int, char **)
// {

//     //initialize the Mersenne Twister.
//     MT::MersenneTwist rng;
//     rng.init_genrand(SEED);

//     //create hypergraph
//     HyperGraph h(10, 2);

//     for (int i = 0; i < h.nVertex; i++)
//     {
//         for (int j = 0; j < h.nEdge; j++)
//         {
//             if (rng.genrand_real1() <= 0.8)
//             {
//                 (*h.hVertex)[i]->insert((*h.hVertex)[i]->begin() + j, 1);
//             }
//         }
//     }

//     for (int i = 0; i < h.nVertex; i++)
//     {
//         for (int j = 0; j < h.nEdge; j++)
//         {
//             std::cout << h.hVertex->at(i)->at(j) << " ";
//         }
//         std::cout << std::endl;
//     }

//     if (is_hypergraph_connected(&h))
//         std::cout << "Hypergraph connected!" << std::endl;

//     return 0;
// }

std::vector<int> *get_edges(HyperGraph *h, int vertices)
{
    std::vector<int> *edges = new std::vector<int>;
    for (int i = 0; i < h->nEdge; i++)
        if (h->hVertex->at(vertices)->at(i) == 1)
            edges->push_back(i);
    return edges;
}

std::vector<int> *get_vertices(HyperGraph *h, int edge)
{
    std::vector<int> *vertices = new std::vector<int>;
    for (int i = 0; i < h->nVertex; i++)
        if (h->hVertex->at(i)->at(edge) == 1)
            vertices->push_back(i);
    return vertices;
}

int get_vertices_number(HyperGraph *h, int edge)
{
    int count = 0;
    for (int i = 0; i < h->nVertex; i++)
        if (h->hVertex->at(i)->at(edge) == 1)
            count++;
    return count;
}

bool is_edge_empty(HyperGraph *h, int edge)
{
    for (int i = 0; i < h->nVertex; i++)
        if (h->hVertex->at(i)->at(edge) == 1)
            return false;
    return true;
}

find_communities_struct findCommunities(HyperGraph *h, CFLabelPropagationFinder parameters)
{
    assert(is_hypergraph_connected(h));

    MT::MersenneTwist rng;
    rng.init_genrand(parameters.seed);

    std::map<int, int> *vLabel = new std::map<int, int>;
    std::map<int, int> *heLabels = new std::map<int, int>;

    std::vector<int> *vertices = new std::vector<int>;
    std::vector<int> *edges = new std::vector<int>;

    for (int i = 0; i < h->nVertex; i++)
    {
        vertices->push_back(i);
        vLabel->insert({i, i});
    }

    for (int i = 0; i < h->nEdge; i++)
        edges->push_back(i);

    bool stop = false;
    int current_iter = 0;

    for (int current_iter = 0; !stop && current_iter < parameters.max_iter; current_iter++)
    {
        stop = true;
        shuffle(edges, rng);

        for (int i = 0; i < h->nEdge; i++)
        {
            int current_edge = edges->at(i);
            if (IS_EDGE_EMPTY(h, current_edge))
                continue;
            heLabels->insert({current_edge, compute_edge_label(h, current_edge, vLabel, heLabels, rng)});
        }

        shuffle(vertices, rng);
        for (int i = 0; i < h->nVertex; i++)
        {
            int current_vertex = vertices->at(i);
            int new_label = compute_vertex_label(h, current_vertex, vLabel, heLabels, rng);
            if (new_label != vLabel->at(current_vertex))
                stop = false;
            vLabel->insert({current_vertex, new_label});
        }
    }

    //Get sets of Vertices lables
    std::map<int, std::set<int> *> *vertices_label_set = reverse_map(vLabel);
    //Get sets of Edges lables
    std::map<int, std::set<int> *> *edges_label_set = reverse_map(heLabels);

    //Collapse all vertives sets into a global set
    std::set<std::set<int> *> *vertices_sets = new std::set<std::set<int> *>;
    for (auto it = vertices_label_set->begin(); it != vertices_label_set->end(); it++)
        vertices_sets->insert(it->second);

    //Collapse all edges sets into a global set
    std::set<std::set<int> *> *edges_set = new std::set<std::set<int> *>;
    for (auto it = edges_label_set->begin(); it != edges_label_set->end(); it++)
        edges_set->insert(it->second);

    std::vector<int> *vertices_labels = new std::vector<int>;
    std::vector<int> *edges_labels = new std::vector<int>;

    //Collapse vertex labels into array
    for (int i = 0; i < h->nVertex; i++)
        vertices_labels->push_back((*vLabel)[i]);

    //Collapse vertex labels into array
    for (int i = 0; i < h->nEdge; i++)
        if (is_edge_empty(h, i))
            edges_labels->push_back(-1);
        else
            edges_labels->push_back((*heLabels)[i]);

    //TODO: A LOTS OF FREE

    return find_communities_struct(vertices_sets, edges_set, vertices_labels, edges_labels, current_iter);
}

/**
 * @brief Get the sets from map object
 * Extract how many Key are related to the same value, collapsing them into a set
 * @return A map of type Value => [Key1,Key2,...]
 */
std::map<int, std::set<int> *> *reverse_map(std::map<int, int> *map)
{
    std::map<int, std::set<int> *> *values_label_set = new std::map<int, std::set<int> *>;
    for (auto it = map->begin(); it != map->end(); it++)
    {
        int value = it->second;
        int key = it->first;
        if (map->count(value) == 1)
            values_label_set->at(value)->insert(key);
        else
        {
            std::set<int> *new_label_set = new std::set<int>;
            new_label_set->insert(key);
            values_label_set->insert({value, new_label_set});
        }
    }
    return values_label_set;
}

int compute_vertex_label(HyperGraph *h, int v, std::map<int, int> *vlabel, std::map<int, int> *heLables, MT::MersenneTwist rng)
{
    std::vector<int> *edges = get_edges(h, v);
    std::map<int, int> *vertex_label_list = new std::map<int, int>;

    int max = 0;
    int current_label, current_edge;

    std::set<int> *max_vertex_label_found = new std::set<int>;

    shuffle(edges, rng);
    for (int i = 0, size = edges->size(); i < size; i++)
    {
        current_edge = edges->at(i);
        current_label = heLables->at(current_edge);

        if (vertex_label_list->count(current_label) == 1)
            vertex_label_list->insert({current_label, vertex_label_list->at(current_label) + 1});
        else
            vertex_label_list->insert({current_label, 1});

        if (vertex_label_list->at(current_label) == max)
            max_vertex_label_found->insert(current_label);

        if (vertex_label_list->at(current_label) > max)
        {
            max = vertex_label_list->at(current_label);
            max_vertex_label_found->erase(max_vertex_label_found->begin(), max_vertex_label_found->end());
            max_vertex_label_found->insert(current_label);
        }
    }

    if (vlabel->count(v) && max_vertex_label_found->find(vlabel->at(v)) != max_vertex_label_found->end())
        return vlabel->at(v);
    return *(max_vertex_label_found->begin());
}

int compute_edge_label(HyperGraph *h, int e, std::map<int, int> *vlabel, std::map<int, int> *heLables, MT::MersenneTwist rng)
{
    std::vector<int> *vertices = get_vertices(h, e);
    std::map<int, int> *edge_label_list = new std::map<int, int>;
    int max = 0, current_label, current_vertex;
    std::set<int> *max_edge_label_found = new std::set<int>;

    shuffle(vertices, rng);
    for (int i = 0, size = vertices->size(); i < size; i++)
    {
        current_vertex = vertices->at(i);
        current_label = vlabel->at(current_vertex);

        if (edge_label_list->count(current_label) == 1)
            edge_label_list->insert({current_label, edge_label_list->at(current_label) + 1});
        else
            edge_label_list->insert({current_label, 1});

        if (edge_label_list->at(current_label) == max)
            max_edge_label_found->insert(current_label);
        else if (edge_label_list->at(current_label) > max)
        {
            max = edge_label_list->at(current_label);
            max_edge_label_found->erase(max_edge_label_found->begin(), max_edge_label_found->end());
            max_edge_label_found->insert(current_label);
        }
    }

    if (heLables->count(e) && max_edge_label_found->find(heLables->at(e)) != max_edge_label_found->end())
        return heLables->at(e);
    return *(max_edge_label_found->begin());
}

void shuffle(std::vector<int> *array, MT::MersenneTwist rng)
{
    int n = array->size();
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + GENRANDOM(rng) / (RAND_MAX / (n - i) + 1);
            int t = array->at(j);
            array[j] = array[i];
            (*array)[i] = t;
        }
    }
}

void get_connected_component(HyperGraph *h, std::vector<int> *visited, std::vector<int> *connected_comp, int v)
{
    (*visited)[v] = 1;
    connected_comp->push_back(v);

    Int_Matrix hVertex = h->hVertex;
    for (int e = 0; e < h->nEdge; e++)
    {
        if (hVertex->at(v)->at(e) == 1)
        {

            for (int j = 0; j < h->nVertex; j++)
            {
                if ((*visited)[j] == 1)
                    continue;
                if (hVertex->at(j)->at(e) == 1)
                    get_connected_component(h, visited, connected_comp, j);
            }
        }
    }
}

bool is_hypergraph_connected(HyperGraph *h)
{
    std::vector<int> visited(h->nVertex);
    std::vector<int> connected_comp;

    get_connected_component(h, &visited, &connected_comp, 0);

    return connected_comp.size() == h->nVertex ? true : false;
}