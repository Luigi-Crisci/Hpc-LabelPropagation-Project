#!/bin/bash
DEST_DIR="../resources/"

vertices=(5000 10000 20000)
edges=(300 600 1200)
perc_vertices=(30 50 70)
cardinality=()

# Check if DEST_DIR exists
if [ ! -d "$DEST_DIR" ]; then
	echo "Directory $DEST_DIR does not exist. Creating it..."
	mkdir $DEST_DIR
fi

for i in ${!vertices[@]}; do
	for curr_perc in ${perc_vertices[@]}; do
		curr_card=$((vertices[i] * curr_perc / 100))
		echo "Generating graph with ${vertices[i]} vertices, ${edges[i]} edges and cardinality $curr_card"
		filename=$DEST_DIR/hg_${vertices[i]}_${edges[i]}_${curr_card}.hgr

		#check if file already exists
		if [ -f "$filename" ]; then
			echo "Hypergraph already exists. Skipping..."
			continue
		fi

		./randHypergraph -nv ${vertices[i]} -nh ${edges[i]} -c $curr_card $filename
	done
done