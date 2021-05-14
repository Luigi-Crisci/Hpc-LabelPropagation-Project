using SimpleHypergraphs
function read_hypergraph(file::String)
    n_vertex = 0
    n_edge = 0
    io = open(file, "r")
    n_vertex = parse(Int, readline(io))
    n_edge = parse(Int, readline(io))

    h = Hypergraph(n_vertex, n_edge)
	
	vertex_connected = AbstractVector{UInt8}([])
	v = 1
	while !eof(io)
		readbytes!(io,vertex_connected,n_edge)
		for i in 1:n_edge
			if vertex_connected[i] == 0x31
				h[v,i] = true
			end
		end
		v = v + 1
	end
	
	close(io)
	return h
end