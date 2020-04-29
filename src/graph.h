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
	std::set<std::pair<IntPoint, Direction>> edges;

	//Weights for above
	std::vector<std::vector<std::vector<int>>> weights;
	
	//For removing trivial cross edge non-planarity
	void remove_cross();

	//Heuristics for features
	void curves_heuristic(IntPoint);
	void sparse_pixels_heuristic(IntPoint);
	void islands_heuristic(IntPoint);
	
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

		std::vector<std::vector<std::vector<int>>> getEdges()
		{
			return weights;
		}
		
		// Returns if there is an edge from (x,y) in kth direction
		bool edge(int x, int y, Direction k) const
		{
			auto search_term = make_pair(IntPoint{ x, y }, k);
			auto p = edges.end();
			if ((p = edges.find(search_term)) != edges.end()) return true;
			else return false;
		}

		// Returns if there is an edge from (x,y) in kth direction
		bool edge(Pixel* p, Direction k) const
		{
			if (p) return edge(p->X(), p->Y(), k);
			else return false;
		}

		void delete_edge(int x, int y, Direction k) {
			auto search_term = make_pair(IntPoint{ x, y }, k);
			edges.erase(search_term);
		}

		// Returns if there is an edge from (x,y) in kth direction
		void delete_edge(Pixel* p, Direction k)
		{
			if (p) delete_edge(p->X(), p->Y(), k);
		}
};

#endif