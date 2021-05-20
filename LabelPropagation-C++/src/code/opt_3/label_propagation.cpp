#include "utils.cpp"
#include <queue>
#include <omp.h>

void shuffle(int *element, int size, MT::MersenneTwist rng)
{
    if (size > 1)
    {
        size_t i;
        for (i = 0; i < size - 1; i++)
        {
            size_t j = i + GENRANDOM(rng) / (RAND_MAX / (size - i) + 1);

            int temp = element[j];
            element[j] = element[i];
            element[i] = temp;
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

int compute_vertex_label(HyperGraph *h, int v, std::unordered_map<int, int> *vlabel, std::unordered_map<int, int> *heLables, MT::MersenneTwist rng)
{
    //std::cout<<"start"<<std::endl;
    std::bitset<MAX_SIZE> *edges_bitset = GET_EDGES(h, v);

    int edges_size = edges_bitset->count();

    if (edges_size == 0)
        return -1;

    int *edges = get_vertices_indices(edges_bitset,h->nEdge);
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

    delete (vertex_label_list);
    //std::cout<<"end"<<std::endl;

    if (vlabel->count(v) && max_vertex_label_found->find(vlabel->at(v)) != max_vertex_label_found->end())
        return vlabel->at(v);
    return *(max_vertex_label_found->begin());
}

int compute_edge_label(HyperGraph *h, int e, std::unordered_map<int, int> *vlabel, std::unordered_map<int, int> *heLables, MT::MersenneTwist rng)
{
    std::bitset<MAX_SIZE> *vertices_bitset = GET_VERTICES(h, e);

    int vertices_size = vertices_bitset->count();

    if (vertices_size == 0)
        return -1;

    int *vertices = get_vertices_indices(vertices_bitset,h->nVertex);

    int max = 0, current_label, current_vertex, current_index;

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

    delete (edge_label_list);

    if (heLables->count(e) && max_edge_label_found->find(heLables->at(e)) != max_edge_label_found->end())
        return heLables->at(e);
    return *(max_edge_label_found->begin());
}

int bfs(HyperGraph *h, int e)
{
    bool **graph = hypergraph_to_graph(h);
    size_t graph_size = h->nEdge;

    std::queue<int> frontier;
    std::vector<bool> checked(graph_size, false);
    frontier.push(e);

    int current = -1, count = 0;
    while (!frontier.empty())
    {
        current = frontier.front();
        frontier.pop();

        for (int i = 0; i < graph_size; i++)
        {
            bool connected = graph[current][i];
            bool c = !checked[i];
            if (graph[current][i] && !checked[i])
            {
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
    return bfs(h, 0) == h->nEdge;
}

find_communities_struct *find_communities(HyperGraph *h, CFLabelPropagationFinder parameters)
{
#ifdef DEBUG
    std::chrono::steady_clock::time_point start;
    start = std::chrono::steady_clock::now();
#endif // DEBUG

    

#ifdef DEBUG
    std::cout << "Time Is_Hypergraph connected: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0 << std::endl;
    start = std::chrono::steady_clock::now();
#endif // DEBUG

    int num_edge = h->nEdge;
    int num_vertex = h->nVertex;
    int current_edge;
    int new_label, current_vertex;

    bool stop = false;
    int current_iter;

    MT::MersenneTwist rng;
    rng.init_genrand(parameters.seed);

    std::unordered_map<int, int> *vLabel = new std::unordered_map<int, int>;
    std::unordered_map<int, int> *heLabels = new std::unordered_map<int, int>;

    int *vertices = (int *)calloc(num_vertex, sizeof(int));
    int *edges = (int *)calloc(num_edge, sizeof(int));

    
    if(num_vertex > num_edge){
        #pragma omp parallel
        {
            #pragma omp single nowait
            {
                for (int i = 0; i < num_vertex; i++)
                {
                    vertices[i] = i;
                    vLabel->insert({i, i});
                    if(i<num_edge){
                        heLabels->insert({i, -1});
                        edges[i] = i;
                    } 
                }
            }
            #pragma omp single nowait
            {
                assert(is_hypergraph_connected(h));
            } 
            #pragma omp barrier
            #pragma omp single nowait
            {
                shuffle(edges, num_edge, rng);
            }
        }
    }else{
        #pragma omp parallel
        {
            #pragma omp single nowait
            {
                for (int i = 0; i < num_edge; i++)
                {
                    heLabels->insert({i, -1});
                    edges[i] = i;
                    if(i<num_vertex){
                        vertices[i] = i;
                        vLabel->insert({i, i}); 
                    } 
                }
            }
            #pragma omp single nowait
            {
                assert(is_hypergraph_connected(h));
            }
            #pragma omp barrier
            #pragma omp single nowait
            {
                shuffle(edges, num_edge, rng);
            }
        }
    }
    
    

#ifdef DEBUG
    std::cout << "Time Parameter initialization: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0 << std::endl;
    start = std::chrono::steady_clock::now();
#endif // DEBUG
    for (current_iter = 1; !stop && current_iter < parameters.max_iter; current_iter++)
    {
        stop = true;

        #pragma omp parallel
        {
            #pragma omp for private(current_edge) nowait
            for (int i = 0; i < num_edge; i++)
            {
                current_edge = edges[i];
                if (GET_EDGE_VERTICES_CONNECTED_NUMBER(h, current_edge) > 0)
                    (*heLabels)[current_edge] = compute_edge_label(h, current_edge, vLabel, heLabels, rng);
            }

            #pragma omp single 
            {
                shuffle(vertices, num_vertex, rng);
            }


            #pragma omp for private(new_label, current_vertex) nowait
            for (int i = 0; i < num_vertex; i++)
            {
                current_vertex = vertices[i];
                new_label = compute_vertex_label(h, current_vertex, vLabel, heLabels, rng);
                if (new_label != vLabel->at(current_vertex))
                {
                    stop &= 0;
                }
                (*vLabel)[current_vertex] = new_label;
            }

            #pragma omp single 
            {
                 shuffle(edges, num_edge, rng);
            }
        }

    }

#ifdef DEBUG
    std::cout << "Time Community for: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0 << std::endl;
    start = std::chrono::steady_clock::now();
#endif // DEBUG

    //TODO: reverse map parallel???
    //Get sets of Vertices lables
    std::unordered_map<int, std::unordered_set<int> *> *vertices_label_set = reverse_map(vLabel);
    //Get sets of Edges lables
    std::unordered_map<int, std::unordered_set<int> *> *edges_label_set = reverse_map(heLabels);

    //Collapse all vertives sets into a global set
    std::unordered_set<std::unordered_set<int> *> *vertices_sets = new std::unordered_set<std::unordered_set<int> *>(vertices_label_set->size());
    std::unordered_set<std::unordered_set<int> *> *edges_set = new std::unordered_set<std::unordered_set<int> *>(edges_label_set->size());

    int *vertices_labels = (int *)calloc(num_vertex, sizeof(int));
    int *edges_labels = (int *)calloc(num_edge, sizeof(int));

    //TODO questa regione ha inserimenti quindi la parallelizzazione non è banale
    for (auto it = vertices_label_set->begin(); it != vertices_label_set->end(); it++)
        vertices_sets->insert(it->second);

    //TODO questa regione ha inserimenti quindi la parallelizzazione non è banale
    for (auto it = edges_label_set->begin(); it != edges_label_set->end(); it++)
        edges_set->insert(it->second);

    //TODO questa regione ha inserimenti quindi la parallelizzazione non è banale
    for (int i = 0; i < num_vertex; i++)
        vertices_labels[i] = vLabel->at(i);

    //TODO questa regione ha inserimenti quindi la parallelizzazione non è banale
    for (int i = 0; i < num_edge; i++)
        if (IS_EDGE_EMPTY(h, i))
            edges_labels[i] = -1;
        else
            edges_labels[i] = heLabels->at(i);

    delete (vLabel);
    delete (heLabels);
    free(vertices);
    free(edges);
    delete (vertices_label_set);
    delete (edges_label_set);

#ifdef DEBUG
    std::cout << "Parameter finalization for: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0 << std::endl;
#endif // DEBUG

    return new find_communities_struct(vertices_sets, edges_set, vertices_labels, h->nVertex, edges_labels, h->nEdge, current_iter);
}
