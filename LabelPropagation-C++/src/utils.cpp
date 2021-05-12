#include <iostream>
#include "headers/label_propagation.h"

void print_find_communities_struct(find_communities_struct* res){
    using namespace std;
    std::cout<<"Vertex labels: [";
    for(auto x: *(res->vLabel))
        std::cout<<x<<", ";
    std::cout<<"]"<<std::endl;

    std::cout<<"Edge labels: [";
    for(auto x: *(res->heLabel))
        std::cout<<x<<", ";
    std::cout<<"]"<<std::endl;

    std::cout<<"Vertex communities: "<<std::endl;
    for(auto set: *(res->np))
    {
        std::cout<<"- [";
        for(auto value: *set)
            std::cout<<value<<", ";
        std::cout<<"]"<<std::endl;
    }

    std::cout<<"Edge communities: "<<std::endl;
    for(auto set: *(res->hep))
    {
        std::cout<<"- [";
        for(auto value: *set)
            std::cout<<value<<", ";
        std::cout<<"]"<<std::endl;
    }
}