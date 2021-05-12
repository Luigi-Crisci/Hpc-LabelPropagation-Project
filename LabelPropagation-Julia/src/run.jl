using SimpleHypergraphs, Random

include("find_communities.jl")

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
Random.seed!(1234);

cflp = CFLabelPropagationFinder(100, 1234)


nv = 1000
ne = 100
full_connected_h = Hypergraph(nv, ne)
p = 0.2
c = 1

while true
    global c
    for i = 1:nv
        for j = 1:ne
            if rand() <= p
                full_connected_h[i, j] = true
            else
                full_connected_h[i, j] = nothing
            end
        end
    end
	println("Iter: ",c)
    c = c + 1
	if length(our_get_connected_components(full_connected_h)) == 1
		break
	end
end

@time comms = our_findcommunities(full_connected_h, cflp)

# add_hyperedge!(h)
# comms = findcommunities(h, cflp)

# @test comms.helabels == [4, 7, -1]

########### Small size