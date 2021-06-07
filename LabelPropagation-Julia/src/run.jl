using SimpleHypergraphs, Random, TimerOutputs, Statistics

include("find_communities.jl")
include("read_hypergraph.jl")

Random.seed!(1234);

v = ARGS[1]
e = ARGS[2]
p = ARGS[3]

cflp = CFLabelPropagationFinder(100, 1234)
global base = []

if isfile("../../LabelPropagation-C++/resources/h_test_hypergraph_$(v)_$(e)_$(p)00000.txt")
    h = read_hypergraph("../../LabelPropagation-C++/resources/h_test_hypergraph_$(v)_$(e)_$(p)00000.txt")
    for i in 1:5
        push!(base,@elapsed(our_findcommunities(h, cflp))) 
    end
    println("Results with vertex [$v], with edges [$e] and density [$p]")
    println("Find Communities time: $(round(median(base), digits=3))\n")
else
    print("NON ESISTE - h_test_hypergraph_$(v)_$(e)_$(p)00000.txt\n")
end