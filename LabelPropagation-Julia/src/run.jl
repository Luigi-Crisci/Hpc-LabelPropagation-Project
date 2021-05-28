using SimpleHypergraphs, Random, TimerOutputs

include("find_communities.jl")
include("read_hypergraph.jl")

### Write code here
Random.seed!(1234);

size_v = [100, 5000,10000,20000]
size_e = [20, 300,600,1200]
p_list = [0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8]

io = open("results.log","w+")
cflp = CFLabelPropagationFinder(100, 1234)

h = read_hypergraph("../LabelPropagation-C++/resources/h_test_hypergraph_10000_600_0.800000.txt")
x =  @elapsed(comms = our_findcommunities(h, cflp))

println(io,"Results with vertex [5000], with edges [300] and density [0.3]")
println(io, "Find Communities time: $(round(x, digits=3))\n")
close(io)

for v in size_v
    for e in size_e
        for p in p_list
            if isfile("../LabelPropagation-C++/resources/h_test_hypergraph_$(v)_$(e)_$(p)00000.txt") == true
                h = read_hypergraph("../LabelPropagation-C++/resources/h_test_hypergraph_$(v)_$(e)_$(p)00000.txt")
                result_files = open("results_file/results_julia_$(v)_$(e)_$(p).txt", "w+")
            end
            
            #h = read_hypergraph("../LabelPropagation-C++/resources/h_test_hypergraph_5000_300_0.300000.txt")
            base = 0
            for i in 1:5
                base += @elapsed(our_findcommunities(h, cflp))
                println(result_files,"Results with vertex [$v], with edges [$e] and density [$p]")
                println(result_files, "Find Communities time: $(round(base, digits=3))\n")
                base = 0
            end
            #base /= 5 #aritmetic mean
            #write(io,"$(v) - $(e) - $(p) : $(base)\n")
            close(result_files)
        end
    end
end




# comms[:vlabels]
# comms[:vlabels]