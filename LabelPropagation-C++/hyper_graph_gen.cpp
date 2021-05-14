#include <fstream>
#include <sstream>
#include "headers/label_propagation.h"

void read_files(std::string sFilename);

int main()
{
    std::ofstream myFile;
    myFile.open("resources/hyper_graph_test.txt");

    float p = 0.3;
    u_int vertex_n = 5;
    u_int hyper_edges_n = 3;

    HyperGraph *hyper_graph = new HyperGraph(vertex_n, hyper_edges_n);

	CFLabelPropagationFinder parameter;
	parameter.max_iter = 100;
	parameter.seed = 1234;

    do{
        for (int i = 0; i < vertex_n; i++)
        {
            for (int j = 0; j < hyper_edges_n; j++)
            {
                double cp = ((double) rand() / RAND_MAX);               
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
    }while (!is_hypergraph_connected(hyper_graph));

    std::stringstream ss;
    for (int i = 0; i < vertex_n; i++)
    {
        for(int j=0; j<hyper_edges_n; j++){
            ss<<hyper_graph->v2he[i]->count(j);
        }
    }

    myFile<<ss.str();
    myFile.close();

    read_files("hyper_graph_test.txt");
}

void read_files(std::string sFilename){

    std::ifstream fileSource(sFilename);

    if(!fileSource){
        std::cout << "Cannot open  " << sFilename << std::endl;
        exit(-1);
    }
    else{
        std::string buffer;

        while(fileSource>>buffer){
            std::cout << buffer << std::endl;
        }
    }
}