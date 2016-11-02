#include "graph.h"

int direction[8][2] = 
{
	{-1,-1},
	{-1,0},
	{-1,1},
	{0,-1},
	{0,1},
	{1,-1},
	{1,0},
	{1,1}
};

bool isValid(int x,int y, int w, int h)
{
	return (x>=0 && x<w && y>=0 && y<h);
}

Graph::Graph(Image& imageI)
{
	this->image = &imageI;
	int h = image->getHeight();
	int w = image->getWidth();

	//Semantic
	// edges[i][j][k] -> denotes whether there is a an edge from (i,j) in kth direction in the graph

	for(int i = 0; i < w; i++) 
	{
		vector<vector<bool>> v1;
		vector<vector<int>> v2;
		for(int j = 0; j < h; j ++)
		{
			v1.push_back(vector<bool>(8,false));
			v2.push_back(vector<int>(8,0));
		}
		edges.push_back(v1);
		weights.push_back(v2);
	}

	//Add edge if similar color
	for(int i = 1 ; i < w ; i++) for(int j = 0; j < h; j++) for(int k = 0 ; k < 8; k ++) {
		if((*image)(i+direction[k][0],j+direction[k][1]) != nullptr)
			edges[i][j][k] = (*image)(i,j)->isSimilar(*(*image)(i+direction[k][0],j+direction[k][1]));
	}
}

void Graph::planarize()
{
	//
}