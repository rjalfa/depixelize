#pragma once

#ifndef _GRAPH_H
#define _GRAPH_H

#include "common.h"
#include "image.h"

class Graph
{
	Image* image;
	vector<vector<vector<bool>>> edges;
	vector<vector<vector<int>>> weights;
	
	void removeCross();
	void curves_heuristic(int,int);
	void sparse_pixels_heuristic(int,int);
	void islands_heuristic(int x, int y);
	int valence(int x,int y);

	public:
		Graph(Image& image);
		Image* getImage() {return image;}
		void planarize();
		bool edge(int x, int y, int k)
		{
			if(x >= 0 && x < image->getWidth() && y >= 0 && y < image->getHeight() && k >= 0 && k < 8) return edges[x][y][k];
			else return false;
		}
		vector<vector<vector<int>>> getEdges()
		{
			return weights;
		}
		Graph()
		{
			image = nullptr;
			edges.clear();
			weights.clear();
		}
};

#endif