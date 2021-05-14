#include <gtest/gtest.h>
#include "headers/label_propagation.h"

#define MAXITER 100
#define SEED 1234
#define VERTEX_N 10000      //100
#define HYPER_EDGES_N 200   //20

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

    
    std::ofstream myFile;
    if(connected)
        m_name = m_name + "_connected";
    else
        m_name = m_name + "_disconnected";
    std::string filename = "../resources/hyper_graph_test_"+m_name+".txt";
    myFile.open(filename);
    std::stringstream ss;
    ss<<VERTEX_N<<"\n";
    ss<<HYPER_EDGES_N<<"\n";
   
    for (int i = 0; i < vertex_n; i++)
    {
        for(int j=0; j<hyper_edges_n; j++){
            ss<<hyper_graph->v2he[i]->count(j);
        }
        ss<<std::endl;
    }
    
    // for (int i = 0; i < hyper_edges_n; i++)
    // {
    //     for(int j=0; j<vertex_n; j++){
    //         ss<<hyper_graph->he2v[i]->count(j);
    //     }
    //     ss<<std::endl;
    // }

    myFile<<ss.str();
    myFile.close();
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

TEST(NaiveCPort, Shuffle)
{
    MT::MersenneTwist rng;
    
    rng.init_genrand(SEED);

    //std::vector<int> *array = new std::vector<int>();
    int *array = (int*) calloc(VERTEX_N, sizeof(int));
    std::set<int> *set = new std::set<int>();

    for (int i = 0; i < VERTEX_N; i++)
    {
        array[i]=i;
        set->insert(i);
    }

    // std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    shuffle(array, VERTEX_N, rng);

    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // std::cout<<"Shuffle time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0<<std::endl;

    bool is_set_equals_to_vector = is_set_equal_to_vector(array, set, VERTEX_N);
    
    EXPECT_TRUE(is_set_equals_to_vector);
    
}

TEST(NaiveCPort, ReverseMap)
{
    std::unordered_map<int, int> *map = new std::unordered_map<int, int>();

    // All value differents
    EXPECT_TRUE(create_and_reverse_map(map, VERTEX_N));

    // 2 values equals
    map->clear();
    EXPECT_TRUE(create_and_reverse_map(map, VERTEX_N / 2));

    // expected false
    map->clear();
    EXPECT_FALSE(create_and_reverse_map(map, VERTEX_N / 3));

    //4 values equals
    map->clear();
    EXPECT_TRUE(create_and_reverse_map(map, VERTEX_N / 4));

    //all values equals
    map->clear();
    EXPECT_TRUE(create_and_reverse_map(map, 1));
}

TEST(NaiveCPort, ComputeEdgeLabel)
{
    MT::MersenneTwist rng;
    rng.init_genrand(SEED);
    int vertex_n = VERTEX_N;
    int hyper_edges_n = HYPER_EDGES_N;

    HyperGraph *h_connected = new HyperGraph(vertex_n, hyper_edges_n);
    populate_hypergraph(rng, vertex_n, hyper_edges_n, h_connected, true, "compute_edge_label");

    std::unordered_map<int, int> *vLabel = new std::unordered_map<int, int>();
    for (int i = 0; i < vertex_n; i++)
    {
        vLabel->insert({i, i});
    }

    std::unordered_map<int, int> *hLabel = new std::unordered_map<int, int>();
    for (int i = 0; i < hyper_edges_n; i++)
    {
        hLabel->insert({i, i});
    }

    for (int edge = 0; edge < hyper_edges_n; edge++)
    {
        int resul = compute_edge_label(h_connected, edge, vLabel, hLabel, rng);
        ASSERT_LT(resul, vertex_n);
    }

    free_hypergraph(h_connected);
    delete(vLabel);
    delete(hLabel);

}

TEST(NaiveCPort, ComputeVertexLabel)
{
    MT::MersenneTwist rng;
    rng.init_genrand(SEED);

    int vertex_n = VERTEX_N;
    int hyper_edges_n = HYPER_EDGES_N;

    HyperGraph *h_connected = new HyperGraph(vertex_n, hyper_edges_n);
    populate_hypergraph(rng, vertex_n, hyper_edges_n, h_connected, true, "compute_vertex_label");

    std::unordered_map<int, int> *vLabel = new std::unordered_map<int, int>();
    for (int i = 0; i < vertex_n; i++)
    {
        vLabel->insert({i, i});
    }

    std::unordered_map<int, int> *hLabel = new std::unordered_map<int, int>();
    for (int i = 0; i < hyper_edges_n; i++)
    {
        hLabel->insert({i, i});
    }

    for (int vertex = 0; vertex < vertex_n; vertex++)
    {
        int resul = compute_vertex_label(h_connected, vertex, vLabel, hLabel, rng);
        ASSERT_LT(resul, vertex_n);
        // if(resul>vertex_n)
        //     std::cout<<"Seed: "<<seed<<std::endl;
    }

    free_hypergraph(h_connected);
    delete(vLabel);
    delete(hLabel);
}

TEST(NaiveCPort, FindCommunities)
{
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
    ASSERT_NO_FATAL_FAILURE(return_find_comm = find_communities(h_connected, parameters));
}
