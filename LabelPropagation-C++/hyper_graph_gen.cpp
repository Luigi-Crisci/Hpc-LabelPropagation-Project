#include "headers/label_propagation.h"
#define MAXITER 100
#define SEED 1234

/*
    Size:
        small 5000 300
        medium 10000 600
        large 20000 1200
        very large 40000 2400

    Density:
      sparse 0.3
      normal 0.5
      dense  0.7
*/

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Missing some arguments!!"<< std::endl;
        std::cout << "Usage: \n\tbin/create_h [vertex_number] [edges_number] [p]" << std::endl;
        exit(0);
    }

    int VERTEX_N = std::stoi(argv[1]);
    int HYPER_EDGES_N = std::stoi(argv[2]);
    float p = std::stof(argv[3]);


    MT::MersenneTwist rng;
    rng.init_genrand(SEED);

    HyperGraph *h = new HyperGraph(VERTEX_N,HYPER_EDGES_N);
    {
        for (int i = 0; i < VERTEX_N; i++)
        {
            for (int j = 0; j < HYPER_EDGES_N; j++)
            {
                if (rng.genrand_real1() <= p)
                {
                    
                    h->v2he[i].set(j, true);
                    h->he2v[j].set(i, true);
                }
                else
                {
                    h->v2he[i].set(j, false);
                    h->he2v[j].set(i, false);
                }
            }
        }
    } while (!is_hypergraph_connected(h));

    std::ofstream myFile;

    std::string filename = "../resources/h_test_hypergraph_"+ std::to_string(VERTEX_N) + "_"+ std::to_string(HYPER_EDGES_N) + "_"+ std::to_string(p) + ".txt";
    myFile.open(filename);
    std::stringstream ss;
    ss << VERTEX_N << "\n";
    ss << HYPER_EDGES_N << "\n";

    for (int i = 0; i < VERTEX_N; i++)
    {
        for (int j = 0; j < HYPER_EDGES_N; j++)
        {
            ss<<h->v2he[i].test(j);
        }
    }

    myFile << ss.str();
    
    myFile.close();
}