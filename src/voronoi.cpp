#include "voronoi.h"

void Voronoi::createDiagram(Graph& graph)
{
	int h = imageRef->getHeight();
	int w = imageRef->getWidth();

	//Semantic
	// edges[i][j][k] -> denotes whether there is a an edge from (i,j) in kth direction in the graph

	for(int i = 0; i < w; i++) 
	{
		vector<vector<pair<bool,int>>> v1;
		for(int j = 0; j < h; j ++)
		{
			v1.push_back(vector<pair<bool,int>>(8,make_pair(false,0)));
		}
		weights.push_back(v1);
	}
}

void Voronoi::fixBoundaries()
{
	//Fix boundaries and complete the polygons to make all closed.
}