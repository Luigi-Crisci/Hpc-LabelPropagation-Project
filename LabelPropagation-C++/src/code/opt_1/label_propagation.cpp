#include "headers/label_propagation.h"
#include "headers/mtrnd.h"

#ifdef DEBUG
    #include<chrono>
#endif

void shuffle(int *array, int size, MT::MersenneTwist rng)
{
   
    if (size > 1)
    {
        size_t i;
        for (i = 0; i < size - 1; i++)
        {
            size_t j = i + GENRANDOM(rng) / (RAND_MAX / (size - i) + 1);

            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

/**
 * @brief Get the sets from map object
 * Extract how many Key are related to the same value, collapsing them into a set
 * @return A map of type Value => [Key1,Key2,...]
 */
std::unordered_map<int, std::unordered_set<int> *> *reverse_map(std::unordered_map<int, int> *map)
{

    std::unordered_map<int, std::unordered_set<int> *> *values_label_set = new std::unordered_map<int, std::unordered_set<int> *>;
    for (auto it = map->begin(); it != map->end(); it++)
    {

        int value = it->second;
        int key = it->first;
        if (values_label_set->count(value) == 1)
            values_label_set->at(value)->insert(key);
        else
        {
            std::unordered_set<int> *new_label_set = new std::unordered_set<int>; // this mem is not freed
            new_label_set->insert(key);
            values_label_set->insert({value, new_label_set});
        }
    }

    return values_label_set;
}

int get_vertices_number(HyperGraph *h, int edge)
{
    int count = 0;
    for (int i = 0; i < h->nVertex; i++)
        if (IS_CONNECTED_TO_VERTEX(h, edge, i))
            count++;
    return count;
}

int compute_vertex_label(HyperGraph *h, int v, std::unordered_map<int, int> *vlabel, std::unordered_map<int, int> *heLables, MT::MersenneTwist rng)
{
    std::map<int, bool> *edges_map = GET_EDGES(h, v);
    int edges_size = edges_map->size();

    if (edges_size == 0)
        return -1;

    int *edges = collapse_map(edges_map);
    std::unordered_map<int, int> *vertex_label_list = new std::unordered_map<int, int>;

    int max = 0;
    int current_label, current_edge;

    std::unordered_set<int> *max_vertex_label_found = new std::unordered_set<int>;

    shuffle(edges, edges_size, rng);
    for (int i = 0; i < edges_size; i++)
    {
        current_edge = edges[i];
        current_label = heLables->at(current_edge);

        if (vertex_label_list->count(current_label) == 1)
            (*vertex_label_list)[current_label] = vertex_label_list->at(current_label) + 1;
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

    delete(vertex_label_list);
    if (vlabel->count(v) && max_vertex_label_found->find(vlabel->at(v)) != max_vertex_label_found->end())
        return vlabel->at(v);
    return *(max_vertex_label_found->begin());
}

int compute_edge_label(HyperGraph *h, int e, std::unordered_map<int, int> *vlabel, std::unordered_map<int, int> *heLables, MT::MersenneTwist rng)
{    
    std::map<int, bool> *vertices_map = GET_VERTICES(h, e);
    int vertices_size = vertices_map->size();

    if (vertices_size == 0)
        return -1;

    int *vertices = collapse_map(vertices_map);

    int max = 0, current_label, current_vertex;
    std::unordered_map<int, int> *edge_label_list = new std::unordered_map<int, int>;
    std::unordered_set<int> *max_edge_label_found = new std::unordered_set<int>; //this is not freed

    shuffle(vertices, vertices_size, rng);
    for (int i = 0; i < vertices_size; i++)
    {
        current_vertex = vertices[i];
        current_label = vlabel->at(current_vertex);

        if (edge_label_list->count(current_label) == 1)
            (*edge_label_list)[current_label] = edge_label_list->at(current_label) + 1;
        else
            edge_label_list->insert({current_label, 1});

        if (edge_label_list->at(current_label) == max)
        {
            max_edge_label_found->insert(current_label);
        }
        else if (edge_label_list->at(current_label) > max)
        {

            max = edge_label_list->at(current_label);
            max_edge_label_found->erase(max_edge_label_found->begin(), max_edge_label_found->end());
            max_edge_label_found->insert(current_label);
        }
    }

    delete(edge_label_list);

    if (heLables->count(e) && max_edge_label_found->find(heLables->at(e)) != max_edge_label_found->end())
        return heLables->at(e);
    return *(max_edge_label_found->begin());
}

void get_connected_component(HyperGraph *h, std::vector<int> *visited, std::vector<int> *connected_comp, int v)
{
    (*visited)[v] = 1;
    connected_comp->push_back(v);

    for (int e = 0; e < h->nEdge; e++)
    {
        if (IS_CONNECTED_TO_EDGE(h, v, e))
        {
            for (int j = 0; j < h->nVertex; j++)
            {
                if ((*visited)[j] == 1)
                    continue;
                if (IS_CONNECTED_TO_VERTEX(h, e, j))
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

find_communities_struct *find_communities(HyperGraph *h, CFLabelPropagationFinder parameters)
{
    #ifdef DEBUG
        std::chrono::steady_clock::time_point start;
        start = std::chrono::steady_clock::now();
    #endif // DEBUG

    //TODO: Parallelize BFS
    assert(is_hypergraph_connected(h));
    
    #ifdef DEBUG
        std::cout<<"Time Is_Hypergraph connected: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0<<std::endl;
        start = std::chrono::steady_clock::now();
    #endif // DEBUG


    // TODO: Find a way to have a multicore random > This is multithread
    MT::MersenneTwist rng;
    rng.init_genrand(parameters.seed);

    std::unordered_map<int, int> *vLabel = new std::unordered_map<int, int>;
    std::unordered_map<int, int> *heLabels = new std::unordered_map<int, int>;

    int *vertices = (int *)calloc(h->nVertex, sizeof(int));
    int *edges = (int *)calloc(h->nEdge, sizeof(int));

    for (int i = 0; i < h->nVertex; i++)
    {
        vertices[i] = i;
        vLabel->insert({i, i});
    }

    for (int i = 0; i < h->nEdge; i++)
        edges[i] = i;

    bool stop = false;
    int current_iter;

    #ifdef DEBUG
        std::cout<<"Time Parameter initialization: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0<<std::endl;
        start = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point start_inner;
    #endif // DEBUG
    
    for (current_iter = 1; !stop && current_iter < parameters.max_iter; current_iter++)
    {
        stop = true;
        #ifdef DEBUG
            start_inner = std::chrono::steady_clock::now();
        #endif // DEBUG

        shuffle(edges, h->nEdge, rng);
        for (int i = 0; i < h->nEdge; i++)
        {
            int current_edge = edges[i];
            if (get_vertices_number(h, current_edge) == 0)
                continue;
            (*heLabels)[current_edge] = compute_edge_label(h, current_edge, vLabel, heLabels, rng);
        }

        #ifdef DEBUG
            std::cout<<current_iter<<" - Edge Label: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_inner).count() / 1000.0<<std::endl;
            start_inner = std::chrono::steady_clock::now();
        #endif // DEBUG
        
        shuffle(vertices, h->nVertex, rng);
        for (int i = 0; i < h->nVertex; i++)
        {
            int current_vertex = vertices[i];
            int new_label = compute_vertex_label(h, current_vertex, vLabel, heLabels, rng);
            if (new_label != vLabel->at(current_vertex))
                stop = false;

            (*vLabel)[current_vertex] = new_label;
        }
        #ifdef DEBUG
           std::cout<<current_iter<<" - Vertex Label: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_inner).count() / 1000.0<<std::endl;
        #endif // DEBUG

    }
    
    #ifdef DEBUG
        std::cout<<"Time Community for: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0<<std::endl;
        start = std::chrono::steady_clock::now();
    #endif // DEBUG

    //Get sets of Vertices lables
    std::unordered_map<int, std::unordered_set<int> *> *vertices_label_set = reverse_map(vLabel);
    //Get sets of Edges lables
    std::unordered_map<int, std::unordered_set<int> *> *edges_label_set = reverse_map(heLabels);

    //Collapse all vertives sets into a global set
    //TODO: Check if the constructor with size is in conflict with the insert
    std::unordered_set<std::unordered_set<int> *> *vertices_sets = new std::unordered_set<std::unordered_set<int> *>();
    for (auto it = vertices_label_set->begin(); it != vertices_label_set->end(); it++)
        vertices_sets->insert(it->second);

    //Collapse all edges sets into a global set
    //TODO: Check if the constructor with size is in conflict with the insert
    std::unordered_set<std::unordered_set<int> *> *edges_set = new std::unordered_set<std::unordered_set<int> *>;
    for (auto it = edges_label_set->begin(); it != edges_label_set->end(); it++)
        edges_set->insert(it->second);

    int* vertices_labels = (int*) calloc(h->nVertex,sizeof(int));
    int* edges_labels = (int*) calloc(h->nEdge,sizeof(int));

    //Collapse vertex labels into array
    for (int i = 0; i < h->nVertex; i++)
        vertices_labels[i]= vLabel->at(i);

    //Collapse vertex labels into array
    for (int i = 0; i < h->nEdge; i++)
        if (IS_EDGE_EMPTY(h, i))
            edges_labels[i]=-1;
        else
            edges_labels[i]= heLabels->at(i);

    delete(vLabel);
    delete(heLabels);
    free(vertices);
    free(edges);
    delete(vertices_label_set);
    delete(edges_label_set);

    #ifdef DEBUG
        std::cout<<"Parameter finalization for: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0<<std::endl;
    #endif // DEBUG
    return new find_communities_struct(vertices_sets, edges_set, vertices_labels,h->nVertex, edges_labels, h->nEdge, current_iter);
}

