#include <gtest/gtest.h>
#include "headers/label_propagation.h"

#define MAXITER 100
#define SEED 1234
#define VERTEX_N 1000    //100
#define HYPER_EDGES_N 60 //20

void populate_hypergraph(MT::MersenneTwist rng, int vertex_n, int hyper_edges_n, HyperGraph *hyper_graph, bool connected)
{
    //TODO:: this parameters shoudl actually statically create an hypergraph of said dimension connected or disconnected
    float p = 0.9;

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

                        (*(hyper_graph->hVertex->at(i)))[j] = 1;
                    }
                    else
                    {

                        (*(hyper_graph->hVertex->at(i)))[j] = 0;
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
                    if (rng.genrand_real1() <= p / 2)
                    {

                        (*(hyper_graph->hVertex->at(i)))[j] = 1;
                    }
                    else
                    {
                        (*(hyper_graph->hVertex->at(i)))[j] = 0;
                    }
                }
            }
        } while (is_hypergraph_connected(hyper_graph));
    }
}

bool is_set_equal_to_vector(std::vector<int> *vec, std::unordered_set<int> *set)
{
    for (auto i : *vec)
    {
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
    int iter = MAXITER / quo;
    // Half values equals, so we expect each element of the map with 2 elements for each index
    for (int i = 0; i < MAXITER; i++)
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

    for (int seed = 0; seed < SEED; seed++)
    {
        rng.init_genrand(seed);

        std::vector<int> *array = new std::vector<int>();
        std::unordered_set<int> *set = new std::unordered_set<int>();

        for (int i = 0; i < VERTEX_N; i++)
        {

            array->push_back(i);

            set->insert(i);
        }
        shuffle(array, rng);
        EXPECT_TRUE(is_set_equal_to_vector(array, set));
        if (!is_set_equal_to_vector(array, set))
            std::cout << "Seed: " << seed << std::endl;
    }
}

TEST(NaiveCPort, ReverseMap)
{
    std::unordered_map<int, int> *map = new std::unordered_map<int, int>();

    // All value differents
    EXPECT_TRUE(create_and_reverse_map(map, MAXITER));

    // 2 values equals
    map->clear();
    EXPECT_TRUE(create_and_reverse_map(map, MAXITER / 2));

    // expected false
    map->clear();
    EXPECT_FALSE(create_and_reverse_map(map, MAXITER / 3));

    //4 values equals
    map->clear();
    EXPECT_TRUE(create_and_reverse_map(map, MAXITER / 4));

    //all values equals
    map->clear();
    EXPECT_TRUE(create_and_reverse_map(map, 1));
}

TEST(NaiveCPort, ComputeEdgeLabel)
{
    MT::MersenneTwist rng;
    // for(int seed=0; seed<SEED; seed++){
    rng.init_genrand(SEED);

    int vertex_n = VERTEX_N;
    int hyper_edges_n = HYPER_EDGES_N;

    HyperGraph *h_connected = new HyperGraph(vertex_n, hyper_edges_n);
    populate_hypergraph(rng, vertex_n, hyper_edges_n, h_connected, true);

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
        // if(resul>vertex_n)
        //     std::cout<<"Seed: "<<seed<<std::endl;
    }

    free_hypergraph(h_connected);
    free(vLabel);
    free(hLabel);
    // }
}

TEST(NaiveCPort, ComputeVertexLabel)
{
    MT::MersenneTwist rng;
    // for(int seed=0; seed<SEED; seed++){

    rng.init_genrand(SEED);

    int vertex_n = VERTEX_N;
    int hyper_edges_n = HYPER_EDGES_N;

    HyperGraph *h_connected = new HyperGraph(vertex_n, hyper_edges_n);
    populate_hypergraph(rng, vertex_n, hyper_edges_n, h_connected, true);

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
    free(vLabel);
    free(hLabel);
    // }
}

TEST(NaiveCPort, find_communities_test)
{
    MT::MersenneTwist rng;
    rng.init_genrand(SEED);

    int vertex_n = VERTEX_N;
    int hyper_edges_n = HYPER_EDGES_N;

    HyperGraph *h_connected = new HyperGraph(vertex_n, hyper_edges_n);
    populate_hypergraph(rng, vertex_n, hyper_edges_n, h_connected, true);
    CFLabelPropagationFinder parameters;
    parameters.seed = SEED;
    parameters.max_iter = MAXITER;

    find_communities_struct *return_find_comm;
    ASSERT_NO_FATAL_FAILURE(return_find_comm = find_communities(h_connected, parameters));
}
