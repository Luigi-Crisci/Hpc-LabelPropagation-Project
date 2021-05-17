#include "headers/label_propagation.h"

#define MAXITER 100
#define SEED 1234
#define VERTEX_N 1000      
#define HYPER_EDGES_N 80   

void populate_hypergraph(MT::MersenneTwist rng, int vertex_n, int hyper_edges_n, HyperGraph *hyper_graph, bool connected, std::string m_name)
{
    float p = 0.3;

    if (connected)
    {
        do
        {
            for (int i = 0; i < vertex_n; i++)
            {
                for (int j = 0; j < hyper_edges_n; j++)
                {
                    if (rng.genrand_real1() <= p)
                    {
                        hyper_graph->v2he[i]->try_emplace(j,true);
                        hyper_graph->he2v[j]->try_emplace(i,true);
                    }
                    else
                    {
                        hyper_graph->v2he[i]->erase(j);
                        hyper_graph->he2v[j]->erase(i);
                    }
                }
            }
        } while (!is_hypergraph_connected(hyper_graph));
    }
    else
    {
        do
        {
            for (int i = 0; i < vertex_n; i++)
            {
                for (int j = 0; j < hyper_edges_n; j++)
                {
                    if (rng.genrand_real1() >= p)
                    {

                        hyper_graph->v2he[i]->try_emplace(j,true);
                        hyper_graph->he2v[j]->try_emplace(i,true);
                    }
                    else
                    {
                        hyper_graph->v2he[i]->erase(j);
                        hyper_graph->he2v[j]->erase(i);
                    }
                }
            }
        } while (is_hypergraph_connected(hyper_graph));
    }
}

bool is_set_equal_to_vector(int *vec, std::set<int> *set, u_int size)
{
    for(int i=0; i<size; i++){
        if (set->count(i) == 0)
        {
            return false;
        }
    }

    return true;
}

bool create_and_reverse_map(std::unordered_map<int, int> *map, int quo)
{
    bool flag = true;
    int iter = VERTEX_N / quo;
    // Half values equals, so we expect each element of the map with 2 elements for each index
    for (int i = 0; i < VERTEX_N; i++)
    {
        map->insert({i, i % iter});
    }

    std::unordered_map<int, std::unordered_set<int> *> *return_map;

    return_map = reverse_map(map);

    return_map->size() == iter ? flag = true : flag = false;

    if (!flag)
        return flag;

    for (auto index : *return_map)
    {
        if (index.second->size() != quo)
        {
            flag = false;
        }
    }
    return flag;
}



int main(){
    MT::MersenneTwist rng;
    rng.init_genrand(SEED);

    int vertex_n = VERTEX_N;
    int hyper_edges_n = HYPER_EDGES_N;

    HyperGraph *h_connected = new HyperGraph(vertex_n, hyper_edges_n);
    populate_hypergraph(rng, vertex_n, hyper_edges_n, h_connected, true, "find_communities");
    CFLabelPropagationFinder parameters;
    parameters.seed = SEED;
    parameters.max_iter = MAXITER;

    find_communities_struct *return_find_comm;

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    return_find_comm = find_communities(h_connected, parameters);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout<<"Find Communities time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0<<std::endl;


    return 0;
}