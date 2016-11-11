#pragma once

#ifndef _VORONOI_H
#define _VORONOI_H
#include "graph.h"
#include <vector>
#include <utility>
using namespace std;

class Voronoi
{
	Image* imageRef;
	vector<vector<vector<pair<bool,int>>>> polygons;
	vector<vector<vector<pair<int, int>>>> voronoiPts;
	public: 
		Voronoi(Image& inputImage) { imageRef = &inputImage;}
		void createDiagram(Graph& graph);
		void fixBoundaries();
};

#endif