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

Graph::Graph(Image& image)
{
	auto data = image.getPixels();
	for(_pixel p : data) nodes.push_back(Node(&p));
	//Create a all-direction graph
	for(int i = 0; i <nodes.size(); i++ )
	{
		_pixel p = *nodes[i].getPixel();
		for(int j = 0 ; j < 8 ; j++) if(isValid(p.getX()+direction[i][0],p.getY()+direction[i][1], image.getWidth(),image.getHeight()))
				nodes[i].setAdjacent( j, &nodes[ (p.getX()+direction[i][0]) * (image.getWidth()) + p.getY()+direction[i][1] ] );
	}

	//remove dissimilar adjacents
	for(Node n : nodes)
	{
		for(int j = 0 ; j < 8 ; j++)
		{
			if(n.getAdjacent(j) == nullptr) continue;
			if(!(n.getPixel()->isSimilar(*n.getAdjacent(j)->getPixel()))) n.setAdjacent(j,nullptr);
		}
	}
}

void Graph::planarize()
{
	//
}