using SimpleHypergraphs, Random, TimerOutputs

include("find_communities.jl")
include("read_hypergraph.jl")

### Write code here
Random.seed!(1234);

size_v = [5000,10000,20000]
size_e = [300,600,1200]
p_list = [0.3,0.5,0.7]

io = open("results.log","w+")
cflp = CFLabelPropagationFinder(100, 1234)

for v in size_v
    for e in size_e
        for p in p_list
            h = read_hypergraph("../LabelPropagation-C++/resources/h_test_hypergraph_$(v)_$(e)_$(p)00000.txt")
            base = 0
            for i in 1:5
                base += @elapsed(our_findcommunities(h, cflp))
            end
            base /= 5 #aritmetic mean
            write(io,"$(v) - $(e) - $(p) : $(base)\n")
        end
    end
end



            
            
            h = read_hypergraph("../LabelPropagation-C++/resources/h_test_hypergraph_1000_80.txt")
            
x =  @elapsed(comms = our_findcommunities(h, cflp))

x

comms[:vlabels]
