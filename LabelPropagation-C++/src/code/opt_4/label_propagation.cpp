#include "utils.cpp"
#include <queue>
#include <omp.h>

#define CACHE_LINE_SIZE 64
// #define THREAD_INDEX(i, thread_num) i + thread_num *CACHE_LINE_SIZE

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

void shuffle(int *element, int *labels, int size, MT::MersenneTwist rng)
{
    if (size > 1)
    {
        size_t i;
        for (i = 0; i < size - 1; i++)
        {
            size_t j = i + GENRANDOM(rng) / (RAND_MAX / (size - i) + 1);
            //Swap element vector
            int temp = element[j];
            element[j] = element[i];
            element[i] = temp;
            //Swap labels vector
            temp = labels[j];
            labels[j] = labels[i];
            labels[i] = temp;
        }
    }
}

inline void update_ordered_labels(int *labels, int *ordered_labes, int *vertex_indices, int size)
{
    for (int i = 0; i < size; i++)
        ordered_labes[vertex_indices[i]] = labels[i];
}

std::unordered_map<int, std::unordered_set<int> *> *reverse_map(int *vet, int size)
{
    std::unordered_map<int, std::unordered_set<int> *> *values_label_set = new std::unordered_map<int, std::unordered_set<int> *>;
    for (int i = 0; i < size; i++)
    {

        int value = vet[i];
        int key = i;

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

int compute_vertex_label(HyperGraph *h, int v, int *vlabel, int *heLables, MT::MersenneTwist rng)
{
    std::bitset<MAX_SIZE> *edges_bitset = GET_EDGES(h, v);
    int edges_size = edges_bitset->count();

    if (edges_size == 0)
        return -1;

    int *edges = get_vertices_indices(edges_bitset, h->nEdge);

    std::unordered_map<int, int> *vertex_label_list = new std::unordered_map<int, int>;
    std::unordered_set<int> *max_vertex_label_found = new std::unordered_set<int>;

    int max = 0;
    int current_label, current_edge;

    shuffle(edges, edges_size, rng);

    for (int i = 0; i < edges_size; i++)
    {
        current_edge = edges[i];

        current_label = heLables[current_edge];

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

    if (vlabel[v] != -1 && max_vertex_label_found->find(vlabel[v]) != max_vertex_label_found->end())
        return vlabel[v];
    return *(max_vertex_label_found->begin());
}

int compute_edge_label(HyperGraph *h, int e, int *vlabel, int *heLables, MT::MersenneTwist rng)
{
    std::bitset<MAX_SIZE> *vertices_bitset = GET_VERTICES(h, e);
    int vertices_size = vertices_bitset->count();

    if (vertices_size == 0)
        return -1;

    int *vertices = get_vertices_indices(vertices_bitset, h->nVertex);

    int max = 0, current_label, current_vertex, current_index;

    std::unordered_map<int, int> *edge_label_list = new std::unordered_map<int, int>;
    std::unordered_set<int> *max_edge_label_found = new std::unordered_set<int>; //this is not freed

    shuffle(vertices, vertices_size, rng);

    for (int i = 0; i < vertices_size; i++)
    {
        current_vertex = vertices[i];
        current_label = vlabel[current_vertex];

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

    if ((heLables[e] != -1) && max_edge_label_found->find(heLables[e]) != max_edge_label_found->end())
    {
        return heLables[e];
    }

    return *(max_edge_label_found->begin());
}

//TODO:: non sono sicuro che la parallelizzazione funziona
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
        current = frontier.front(); //TODO:: usare indice array per simulare la coda così da poter parallelizzare?
        frontier.pop();

#pragma omp parallel for firstprivate(current, graph_size)
        for (int i = 0; i < graph_size; i++)
        {
            bool connected = graph[current][i];
            bool c = !checked[i];
            if (graph[current][i] && !checked[i])
            {
#pragma omp critical
                {
                    if (graph[current][i] && !checked[i])
                    {
                        checked[i] = true;
                        frontier.push(i);
                        count++;
                    }
                }
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
    MT::MersenneTwist rng;
    rng.init_genrand(parameters.seed);

    int num_edge = h->nEdge;
    int num_vertex = h->nVertex;
    int new_label;

    bool stop = false;
    int current_iter;

    int vLabel_size = next_multiple(num_vertex, CACHE_LINE_SIZE);
    int heLabel_size = next_multiple(num_edge, CACHE_LINE_SIZE);

    int *vLabel = (int *) std::aligned_alloc(CACHE_LINE_SIZE, vLabel_size * sizeof(int));
    int *ordered_vLabel = (int *) std::aligned_alloc(CACHE_LINE_SIZE, vLabel_size * sizeof(int));
    int *heLabels = (int *) std::aligned_alloc(CACHE_LINE_SIZE, heLabel_size * sizeof(int));
    int *ordered_heLabels = (int *) std::aligned_alloc(CACHE_LINE_SIZE, heLabel_size * sizeof(int));

    int *vertices = (int *)calloc(num_vertex, sizeof(int));
    int *edges = (int *)calloc(num_edge, sizeof(int));

    for (int i = 0; i < num_vertex; i++)
        vertices[i] = i;
    for (int i = 0; i < num_edge; i++)
        edges[i] = i;

#pragma omp parallel
    {
#pragma omp for nowait
        for (int i = 0; i < vLabel_size; i++)
        {
            if (i < num_vertex)
            {
                vLabel[i] = i;
                ordered_vLabel[i] = i;
            }
        }

#pragma omp for nowait
        for (int i = 0; i < heLabel_size; i++)
        {
            if (i < num_edge)
            {
                heLabels[i] = -1;
                ordered_heLabels[i] = -1;
            }
        }

#pragma omp single
        {
#ifdef DEBUG
            std::chrono::steady_clock::time_point start;
            start = std::chrono::steady_clock::now();
#endif
            assert(is_hypergraph_connected(h));

#ifdef DEBUG
            std::cout << "Time Is_Hypergraph connected: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0 << std::endl;
            start = std::chrono::steady_clock::now();
#endif
        }
    }

#ifdef DEBUG
    std::chrono::steady_clock::time_point start;
    start = std::chrono::steady_clock::now();
#endif
    for (current_iter = 1; !stop && current_iter < parameters.max_iter; current_iter++)
    {
        stop = true;

#pragma omp parallel
        {
#pragma omp single
            {
                shuffle(edges, heLabels, num_edge, rng);
            }

#pragma omp for
            for (int i = 0; i < heLabel_size; i++)
            {
                if (i < num_edge)
                {
                    if (GET_EDGE_VERTICES_CONNECTED_NUMBER(h, edges[i]) > 0)
                        heLabels[i] = compute_edge_label(h, edges[i], ordered_vLabel, ordered_heLabels, rng);
                }
            }

#pragma omp single nowait
            {
                update_ordered_labels(heLabels, ordered_heLabels, edges, num_edge);
            }
#pragma omp single
            {
                shuffle(vertices, vLabel, num_vertex, rng);
            }

#pragma omp for private(new_label)
            for (int i = 0; i < vLabel_size; i++)
            {
                if (i < num_vertex)
                {
                    new_label = compute_vertex_label(h, vertices[i], ordered_vLabel, ordered_heLabels, rng);
                    if (new_label != vLabel[i])
                    {
                        stop &= 0;
                    }
                    vLabel[i] = new_label;
                }
            }

#pragma omp single
            {
                update_ordered_labels(vLabel, ordered_vLabel, vertices, num_vertex);
            }
        }
    }

#ifdef DEBUG
    std::cout << "Time Community for: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0 << std::endl;
    start = std::chrono::steady_clock::now();
#endif // DEBUG

    //TODO: reverse map parallel???
    //Get sets of Vertices lables
    std::unordered_map<int, std::unordered_set<int> *> *vertices_label_set = reverse_map(vLabel, num_vertex);
    //Get sets of Edges lables
    std::unordered_map<int, std::unordered_set<int> *> *edges_label_set = reverse_map(heLabels, num_edge);

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

    free(vertices);
    free(edges);
    delete (vertices_label_set);
    delete (edges_label_set);

#ifdef DEBUG
    std::cout << "Parameter finalization for: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0 << std::endl;
#endif // DEBUG

    return new find_communities_struct(vertices_sets, edges_set, vLabel, h->nVertex, heLabels, h->nEdge, current_iter);
}
