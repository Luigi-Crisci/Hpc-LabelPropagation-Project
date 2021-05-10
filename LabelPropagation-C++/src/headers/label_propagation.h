#include <map>
#include <set>
#include <vector>
#include "headers/mtrnd.h"

typedef std::vector<std::vector<int> *> *Int_Matrix;
typedef struct CFLabelPropagationFinder
{
    int max_iter;
    int seed;
} CFLabelPropagationFinder;

/*
    Hyper-graph structure
*/
typedef struct HyperGraph
{
    int nVertex, nEdge;
    Int_Matrix hVertex;

    HyperGraph(int nVertex, int nEdge)
    {
        this->nVertex = nVertex;
        this->nEdge = nEdge;
        hVertex = new std::vector<std::vector<int> *>(nVertex);
        for (int i = 0; i < nVertex; i++)
        {
            (*hVertex)[i] = new std::vector<int>(nEdge);
        }
    }

} HyperGraph;

typedef struct find_communities_struct
{

    std::set<std::set<int> *> *np;
    std::set<std::set<int> *> *hep;
    std::vector<int> *vLabel;
    std::vector<int> *heLabel;
    int iter;

    find_communities_struct(std::set<std::set<int> *> *_np, std::set<std::set<int> *> *_hep, std::vector<int> *_vLabel,
                            std::vector<int> *_heLabel, int _iter)
    {
        np = _np;
        hep = _hep;
        vLabel = _vLabel;
        heLabel = _heLabel;
        iter = _iter;
    }

} find_communities_struct;

find_communities_struct findCommunities(HyperGraph *h, CFLabelPropagationFinder parameters);