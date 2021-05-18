using SimpleHypergraphs
function read_hypergraph(file::String)
    n_vertex = 0
    n_edge = 0
    io = open(file, "r")
    n_vertex = parse(Int, readline(io))
    n_edge = parse(Int, readline(io))

    h = Hypergraph(n_vertex, n_edge)
	
	vertex_connected = AbstractVector{UInt8}([])
	while !eof(io)
		readbytes!(io,vertex_connected,n_vertex * n_edge)
		for i in 1:n_vertex
			for j in 1:n_edge
				if vertex_connected[i+j] == 0x31
					h[i,j] = true
				end
			end
		end
	end
	
	close(io)
	return h
end