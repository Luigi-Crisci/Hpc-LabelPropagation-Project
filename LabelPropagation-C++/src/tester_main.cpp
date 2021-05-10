#include "label_propagation.cpp"

// Defines method values
#define MAXITER 100 
#define SEED time(NULL) // ¯\_(ツ)_/¯


int main(int, char **)
{
    // Initialize the Mersenne Twister.
    MT::MersenneTwist rng;
    rng.init_genrand(SEED);



    // Create two Hypergraph
    int vertex_n = 5;
    int hyper_edges_n = 3;
    HyperGraph h_connected(vertex_n, hyper_edges_n);
    HyperGraph h_disconnected(vertex_n, hyper_edges_n);

    float p = 0.6;
    
    // Initialize both Hypergraph with random values [0,1]
    do
    {       
        for (int i = 0; i < vertex_n; i++)
        {
            for (int j = 0; j < hyper_edges_n; j++)
            {
                if (rng.genrand_real1() <= p)
                {
                    (*h_connected.hVertex)[i]->insert((*h_connected.hVertex)[i]->begin() + j, 1);
                }else
                {
                    (*h_connected.hVertex)[i]->insert((*h_connected.hVertex)[i]->begin() + j, 0);
                }
                if (rng.genrand_real1() <= p/2)
                {
                    (*h_disconnected.hVertex)[i]->insert((*h_disconnected.hVertex)[i]->begin() + j, 1);
                }else
                {
                    (*h_disconnected.hVertex)[i]->insert((*h_disconnected.hVertex)[i]->begin() + j, 0);
                }
            }
        }
    }while(!is_hypergraph_connected(&h_connected) || is_hypergraph_connected(&h_disconnected));
    

    // Print the Hypergraphs
    std::cout << "Connected hypegraph" << std::endl;
    for (int i = 0; i < vertex_n; i++)
    {
        for (int j = 0; j < hyper_edges_n; j++)
        {
            std::cout << h_connected.hVertex->at(i)->at(j) << " ";
        }
        std::cout << std::endl;
    }

    // Test find_communities
    CFLabelPropagationFinder method;
    method.seed = SEED;
    method.max_iter = MAXITER;

    //find_communities_struct result_disconnected = find_communities(&h_disconnected, method);
    find_communities_struct result_connected = find_communities(&h_connected, method);

    std::cout<<"np -> "<<result_connected.np<<std::endl;
    for(auto elem : *result_connected.np)
    {
        
        for(auto elem1 : *elem)
        {
            std::cout << elem1 <<std::endl;
        }
    }

    std::cout<<"hep -> "<<result_connected.hep<<std::endl;
    for(auto elem : *result_connected.hep)
    {
        
        for(auto elem1 : *elem)
        {
            std::cout << elem1 <<std::endl;
        }
    }
    std::cout<<"vLabel -> "<<result_connected.vLabel<<std::endl;
    for(auto elem : *result_connected.vLabel)
        {
            std::cout << elem <<std::endl;
        }

    std::cout<<"heLabel -> "<<result_connected.heLabel<<std::endl;
    for(auto elem : *result_connected.heLabel)
        {
            std::cout << elem <<std::endl;
        }
        
    std::cout<<"iter -> "<<result_connected.iter<<std::endl;

    return 0;
}