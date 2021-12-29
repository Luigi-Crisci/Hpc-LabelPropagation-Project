#pragma once

#include <headers/label_propagation.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

HyperGraph* read_hypergraph(std::string filename){
	std::ifstream file;
	std::string str;
	file.open(filename);

	if (!file.is_open()) {
		std::cout << "Error opening file " << filename << std::endl;
		std::abort();
	}

	//Skip header
	std::getline(file, str);
	//Read a line from a file and convert it to an integer
	auto read_int = [&file,&str]() -> auto {
		getline(file, str);
		return std::stoi(str);
	};
	const auto num_vertices = read_int();
	const auto data_size = read_int();
	const auto num_edges = read_int();
	read_int(); // Skip the number of incoming edges

	//Read the vertices offsets
	std::vector<int> vertices_offsets(num_vertices);
	std::generate(vertices_offsets.begin(), vertices_offsets.end(), [&]() { return read_int(); });

	std::vector<int> edges_list(data_size);
	std::generate(edges_list.begin(), edges_list.end(), [&]() { return read_int(); });

	HyperGraph *hg = new HyperGraph(num_vertices, num_edges);
	//Read the edges and write it to the HyperGraph
	for (int i = 0; i < num_vertices; i++)
	{
		for (int j = vertices_offsets[i]; (i < num_vertices - 1) ? j < vertices_offsets[i + 1] : j < data_size; j++)
		{
			const auto cur_edge = edges_list[j];
			hg->add_vertex_to_edge(i, cur_edge);
		}
	}

	return hg;
} 