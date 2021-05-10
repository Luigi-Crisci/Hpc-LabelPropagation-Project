#include <gtest/gtest.h>
#include "label_propagation.cpp"
#define MAXITER 100 
#define SEED time(NULL) // ¯\_(ツ)_/¯

void populate_hypergraph(MT::MersenneTwist rng, int vertex_n, int hyper_edges_n, HyperGraph hyper_graph, bool connected)
{

    //TODO:: this method shoudl actually statically create an hypergraph of said dimension connected or disconnected
  float p = 0.6;

  if(connected){
    do
    {       
        for (int i = 0; i < vertex_n; i++)
        {
            for (int j = 0; j < hyper_edges_n; j++)
            {
                if (rng.genrand_real1() <= p)
                {
                    (*hyper_graph.hVertex)[i]->insert((*hyper_graph.hVertex)[i]->begin() + j, 1);
                }else
                {
                    (*hyper_graph.hVertex)[i]->insert((*hyper_graph.hVertex)[i]->begin() + j, 0);
                }
            }
        }
     }while(!is_hypergraph_connected(&hyper_graph));
  }else{
    do
    {       
        for (int i = 0; i < vertex_n; i++)
        {
            for (int j = 0; j < hyper_edges_n; j++)
            {
                if (rng.genrand_real1() <= p/2)
                {
                    (*hyper_graph.hVertex)[i]->insert((*hyper_graph.hVertex)[i]->begin() + j, 1);
                }else
                {
                    (*hyper_graph.hVertex)[i]->insert((*hyper_graph.hVertex)[i]->begin() + j, 0);
                }
            }
        }
     }while(is_hypergraph_connected(&hyper_graph));
  }
}

bool is_set_equal_to_vector(std::vector<int> *vec, std::set<int> *set)
{
    for(auto i:*vec){
        if(set->count(i) == 0){
            return false;
        }
    }
    return true;
}

bool create_and_reverse_map(std::map<int, int> *map, int quo){
    bool flag = true;
    int iter = MAXITER/quo;
    // Half values equals, so we expect each element of the map with 2 elements for each index
    for(int i=0; i<MAXITER; i++){
        map->insert({i, i%iter});
    }

    std::map<int, std::set<int> *> *return_map;

    return_map = reverse_map(map);
    
    return_map->size()==iter?flag=true:flag=false;

    if(!flag)
        return flag;

    for(auto index:*return_map)
    {
        if(index.second->size() != quo){
            flag = false;
        }
            
    }
    return flag;
}

TEST(LabelPropagationNaiveCPort, IsHypergraphConnected)
{
  MT::MersenneTwist rng;
  rng.init_genrand(SEED);
  // Create two Hypergraph
  int vertex_n = 5;
  int hyper_edges_n = 3;
  HyperGraph h_connected(vertex_n, hyper_edges_n);
  HyperGraph h_disconnected(vertex_n, hyper_edges_n);

  populate_hypergraph(rng, vertex_n, hyper_edges_n, h_connected, true);
  populate_hypergraph(rng, vertex_n, hyper_edges_n, h_disconnected, false);

  ASSERT_EQ(true, is_hypergraph_connected(&h_connected));
  ASSERT_EQ(false, is_hypergraph_connected(&h_disconnected)); 
}

TEST(LabelPropagationNaiveCPort, FindCommunities)
{
  MT::MersenneTwist rng;
  rng.init_genrand(SEED);
  CFLabelPropagationFinder method;
  method.seed = SEED;
  method.max_iter = MAXITER;
  int vertex_n = 5;
  int hyper_edges_n = 3;
  HyperGraph h_connected(vertex_n, hyper_edges_n);
  populate_hypergraph(rng, vertex_n, hyper_edges_n, h_connected, true);

  //find_communities_struct result_connected = find_communities(&h_connected, method);

  // std::cout<<"np -> "<<result_connected.np<<std::endl;
  // for(auto elem : *result_connected.np)
  // {
      
  //     for(auto elem1 : *elem)
  //     {
  //         std::cout << elem1 <<std::endl;
  //     }
  // }

  // std::cout<<"hep -> "<<result_connected.hep<<std::endl;
  // for(auto elem : *result_connected.hep)
  // {
      
  //     for(auto elem1 : *elem)
  //     {
  //         std::cout << elem1 <<std::endl;
  //     }
  // }
  // std::cout<<"vLabel -> "<<result_connected.vLabel<<std::endl;
  // for(auto elem : *result_connected.vLabel)
  //     {
  //         std::cout << elem <<std::endl;
  //     }

  // std::cout<<"heLabel -> "<<result_connected.heLabel<<std::endl;
  // for(auto elem : *result_connected.heLabel)
  //     {
  //         std::cout << elem <<std::endl;
  //     }
      
  // std::cout<<"iter -> "<<result_connected.iter<<std::endl;


  ASSERT_NO_FATAL_FAILURE(find_communities(&h_connected, method));
}

TEST(LabelPropagationNaiveCPort, Shuffle)
{//pippo
    MT::MersenneTwist rng;
    rng.init_genrand(SEED);

    std::vector<int> *array = new std::vector<int>();
    std::set<int> *set = new std::set<int>();
    
    for(int i=0; i<MAXITER; i++){
        
        array->push_back(i);
        
        set -> insert(i);
        
    }
   
    shuffle(array, rng);
    
    EXPECT_TRUE(is_set_equal_to_vector(array, set));
}

TEST(LabelPropagationNaiveCPort, ReverseMap)
{
    std::map<int, int> *map = new std::map<int, int>();

    // All value differents
    EXPECT_TRUE(create_and_reverse_map(map, MAXITER));

    // 2 values equals
    map->clear();
    EXPECT_TRUE(create_and_reverse_map(map, MAXITER/2));

    // expected false
    map->clear();
    EXPECT_FALSE(create_and_reverse_map(map, MAXITER/3));

    //4 values equals
    map->clear();
    EXPECT_TRUE(create_and_reverse_map(map, MAXITER/4));

    //all values equals
    map->clear();
    EXPECT_TRUE(create_and_reverse_map(map, 1));
}

TEST(LabelPropagationNaiveCPort, GetEdges)
{
    MT::MersenneTwist rng;
    rng.init_genrand(SEED);
    // Create two Hypergraph
    int vertex_n = 5;
    int hyper_edges_n = 3;
    HyperGraph h_connected(vertex_n, hyper_edges_n);
    HyperGraph h_disconnected(vertex_n, hyper_edges_n);

    populate_hypergraph(rng, vertex_n, hyper_edges_n, h_connected, true);
    populate_hypergraph(rng, vertex_n, hyper_edges_n, h_disconnected, false);

    bool disconnected_edge_found = false;
    for(int i=0; i<vertex_n; i++)
    {
        EXPECT_TRUE(get_edges(&h_connected, i)->size() >= 1);
        EXPECT_FALSE(get_edges(&h_connected, i)->size() < 1);
        if (get_edges(&h_disconnected, i)->size() < 1)
            disconnected_edge_found = true;
        else
            EXPECT_TRUE(get_edges(&h_disconnected, i)->size() >= 1);
    }

    EXPECT_TRUE(disconnected_edge_found);
    if(!disconnected_edge_found){
        std::cout << "Disconnected hypegraph" << std::endl;
        for (int i = 0; i < vertex_n; i++)
        {
            for (int j = 0; j < hyper_edges_n; j++)
            {
                std::cout << h_disconnected.hVertex->at(i)->at(j) << " ";
            }
            std::cout << std::endl;
        }

    }

}

TEST(LabelPropagationNaiveCPort, GetVertices)
{
    MT::MersenneTwist rng;
    rng.init_genrand(SEED);
    // Create two Hypergraph
    int vertex_n = 5;
    int hyper_edges_n = 3;
    HyperGraph h_connected(vertex_n, hyper_edges_n);
    HyperGraph h_disconnected(vertex_n, hyper_edges_n);

    populate_hypergraph(rng, vertex_n, hyper_edges_n, h_connected, true);
    populate_hypergraph(rng, vertex_n, hyper_edges_n, h_disconnected, false);

    for(int i=0; i<hyper_edges_n; i++)
    {
        EXPECT_TRUE(get_vertices(&h_connected, i)->size() >= 1);
        EXPECT_FALSE(get_vertices(&h_connected, i)->size() < 1);
        EXPECT_TRUE(get_vertices(&h_disconnected, i)->size() >= 0);// we cant say much here because there could be or not edges without vertices
    }
}

TEST(LabelPropagationNaiveCPort, GetConnectedComponent)
{
    MT::MersenneTwist rng;
    rng.init_genrand(SEED);
    // Create two Hypergraph
    int vertex_n = 5;
    int hyper_edges_n = 3;
    HyperGraph h_connected(vertex_n, hyper_edges_n);

    populate_hypergraph(rng, vertex_n, hyper_edges_n, h_connected, true);

    std::vector<int> connected_comp;
    std::vector<int> visited(h_connected.nVertex);

    for(int i=0; i<vertex_n; i++){
        get_connected_component(&h_connected, &visited, &connected_comp, i);
        
        // for(auto elem:connected_comp){
        //     std::cout<<elem<<std::endl;
        // }
        // std::cout<<std::endl;
        connected_comp.clear();
    }
        
}