#include "headers/label_propagation.h"

#define MAXITER 100
#define SEED 1234
#define VERTEX_N 10000
#define HYPER_EDGES_N 600 

int main(int argc, char *argv[])
{
    MT::MersenneTwist rng;
    rng.init_genrand(SEED);

    HyperGraph *h = new HyperGraph(VERTEX_N,HYPER_EDGES_N);
    float p = 0.3;
    do
    {
        for (int i = 0; i < VERTEX_N; i++)
        {
            for (int j = 0; j < HYPER_EDGES_N; j++)
            {
                if (rng.genrand_real1() <= p)
                {
                    h->v2he[i]->try_emplace(j, true);
                    h->he2v[j]->try_emplace(i, true);
                }
                else
                {
                    h->v2he[i]->erase(j);
                    h->he2v[j]->erase(i);
                }
            }
        }
    } while (!is_hypergraph_connected(h));

    std::ofstream myFile;

    std::string filename = "../resources/h_test_hypergraph_"+ std::to_string(VERTEX_N) + "_"+ std::to_string(HYPER_EDGES_N) + ".txt";
    myFile.open(filename);
    std::stringstream ss;
    ss << VERTEX_N << "\n";
    ss << HYPER_EDGES_N << "\n";

    for (int i = 0; i < VERTEX_N; i++)
    {
        for (int j = 0; j < HYPER_EDGES_N; j++)
        {
            ss << h->v2he[i]->count(j);
        }
    }

    myFile << ss.str();
    
    myFile.close();
}