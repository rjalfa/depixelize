#pragma once

#ifndef _GRAPH_H
#define _GRAPH_H

#include "image.h"
#include <stack>
using namespace std;
//DIRECTION MACROS
#define TOP 1
#define TOP_RIGHT 2
#define TOP_LEFT 0
#define LEFT 3
#define RIGHT 4
#define BOTTOM 6
#define BOTTOM_RIGHT 7
#define BOTTOM_LEFT 5

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
		void planarize();
		bool edge(int x, int y, int k)
		{
			return edges[x][y][k];
		}
		Graph()
		{
			image = nullptr;
			edges.clear();
			weights.clear();
		}
};

#endif