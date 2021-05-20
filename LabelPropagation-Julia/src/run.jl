using SimpleHypergraphs, Random

include("find_communities.jl")
include("read_hypergraph.jl")

"""
    _walk!(h::Hypergraph, s::AbstractVector{Int}, i::Int, visited::AbstractVector{Bool})
Appends the list of neighbors `s` of a given vertex `i` (an auxiliary function for `get_connected_components`).
"""
function _our_walk!(h::Hypergraph, s::AbstractVector{Int}, i::Int, visited::AbstractVector{Bool})
    visited[i] && return
    visited[i] = true
    push!(s, i)
    for he in keys(gethyperedges(h, i))
        for j in keys(getvertices(h, he))
            _our_walk!(h, s, j, visited)
        end
    end
end


"""
    get_connected_components(h::Hypergraph)
Return an array of connected components in the hypergraph `h`
(array of vectors of vertices) using recurrence.
"""
function our_get_connected_components(h::Hypergraph)
    visited = falses(nhv(h))
    cc = Vector{Int}[]
        for i in 1:nhv(h)
            if !visited[i]
                s = Int[]
                _our_walk!(h, s, i, visited)
                push!(cc, s)
        end
    end
    cc
end

### Write code here
Random.seed!(0);

h = read_hypergraph("../LabelPropagation-C++/resources/h_test_hypergraph_1000_80.txt")

cflp = CFLabelPropagationFinder(100, 0)

comms = our_findcommunities(h, cflp)

comms[:vlabels]

# add_hyperedge!(h)
# comms = findcommunities(h, cflp)

# @test comms.helabels == [4, 7, -1]

########### Small size