#pragma once

#ifndef _GRAPH_H
#define _GRAPH_H

#include "common.h"
#include "image.h"

//Graph Class, for handling similarity graphs and planarization

class Graph
{
	//Reference to image
	Image* image;

	//Edges of the graph in the form of pixel and 8 possible edges
	vector<vector<vector<bool>>> edges;

	//Weights for above
	vector<vector<vector<int>>> weights;
	
	//For removing trivial cross edge non-planarity
	void removeCross();

	//Heuristics for features
	void curves_heuristic(int,int);
	void sparse_pixels_heuristic(int,int);
	void islands_heuristic(int x, int y);
	
	//How many edges for the pixel (x,y)
	int valence(int x,int y);

	public:
		//Default Constructor
		Graph()
		{
			image = nullptr;
			edges.clear();
			weights.clear();
		}

		//Parametric Constructor
		Graph(Image& image);

		void planarize();
		
		//Accessors
		Image* getImage() {return image;}

		vector<vector<vector<int>>> getEdges()
		{
			return weights;
		}
		
		//Returns if there is an edge from (x,y) in kth direction
		bool edge(int x, int y, int k)
		{
			if(x >= 0 && x < image->getWidth() && y >= 0 && y < image->getHeight() && k >= 0 && k < 8) return edges[x][y][k];
			else return false;
		}
};

#endif