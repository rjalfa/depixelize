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
		vector<vector<pair<int,int>>> v2;
		for(int j = 0; j < h; j ++)
		{
			v1.push_back(vector<pair<bool,int>>(8,make_pair(false,0)));
			v2.push_back(vector<pair<int,int>>(8,make_pair(-1,-1)));
		}
		polygons.push_back(v1);
		voronoiPts.push_back(v2);
	}

	createRegions();
}

void Voronoi::createRegions()
{
	int i, j, k;
	for(i = 0; i < graph->getWidth; i++)
	{
		for(j = 0; j < graph->getHeight; j++)
		{
			

		}
	}


}

void Voronoi::fixBoundaries()
{
	//Fix boundaries and complete the polygons to make all closed.
}