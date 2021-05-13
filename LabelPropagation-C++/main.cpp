#include <iostream>
#include "random"
#include "headers/label_propagation.h"
#include <chrono>
#include <utility>

#define N_VERTEX 1000
#define N_EDGES 100


void populate_hypergraph(int vertex_n, int hyper_edges_n, HyperGraph *hyper_graph)
{

    double p = 0.2;
	bool connected = true;
	int count = 1;
    if (connected)
    {
        do
        {
            for (int i = 0; i < vertex_n; i++)
            {
                for (int j = 0; j < hyper_edges_n; j++)
                {
                    double cp = ((double) rand() / RAND_MAX);
                    // std::cout<<"cp: "<<cp<<std::endl;                    
                    if (  cp <= p)
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
			std::cout<<"Current iter: "<<count++<<std::endl;
        } while (!is_hypergraph_connected(hyper_graph));
	}
}


int main()
{

	srand(time(NULL));

	HyperGraph *h = new HyperGraph(N_VERTEX,N_EDGES);
	populate_hypergraph(N_VERTEX,N_EDGES,h);

	CFLabelPropagationFinder parameter;
	parameter.max_iter = 100;
	parameter.seed = 1234;

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	find_communities_struct *res = find_communities(h,parameter);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout<<"Elapsed time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0<<std::endl;
	std::cout<<"Number of iteration: "<<res->iter<<std::endl<<std::endl;
    
    // print_find_communities_struct(res);

	return 0;
}