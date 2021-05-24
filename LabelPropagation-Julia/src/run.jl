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

h = read_hypergraph("../LabelPropagation-C++/resources/h_test_hypergraph_5000_300_0.300000.txt")
x =  @elapsed(comms = our_findcommunities(h, cflp))

for v in size_v
    for e in size_e
        for p in p_list
            h = read_hypergraph("../LabelPropagation-C++/resources/h_test_hypergraph_$(v)_$(e)_$(p)00000.txt")
            result_files = open("results_file/results_julia_$(v)_$(e)_$(p).txt", "w+")
            #h = read_hypergraph("../LabelPropagation-C++/resources/h_test_hypergraph_5000_300_0.300000.txt")
            base = 0
            for i in 1:5
                base += @elapsed(our_findcommunities(h, cflp))
                write(result_files,"Results with vertex [$v], with edges [$e] and density [$p]\n")
                write(result_files, "Find Communities time: $(base)\n")
                base = 0
            end
            #base /= 5 #aritmetic mean
            #write(io,"$(v) - $(e) - $(p) : $(base)\n")
            close(result_files)
        end
    end
end

close(io)


# comms[:vlabels]