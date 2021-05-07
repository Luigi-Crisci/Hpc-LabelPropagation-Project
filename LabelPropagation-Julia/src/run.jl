using SimpleHypergraphs, Random

### Write code here
Random.seed!(1234);

cflp = CFLabelPropagationFinder(100, 1234)

h = Hypergraph(11, 2)
h[1:5, 1] .= true
h[5:11, 2] .= true

comms = findcommunities(h, cflp)

add_hyperedge!(h)
comms = findcommunities(h, cflp)

@test comms.helabels == [4, 7, -1]

